#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

int main(){
    struct stat statbuf;

    // 파일을 직접 넘겨줌
    stat("linux.txt", &statbuf);

    // inode
    printf("Inode = %d\n", (int)statbuf.st_ino);
    
    // 파일의 종류와 접근 권한
    printf("Mode = %o\n", (unsigned int)statbuf.st_mode);

    // 하드 링크의 개수
    printf("Nlink = %d\n", (int)statbuf.st_nlink);

    // 파일 소유자의 UID
    printf("UID = %d\n", (int)statbuf.st_uid);

    // 파일 소유 그룹의 GID를 저장한다.
    printf("GID = %d\n", (int)statbuf.st_gid);

    printf("Size = %d\n", (int)statbuf.st_size);

    printf("Blksize = %d\n", (int)statbuf.st_blksize);

    printf("Blocks = %d\n", (int)statbuf.st_blocks);

    printf("** timespec Style **\n");
    printf("Atime = %ld\n", (int)statbuf.st_atimespec.tv_sec);
    printf("Mtime = %ld\n", (int)statbuf.st_mtimespec.tv_sec);
    printf("Ctime = %ld\n", (int)statbuf.st_ctimespec.tv_sec);

    printf("** old Style **\n");
    printf("Atime = %ld\n", (int)statbuf.st_atime);
    printf("Mtime = %ld\n", (int)statbuf.st_mtime);
    printf("Ctime = %ld\n", (int)statbuf.st_ctime);
    
}
