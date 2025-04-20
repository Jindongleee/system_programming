#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main(int argc, char *argv[]){

    // 문자열을 가리키는 포인터 5개짜리 배열
    // char *arg[5];

    // for (int i = 0; i < 5;i++){
    //     arg[i] = (char *)malloc(sizeof(char)*256);
    // }

    // char test[3] = {'a', 'b', 'c'};

    // printf("%s", test);

    int n;

    extern char *optarg;
    extern int optind;

    printf("Current Optind: %d\n", optind);
    while ((n=getopt(argc,argv,"abc:")) != -1){
        switch (n){

            case 'a':
                printf("Option: a\n");
                break;
            case 'b':
                printf("Option: b\n");
                break;
            case 'c':
                printf("Option:c, Argument=%s\n", optarg);
                break;
            }
            printf("Next Optind: %d\n", optind);
    }
}