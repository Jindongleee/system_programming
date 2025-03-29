#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

int main(){
    struct stat statbuf;

    stat("linux.txt", &statbuf);

    pruntf("Inode = %d\n", (int)statbuf.st_ino);
    printf("Mode = %o\n", (unsigned int)statbuf.st_mode);
    printf("Nlink = %d\n", (int)statbuf.st_nlink);
    printf("UID = %d\n", (int)statbuf.st_uid);
    printf("GID = %d\n", (int)statbuf.st_gid);
    printf("Size = %d\n", (int)statbuf.st_size);
    printf("Blksize = %d\n", (int)statbuf.st_blksize);
    printf("Blocks = %d\n", (int)statbuf.st_blocks);

    printf("** test.txt information **\n");
    
}