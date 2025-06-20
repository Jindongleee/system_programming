/*
문제(2)
(1) 프로세스 p1은 사용자로부터 임의의 4칙연산 수식(예를 들면 10 + 7 * 2 - 5) 를 입력받아서 파일 input.txt에 저장합니다.
(2) 프로세스 p2는 input.txt 파일에 저장된 수식을 읽고 계산한 후 그 결과를 화면에 출력하세요.
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

int main(){

    //1. 파일을 열고 2.입력을 하고 3. p2에서 읽고 4. 닫기
    pid_t pid;
    char buffer[256];
    int file = open("input.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    int buf;

    fgets(buffer, SIZE, stdin);

    write(file, buffer, strlen(buffer)); // 쓰려고 하는 파일 디스크립터, 버퍼 시작 주소, 버퍼크기 strlen(buffer)은 널 종료 문자(\0) 이전까지의 문자 수를 반환합니다.
    // 따라서 hello -> hello\n\0 \0=null 이지만 \0 이전이므로 strlen(buffer) = 6
    printf("버퍼 크기: %d\n", strlen(buffer));
    printf("버퍼 마지막 문자: %c\n", buffer[strlen(buffer)-1]);
    close(file);

    pid = fork(); // 자식 프로세스 생성

    if (pid==0){
        int i = 0;
        char ptr[256];
        char *str[256];
        file = open("input.txt", O_RDONLY);

        ssize_t len = read(file, ptr, sizeof(ptr)-1);

        ptr[len] = '\0';
        close(file);

        char *tok = strtok(ptr, " ");
        while( tok != NULL ){
            str[i] = tok;
            tok = strtok(NULL, " "); 
            i++;
        }

        printf("자식 프로세스 \n");
        for (int j = 0; str[j] != NULL; j++)
            printf("%s", str[j]);

        // 후위 연산으로 바꾸기
        for (int j = 0; str[j] != NULL;j++){

        }
            exit(0); 
    }

    else{
        wait(NULL); 
    }
}
