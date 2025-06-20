#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int fd[2];
    int value;
    int status;

    if (pipe(fd) == -1) // pipe 생성
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    // 자식 프로세스  0 = 입력 , 1 = 출력, 2 = 에러
    if (pid == 0)
    {
        close(fd[1]); // write 닫기
        int num;
        read(fd[0], &num, sizeof(int)); // 출력 활성화
        printf("%o\n", num);
        int a = 1, b = 0;
        while (num > 0)
        {
            b = b + (num % 8) * a;
            a *= 10;
            num = num / 8;
        }
        close(fd[0]); // 출력 닫기
    }

    // 부모 프로세스
    else
    {
        close(fd[0]); // read 닫기
        scanf("%d", &value);

        write(fd[1], &value, sizeof(int));

        close(fd[1]);
        waitpid(pid, &status, 0);
    }

    return 0;
}