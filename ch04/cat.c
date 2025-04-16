#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]){

    int fd, n, len;
    char buf[100];

    // argc: 인자 개수
    if(argc<2)
    {
        len = 1; //

        while(len<argc){
    
            while(read(fd, buf, 100)>0){
                write(fd,buf, 100);
            }
    
        }

    }

}