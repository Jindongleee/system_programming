/*
수업시간에 배운 파이프를 이용하여 아래의 기능을 구현하세요.

과제 1.

    - 프로세스 1은 키보드로 n개의 디렉토리 이름을 입력받아 프로세스 2에게 전달한다. "//"을 입력하면 더이상 입력받지 않는다. (디렉토리는 상대경로/절대경로 모두 가능합니다)
    - 프로세스 2는 프로세스 1로부터 디렉토리를 전달받으면 그 디렉토리에 있는 파일의 총 개수를 계산하여 저장한다.
    - 프로세스 2는 프로세스 1로부터 -1로부터 "//"을 넘겨받으면 지금까지 계산한 파일의 총 개수를 프로세스 1에게 전달한다.
    - 최종적으로 프로세스 1은 총 파일의 개수를 화면에 출력하고 종료한다.
*/

#include <dirent.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_DIR_NAME 256

int main()
{
    // 파이프 2개 생성: fd1은 부모->자식 통신용, fd2는 자식->부모 통신용
    // 파이프는 양방향 통신이 불가능하므로 두 개를 사용하여 양방향 통신 구현
    int fd1[2]; // 부모가 쓰고 자식이 읽음
    int fd2[2]; // 자식이 쓰고 부모가 읽음

    // 파이프 생성, 실패 시 에러 출력 후 종료
    if (pipe(fd1)==-1 || pipe(fd2) == -1)
    {
        perror("pipe");
        exit(1);
    }

    // 프로세스 생성 (fork)
    // 부모 프로세스는 디렉토리 이름을 입력받아 자식에게 전달
    // 자식 프로세스는 디렉토리 내 파일 개수를 계산하여 부모에게 전달
    pid_t pid = fork();

    if (pid < 0)
    {
        // fork 실패 시 에러 출력 후 종료
        perror("fork");
        exit(1);
    }
    else if (pid == 0)
    {
        // 자식 프로세스 영역

        // fd1 파이프에서 쓰기 끝단 닫기: 자식은 부모가 쓴 데이터를 읽기만 함
        close(fd1[1]);

        // fd2 파이프에서 읽기 끝단 닫기: 자식은 결과를 쓰기만 함
        close(fd2[0]);

        char dirname[MAX_DIR_NAME];
        int total = 0; // 총 파일 개수 누적 변수
        DIR *dirp;
        struct dirent *entry;

        // 부모로부터 디렉토리 이름을 계속 읽음
        while (1)
        {
            int len = 0;
            // 먼저 디렉토리 이름 길이를 읽음 (int 단위)
            // read가 정상적으로 len 크기만큼 읽지 못하면 반복 종료
            if (read(fd1[0], &len, sizeof(int)) != sizeof(int))
                break;

            // 길이가 0 이하이거나 버퍼 크기 초과 시 종료 (비정상 데이터 방지)
            if (len <= 0 || len > MAX_DIR_NAME)
                break;

            // 디렉토리 이름 읽기, 길이만큼 읽지 못하면 종료
            if (read(fd1[0], dirname, len) != len)
                break;

            // 문자열 종료 문자 삽입
            dirname[len] = '\0';

            // 종료 신호 "//" 입력 시 반복 종료
            if (strcmp(dirname, "//") == 0)
                break;

            // 디렉토리 열기, 실패 시 해당 디렉토리는 건너뜀
            dirp = opendir(dirname);
            if (dirp == NULL)
                continue;

            // 디렉토리 내 엔트리 순회
            // d_type이 DT_REG인 경우 일반 파일이므로 총 개수 증가
            while ((entry = readdir(dirp)) != NULL) {
                if (entry->d_type == DT_REG)
                    total++;
            }
            closedir(dirp);
        }

        // 계산된 총 파일 개수를 부모에게 전달
        // write 실패 시 별도 처리 없음, 종료 준비
        write(fd2[1], &total, sizeof(int));

        // 파이프 닫기 (자원 해제)
        close(fd1[0]);
        close(fd2[1]);

        // 자식 프로세스 정상 종료
        exit(0);
    }
    else
    {
        // 부모 프로세스 영역

        // fd1 파이프에서 읽기 끝단 닫기: 부모는 쓰기만 하므로 읽기 끝단 닫음
        close(fd1[0]);

        // fd2 파이프에서 쓰기 끝단 닫기: 부모는 읽기만 하므로 쓰기 끝단 닫음
        close(fd2[1]);

        char dirname[MAX_DIR_NAME];

        // 사용자로부터 디렉토리 이름을 입력받아 자식에게 전달
        while (1)
        {
            printf("디렉토리 이름 입력 (종료: //): ");

            // fgets로 입력받음, NULL이면 입력 종료
            if (fgets(dirname, MAX_DIR_NAME, stdin) == NULL)
                break;

            // 입력된 문자열에서 개행 문자 제거
            dirname[strcspn(dirname, "\n")] = 0;

            int len = strlen(dirname);

            // 디렉토리 이름 길이와 문자열을 파이프를 통해 자식에게 전달
            write(fd1[1], &len, sizeof(int));
            write(fd1[1], dirname, len);

            // 종료 신호 "//" 입력 시 반복 종료
            if (strcmp(dirname, "//") == 0)
                break;
        }

        // 자식 프로세스로부터 총 파일 개수를 읽음
        int total = 0;
        if (read(fd2[0], &total, sizeof(int)) == sizeof(int))
        {
            // 정상적으로 읽은 경우 결과 출력
            printf("총 파일 개수: %d\n", total);
        }

        // 파이프 닫기 (자원 해제)
        close(fd1[1]);
        close(fd2[0]);

        // 자식 프로세스 종료 대기
        wait(NULL);
    }
    return 0;
}
