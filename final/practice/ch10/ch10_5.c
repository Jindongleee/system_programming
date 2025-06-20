#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>

int main() {

    int fd1[2], fd2[2];
    pid_t pid;
    char buf[257];
    int len, status;

    if(pipe(fd1) == -1){
        perror("pipe");
        exit(1);
    }

    if(pipe(fd2) == -1){
        perror("pipe");
        exit(1);
    }

    switch (pid = fork()) {
        case -1:
            perror("fork");
            exit(1);
            break;
        case 0: // 자식
            close(fd1[1]); // fd1 파일 디스크립터 쓰기 닫기 / 읽기 열기
            close(fd2[0]); // fd2 파일 디스크립터 읽기 닫기 / 쓰기 열기
            len = read(fd1[0], buf, 256); // 읽기
            write(1, "Child Process:", 14); //쓰기
            write(1, buf, len);
            strcpy(buf, "Good\n");
            write(fd2[1], buf, strlen(buf));
            break;
        default: // 부모
            close(fd1[0]); // fd1 파일 디스크립터 읽기 닫기 / 쓰기 열기
            close(fd2[1]); // fd2 파일 디스크립터 쓰기 닫기 / 읽기 열기
            write(fd1[1], "Hello\n", 6); // 쓰기

            len = read(fd2[0], buf, 256); // 읽기
            write(1, "Parent Process:", 15);
            write(1, buf, len);
            waitpid(pid, &status, 0);
            break;
    }
}