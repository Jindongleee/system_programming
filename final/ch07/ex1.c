#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

int main(){

    int a;
    int pid = fork();

    if (pid > 0)
    {

        for (a = 1; a < 9; a++){
            printf("P: 2 * %d = %d\n", a, 2*a);
        }
        sleep(10);
    }
    else if(pid == 0){
        execl("ls", "ls" , "- l" ,NULL);
            for (a = 1; a < 9; a++)
        {
            printf("C:3 * %d = %d\n", a, 3*a);
        }
        sleep(10);
    }
}