#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>

int main(){
    printf("Before SIGCOONT Signal to parent.\n");
    kill(getppid(), SIGCONT); //부모 pid를 킬한것이기 떄문에 내 프로세스는 영향 x

    printf("Before SIGQUIT Signal to me .\n"); // 
    kill(getpid(), SIGQUIT); // 자기 자신에게 시그널 보냄

    printf("After SIGQUIT Signal.\n");
}