#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


int main(){
    char *cwd;

    // getcwd -> 자동으로 동적메모리 할당
    cwd = getcwd(NULL, 0);
    printf("1.Current Directory: %s\n", cwd);

    if(chdir("bi")== -1){
        perror("Can't chdir");
        exit(1);
    }

    cwd = getcwd(NULL, BUFSIZ);

    printf("2.Current Directory: %s\n", cwd);

    free(cwd);
}