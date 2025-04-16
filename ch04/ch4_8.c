#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    int fd;

    fd = open("test.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        exit(1);
    }

    // Redirect standard output to the file
    // 1: standard output
    dup2(fd, 1);    
    printf("DUP2: Standard output is redirected to test.txt\n");

    close(fd);
}