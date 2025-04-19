#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
    char *cwd;
    int fd;

    cwd = getcwd(NULL, 0);
    printf("1. Current Directory: %s\n", cwd);

    // O_RDONLY = read only 
    // fd는 파일디스크립터 번호 반환
    // 파일디스크립터란 운영체제가 "열린"파일을 추적하기 위함
    // 대체적으로 0,1,2 표준입출에러이므로 3부터 반환할듯
    fd = open("bit", O_RDONLY);

    // fchdir은 파일디스크립터를 받아 change directory
    fchdir(fd);

    cwd = getcwd(NULL, 0);
    printf("2. Current Directory: %s\n", cwd);

    close(fd);
    free(cwd);
}