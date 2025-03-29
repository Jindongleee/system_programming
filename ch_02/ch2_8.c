#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

int main() {
    DIR *dp;
    struct dirent *dent;

    dp = opendir(".");

    // read directory entries
    while((dent = readdir(dp))) {
        printf("name %s\n", dent->d_name);
        printf("inode %d\n", dent->d_ino);
    }
    
    closedir(dp);
}