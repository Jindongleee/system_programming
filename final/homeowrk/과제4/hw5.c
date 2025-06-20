#include <stdio.h>      // 표준 입출력 함수
#include <stdlib.h>     // 표준 라이브러리 함수 (예: atoi, exit 등)
#include <sys/wait.h>   // wait 함수 사용을 위한 헤더
#include <unistd.h>     // fork, remove 등의 시스템 콜 함수
#include <string.h>     // 문자열 처리 함수 (예: strtok)

#define FILENAME "temp_data.txt" // 임시 파일 이름 정의

int main()
{
    pid_t pid;               // 프로세스 ID를 저장할 변수
    int i = 0;               // 입력받은 숫자 개수 카운터
    int numbers[100];        // 최대 100개의 숫자를 저장할 배열
    char buffer[1024];       // 사용자 입력을 저장할 문자열 버퍼

    // 사용자로부터 숫자들을 한 줄로 입력받음
    printf("숫자들을 공백으로 구분하여 입력하세요 (엔터로 종료):\n");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL)
    {
        fprintf(stderr, "입력 오류\n");
        exit(1);
    }

    // 입력된 문자열을 공백 또는 개행 문자 기준으로 분리
    char *token = strtok(buffer, " \n");
    while (token != NULL && i < 100)
    {
        numbers[i++] = atoi(token);  // 문자열을 정수로 변환하여 저장
        token = strtok(NULL, " \n");
    }

    int n = i;  // 입력된 숫자의 총 개수 저장

    // 부모 프로세스가 파일에 숫자를 기록
    FILE *fp = fopen(FILENAME, "w");
    if (fp == NULL)
    {
        perror("파일 열기 실패 (부모)");
        exit(1);
    }

    for (i = 0; i < n; i++)
    {
        fprintf(fp, "%d\n", numbers[i]);  // 파일에 숫자 한 줄씩 기록
    }
    fclose(fp);  // 파일 닫기

    // 자식 프로세스를 생성
    pid = fork();

    if (pid < 0)
    {
        perror("fork 실패");
        exit(1);
    }
    else if (pid == 0)
    {
        // 자식 프로세스: 파일을 읽고 숫자들의 합 계산
        FILE *fp = fopen(FILENAME, "r");
        if (fp == NULL)
        {
            perror("파일 열기 실패 (자식)");
            exit(1);
        }

        int num, sum = 0;
        while (fscanf(fp, "%d", &num) == 1)
        {
            sum += num;  // 숫자를 읽어 합산
        }
        fclose(fp);  // 파일 닫기
        printf("총합: %d\n", sum);  // 결과 출력
        exit(0);     // 자식 프로세스 종료
    }
    else
    {
        // 부모 프로세스: 자식 프로세스 종료 대기 후 임시 파일 삭제
        wait(NULL);
        remove(FILENAME);
    }

    return 0;
}
