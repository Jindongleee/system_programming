#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
    char *cwd;
    char wd1[BUFSIZ];
    char wd2[10];

    // get current working directory
    // getcwd(char *buf, size_t size);
    // buf: buffer to store the current working directory
    // size: size of the buffer
    getcwd(wd1, BUFSIZ);
    printf("wd1 = %s\n", wd1);

    // NULL: if buf is NULL, the function will allocate a buffer of size size
    // BUFSIZ: size of the buffer
    cwd = getcwd(NULL, BUFSIZ); 
    printf("cwd1 = %s\n", cwd);
    free(cwd);

    // glibc: if size is 0, the function will allocate a buffer of size BUFSIZ
    cwd = getcwd(NULL, 0);
    printf("cwd2 = %s\n", cwd);
    free(cwd);

    if(getcwd(wd2, 10) == NULL) {
        perror("getcwd");
        exit(1);
    }

}