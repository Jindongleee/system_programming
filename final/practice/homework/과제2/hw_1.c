/*
문제 (1)
(1) 프로세스 p1은 사용자로부터 n개의 숫자를 입력받습니다.
(2) 프로세스 p2는 p1이 입력받은 n개의 숫자의 합을 계산하여 출력합니다.
(3) 프로세스 p1은 p2가 종료된 이후 try again(Y/N)을 출력합니다. 사용자가 Y를 누르면 다시 (1)의 단계를 수행하세요

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#define SIZE 256

int main()
{

    pid_t pid;
    char buffer[SIZE];
    char c;

    do {
        int count = 0;
        int *num = (int *)malloc(1024);

        while (1)
        {

            if(fgets(buffer, SIZE, stdin)==NULL) // EOF임
                break;

            if (strcmp(buffer, "\n") == 0) // \n시 브레이크
                break;

            num[count] = atoi(buffer);

            count++;
        }

        pid = fork();

        if (pid == 0) 
        {
            int sum = 0;
            while(1){

                for (int i = 0; i < count;i++){
                    sum += num[i];
                }

                break;
            }
            free(num);
            printf("결과 출력: %d\n", sum);
            exit(0);
        }

        // 부모 프로세스 (숫자 입력받기)
        else
        {
            wait(NULL);

            printf("try again T/Y: ");

            scanf("%c", &c); // 이전 입력 버퍼에 남아있는 \n나 공백 문자를 건너뛰고 실제 문자 입력 받음
            getchar();
        }
    } while (c =='y');
}

//fgets() -> 한줄 단위로 "문자열"을 안전하게 입력 받을수 있다