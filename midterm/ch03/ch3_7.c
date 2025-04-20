#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

int main(){
    struct stat statbuf;


    // read, write, execute 
    chmod("linux.txt", S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH);

    // linux.txt의 메타데이터를 statbuf 구조체에 저장하는 역할
    stat("linux.txt", &statbuf);
    printf("1. Mode = %o\n", (unsigned int)statbuf.st_mode);

    statbuf.st_mode |= S_IWGRP;
    statbuf.st_mode &= ~(S_IROTH);

    chmod("linux.txt", statbuf.st_mode);

    stat("linux.txt", &statbuf);
    printf("2. Mode = %o\n", (unsigned int)statbuf.st_mode);
}