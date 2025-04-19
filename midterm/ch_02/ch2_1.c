#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>

// make a directory
int main() {
    if(mkdir("han", 0755) == -1) {
        perror("han");
        exit(1);
    }
}