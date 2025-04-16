#include <stdio.h>

int main(){

    int a;
    int *p = &a;

    printf("Hello World\n");
    scanf("%d", &a);

    printf("%d\n", a);

    printf("%d", p);
}

/*
    출력장치, 입력장치도 모두 파일 프로세스 동작시 운영체제가 3가지 연다
    1. 표준 입력
    2. 표준 출력
    3. 표준 에러

    buffer 단위 전달 -> &scanf

    printf("%d", t); -> printf는 표준 출력으로 전달
    fprintf(fp, "%d", t); -> fp처럼 장치를 주는게 중요함

    char buf[100];
    int n = 100;
    read(fp,buf, n); -> buf = 시작주소, n은 크기
*/