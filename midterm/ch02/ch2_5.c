#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>

int main(){

    //if rename success return -> 0 else -> -1
    if(rename("ha","bit") == -1){
        perror("rename");
        exit(1);
    }
    
}