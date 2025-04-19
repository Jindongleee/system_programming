#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

/*
    struct dirent{
        ino_t d_ino; // 해당 항목의 inode 번호
        off_t d_off; // 디렉터리 오프셋의 위치
        unsigned short d_reclen; // 해당 항목의 레코드 길이
        unsigned char d_type; // 파일의 종류
        char d_name[256]; // 항목의 이름
    };
*/

int main() {
    DIR *dp;
    struct dirent *dent;

    dp = opendir(".");

    // read directory entries
    while((dent = readdir(dp))) {
        // 항목 이름
        printf("name %s ", dent->d_name);
        // inode 번호
        printf("inode %d\n", dent->d_ino);
    }
    
    closedir(dp);
}