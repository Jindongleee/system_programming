#include <stdio.h>    // 표준 입출력 라이브러리
#include <stdlib.h>   // malloc, atoi 등의 함수 사용을 위한 라이브러리
#include <string.h>   // 문자열 처리를 위한 라이브러리
#include <unistd.h>   // fork, exit 등의 시스템 호출을 위한 라이브러리
#include <sys/wait.h> // wait() 함수 사용을 위한 라이브러리

int main()
{
    while (1) // 사용자가 'N'을 입력하기 전까지 계속 반복
    {
        char buffer[256];                                // 사용자 입력을 저장할 문자열 버퍼
        int *number = (int *)malloc(sizeof(int) * 1024); // 최대 1024개의 정수를 저장할 동적 배열
        int i = 0;                                       // 입력받은 숫자의 개수를 저장
        int sum = 0;                                     // 숫자들의 합을 저장
        char input;                                      // 사용자로부터 Y/N을 입력받을 변수

        // 사용자로부터 숫자를 입력받는 반복문 (빈 줄 입력 시 종료)
        while (1)
        {
            if (fgets(buffer, sizeof(buffer), stdin) == NULL) // 입력이 종료되었을 경우 반복 종료
            {
                break;
            }

            if (buffer[0] == '\n') // 빈 줄(Enter) 입력 시 반복 종료
            {
                break;
            }

            number[i] = atoi(buffer); // 입력된 문자열을 정수로 변환하여 배열에 저장
            i++;                      // 입력된 숫자 개수 증가
        }

        // 입력된 숫자가 없을 경우 다시 반복문 처음으로
        if (i == 0)
        {
            free(number); // 동적 메모리 해제
            continue;
        }

        pid_t pid = fork(); // 자식 프로세스 생성

        if (pid == 0) // 자식 프로세스인 경우
        { 
            for (int j = 0; j < i; j++)
            {
                sum += number[j]; // 숫자들의 합 계산
            }

            printf("출력: %d\n", sum); // 계산 결과 출력
            free(number);              // 동적 메모리 해제
            exit(0);                   // 자식 프로세스 종료
        }
        else if (pid > 0)
        {               // 부모 프로세스인 경우
            wait(NULL); // 자식 프로세스가 종료될 때까지 대기
        }


        // 사용자에게 반복 여부를 묻는 메시지 출력
        printf("try again (Y/N): ");
        scanf(" %c", &input); // 사용자 입력 받기 (공백으로 이전 개행 제거)

        if (input == 'Y' || input == 'y') // 사용자가 Y 또는 y를 입력한 경우
        {
            free(number); // 동적 메모리 해제
            continue;     // 다시 입력 받기 위해 반복
        }
        else
        {
            free(number); // 동적 메모리 해제
            break;        // 프로그램 종료
        }
    }

    return 0; // 정상 종료
}