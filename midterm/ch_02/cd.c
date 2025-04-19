#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(){

    char *cwd;
    cwd = getcwd(NULL, BUFSIZ);
    printf("1. Current Directory: %s\n", cwd);

    // change directory
    chdir("test"); // shell이 수행한 것이 아님 따라서 실제(shell 상에서) 디렉토리가 변경되지 않음


    cwd = getcwd(NULL, BUFSIZ);
    printf("2. Current Directory: %s\n", cwd);

    free(cwd);
    return 0;
}