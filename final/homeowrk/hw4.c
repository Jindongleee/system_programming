/*
과제 2. 이름있는 파이프를 이용하여 채팅 프로그램 만들기.

    - 프로세스 1과 프로세스 2를 생성한다.
    - 프로세스 1부터 프로세스 2에게 채팅 문자열을 전달한다. 문자열의 끝은 항상 .으로 종료한다.
    - 프로세스 2는 프로세스 1로부터 .으로 종료하는 문자열을 전달받으면 채팅 문자열을 전달한다.
    - 두 프로세스 중 하나라도 채팅을 종료하고 싶으면 .만 전달한다.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define FIFO1 "fifo1"      // 프로세스 1이 쓰고 프로세스 2가 읽는 FIFO 이름
#define FIFO2 "fifo2"      // 프로세스 2가 쓰고 프로세스 1이 읽는 FIFO 이름
#define MAX_MSG 256        // 메시지 최대 길이

int main()
{
    // 이름있는 파이프(FIFO) 생성
    // FIFO1과 FIFO2는 양방향 통신을 위해 각각 한 방향씩 사용
    // 0666 권한은 모든 사용자에게 읽기/쓰기 권한을 부여
    mkfifo(FIFO1, 0666);
    mkfifo(FIFO2, 0666);

    // 프로세스 생성: fork()를 통해 부모 프로세스와 자식 프로세스 생성
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork");  // fork 실패 시 에러 출력 후 종료
        exit(1);
    }
    else if (pid == 0)
    {
        // 자식 프로세스: 프로세스 2 역할 수행
        char msg[MAX_MSG];

        // FIFO1을 읽기 전용으로 열기 (프로세스 1이 쓴 데이터를 읽음)
        int fd1 = open(FIFO1, O_RDONLY);

        // FIFO2를 쓰기 전용으로 열기 (프로세스 2가 쓴 데이터를 프로세스 1이 읽음)
        int fd2 = open(FIFO2, O_WRONLY);

        while (1)
        {
            // 프로세스 1로부터 메시지 읽기
            // read는 블록킹 호출이며, 최대 MAX_MSG - 1 바이트를 읽음
            int len = read(fd1, msg, MAX_MSG - 1);
            if (len <= 0)
                break;  // 읽기 실패 또는 파이프 닫힘 시 종료
            msg[len] = '\0';  // 문자열 종료 문자 추가

            printf("프로세스1: %s\n", msg);

            // 종료 조건: 메시지가 "."일 경우 채팅 종료
            if (strcmp(msg, ".") == 0)
                break;

            // 프로세스 2가 사용자로부터 메시지 입력 받기
            printf("프로세스2 입력 (.만 입력시 종료): ");
            fflush(stdout);
            if (fgets(msg, MAX_MSG, stdin) == NULL)
                break;  // 입력 실패 시 종료
            msg[strcspn(msg, "\n")] = 0; // fgets로 받은 문자열의 개행 문자 제거

            // 메시지 끝이 '.'로 종료되지 않으면 자동으로 '.' 추가
            int mlen = strlen(msg);
            if (mlen == 0 || msg[mlen - 1] != '.')
            {
                if (mlen < MAX_MSG - 1)
                {
                    msg[mlen] = '.';
                    msg[mlen + 1] = '\0';
                }
            }

            // 프로세스 1로 메시지 전달
            // 문자열 끝의 널 문자('\0')까지 포함하여 전송
            write(fd2, msg, strlen(msg) + 1);

            // 종료 조건: 메시지가 "."일 경우 채팅 종료
            if (strcmp(msg, ".") == 0)
                break;
        }
        // 사용한 FIFO 파일 디스크립터 닫기
        close(fd1);
        close(fd2);
        exit(0);
    }
    else
    {
        // 부모 프로세스: 프로세스 1 역할 수행
        char msg[MAX_MSG];

        // FIFO1을 쓰기 전용으로 열기 (프로세스 1이 프로세스 2에게 메시지 전송)
        int fd1 = open(FIFO1, O_WRONLY);

        // FIFO2를 읽기 전용으로 열기 (프로세스 1이 프로세스 2로부터 메시지 수신)
        int fd2 = open(FIFO2, O_RDONLY);

        while (1)
        {
            // 프로세스 1이 사용자로부터 메시지 입력 받기
            printf("프로세스1 입력 (.만 입력시 종료): ");
            fflush(stdout);
            if (fgets(msg, MAX_MSG, stdin) == NULL)
                break;  // 입력 실패 시 종료
            msg[strcspn(msg, "\n")] = 0; // fgets로 받은 문자열의 개행 문자 제거

            // 메시지 끝이 '.'로 종료되지 않으면 자동으로 '.' 추가
            int mlen = strlen(msg);
            if (mlen == 0 || msg[mlen - 1] != '.')
            {
                if (mlen < MAX_MSG - 1)
                {
                    msg[mlen] = '.';
                    msg[mlen + 1] = '\0';
                }
            }

            // 프로세스 2로 메시지 전달
            write(fd1, msg, strlen(msg) + 1);

            // 종료 조건: 메시지가 "."일 경우 채팅 종료
            if (strcmp(msg, ".") == 0)
                break;

            // 프로세스 2로부터 메시지 읽기
            int len = read(fd2, msg, MAX_MSG - 1);
            if (len <= 0)
                break;  // 읽기 실패 또는 파이프 닫힘 시 종료
            msg[len] = '\0';  // 문자열 종료 문자 추가

            printf("프로세스2: %s\n", msg);

            // 종료 조건: 메시지가 "."일 경우 채팅 종료
            if (strcmp(msg, ".") == 0)
                break;
        }
        // 사용한 FIFO 파일 디스크립터 닫기
        close(fd1);
        close(fd2);

        // 자식 프로세스 종료 대기
        wait(NULL);

        // 사용 완료된 FIFO 파일 제거 (자원 정리)
        unlink(FIFO1);
        unlink(FIFO2);
    }
    return 0;
}