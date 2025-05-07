#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
    pid_t pid;

    switch(pid=fork()){

        case -1: /* fork failed */
            perror("fork");
            exit(1);
            break;
        case 0: /*child process*/
            printf("Child process - My PId:%d, My Parent's PID:%d\n", (int)getpid(), (int)getppid());
            break;

        default: /*parent process*/
            printf("Parent process = My PID:%d, MY Parent's PID:%d, My Child's PID:%d\n", (int)getpid(), (int)getppid(), (int)pid);
            break;
    }
    printf("End of fork\n");
}
