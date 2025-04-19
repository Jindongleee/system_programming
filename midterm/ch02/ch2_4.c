#define _GNU_SOURCE // GNU 확장 기능 활성화
#include <unistd.h> // get_current_dir_name 정의 포함
#include <stdlib.h> // malloc, free
#include <stdio.h>  // printf

int main()
{
    char *cwd;

    cwd = get_current_dir_name(); // 현재 디렉토리의 경로를 동적 할당해서 반환
    printf("cwd = %s\n", cwd);    // 경로 출력
    free(cwd);                    // 할당된 메모리 해제
}