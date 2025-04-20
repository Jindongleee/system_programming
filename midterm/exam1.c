#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//엔터가 끝날때까지 문자를 받고 합쳐라


    // 256*sizeof(char) = sizeof(char)는 1byte 따라서 256byte -> malloc (할당하라) 
    // -> malloc은 void 반환이므로 char형으로 반환하라 (명시적 형변환)
    //char *carr = (char *)malloc(256*sizeof(char));

#include <stdio.h>

int main()
{
    // int a = 10;
    // int *p = &a;
    // int **pp = &p;

    // printf("a = %d\n", a);
    // printf("*p = %d\n", *p);
    // printf("**pp = %d\n", **pp);

    // printf("&a = %p\n", &a);
    // printf("p = %p\n", p);
    // printf("*pp = %p\n", *pp);
    // printf("pp = %p\n", pp);

    // return 0;

    // char *ptr[1];
    // ptr[0] = (char *)malloc(256 * sizeof(char));

    // scanf("%s", ptr[0]);

    // printf("%s\n", ptr[0]);

    char ptr;
    scanf("%s", &ptr);

    printf("%c", ptr);
}