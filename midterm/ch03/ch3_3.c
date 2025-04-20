#include <sys/stat.h> // 파일의 상태 정보를 가져오기 위한 헤더 파일
#include <stdio.h>    // 표준 입출력 라이브러리 (printf 사용)

int main()
{
    struct stat statbuf; // 파일 정보를 저장할 구조체 변수
    int kind;            // 파일 종류를 저장할 변수

    // 상위 디렉터리("..")의 파일 정보를 가져옴
    stat("..", &statbuf);

    // 파일 모드(퍼미션 + 파일 종류) 출력
    printf("Mode = %o\n", (unsigned int)statbuf.st_mode);

    // 파일 종류만 추출 (S_IFMT 비트 마스크 적용)
    kind = statbuf.st_mode & S_IFMT;

    // 파일 종류 출력
    printf("Kind = %o\n", kind);

    switch (kind) {

        case S_IFLNK:
            printf("linux.txt:Symbolic Link\n");
            break;
        case S_IFDIR:
            printf("linux.txt:Directory \n");
            break;
        case S_IFREG:
            printf("linux.txt:Regular File\n");
            break;
    }
    
}