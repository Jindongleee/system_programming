#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

int main(){
    struct stat statbuf;
    int fd;

    // file discripter if file exist -> return 0
    fd = open("linux.txt", O_RDONLY);
    
    if (fd < 0) {
        perror("open error");
        exit(1);
    }

    // fstat()함수는 열려있는 파일 정보 검색
    if (fstat(fd, &statbuf) < 0) {
        perror("fstat error");
        exit(1);
    }

    printf("Inode = %d\n", (int)statbuf.st_ino);
    printf("Mode = %o\n", (unsigned int)statbuf.st_mode);
    printf("Nlink = %d\n", (int)statbuf.st_nlink);
    printf("UID = %d\n", (int)statbuf.st_uid);
    printf("GID = %d\n", (int)statbuf.st_gid);
    printf("Size = %d\n", (int)statbuf.st_size);
    printf("Blksize = %d\n", (int)statbuf.st_blksize);
    printf("Blocks = %d\n", (int)statbuf.st_blocks);

    close(fd);

}

