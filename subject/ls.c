#include <stdio.h>    // printf, perror
#include <stdlib.h>   // qsort, exit
#include <dirent.h>   // opendir
#include <sys/stat.h> // 파일 정보 가져오기
#include <pwd.h>      // 파일 소유자
#include <grp.h>      // 그룹 정보
#include <time.h>     // 파일 수정시간
#include <string.h>   // strcmp, strcpy

#define MAX_FILES 1024 // 최대 파일 개수

int flag_l = 0; // 상세 정보 출력 여부
int flag_r = 0; // 역순 정렬 여부
int flag_R = 0; // 재귀 탐색 여부

// 파일 정보에 대한 구조체
typedef struct
{
    char name[256];      // 파일 이름
    struct stat statbuf; // 파일 정보
} FileInfo;

// 파일 정보 출력 함수 (-l 옵션 구현)
void print_file_info(FileInfo *file)
{
    struct stat *file_stat = &file->statbuf;

    // 파일 타입 및 권한 출력
    printf((S_ISDIR(file_stat->st_mode)) ? "d" : "-");    // 디렉토리 = d, 파일 = -
    printf((file_stat->st_mode & S_IRUSR) ? "r" : "-");   // 소유자 읽기 권한
    printf((file_stat->st_mode & S_IWUSR) ? "w" : "-");   // 소유자 쓰기 권한
    printf((file_stat->st_mode & S_IXUSR) ? "x" : "-");   // 소유자 실행 권한
    printf((file_stat->st_mode & S_IRGRP) ? "r" : "-");   // 그룹 읽기 권한
    printf((file_stat->st_mode & S_IWGRP) ? "w" : "-");   // 그룹 쓰기 권한
    printf((file_stat->st_mode & S_IXGRP) ? "x" : "-");   // 그룹 실행 권한
    printf((file_stat->st_mode & S_IROTH) ? "r" : "-");   // 기타 사용자 읽기 권한
    printf((file_stat->st_mode & S_IWOTH) ? "w" : "-");   // 기타 사용자 쓰기 권한
    printf((file_stat->st_mode & S_IXOTH) ? "x " : "- "); // 기타 사용자 실행 권한

    // 하드 링크 개수 출력
    printf("%2lu ", file_stat->st_nlink);

    // 소유자 및 그룹 출력
    struct passwd *pw = getpwuid(file_stat->st_uid);
    struct group *gr = getgrgid(file_stat->st_gid);
    printf("%s %s ", pw->pw_name, gr->gr_name);

    // 파일 크기 출력
    printf("%8ld ", file_stat->st_size);

    // 파일 수정 시간 출력
    char time_buf[80];
    struct tm *time_info = localtime(&file_stat->st_mtime);
    strftime(time_buf, sizeof(time_buf), "%b %d %H:%M", time_info);
    printf("%s ", time_buf);

    // 파일 이름 출력
    printf("%s\n", file->name);
}

// 파일 정렬 함수 (-r 옵션 구현)
int compare_files(const void *a, const void *b)
{
    FileInfo *file1 = (FileInfo *)a;
    FileInfo *file2 = (FileInfo *)b;

    // 기본적으로 오름차순 정렬, -r 옵션이 있으면 내림차순 정렬
    return flag_r ? strcmp(file2->name, file1->name) : strcmp(file1->name, file2->name);
}

// 디렉토리 목록 출력 함수 (-R 옵션 구현)
void list_directory(const char *path, int first_call)
{
    struct dirent *entry;
    DIR *dir = opendir(path);
    if (!dir)
    {
        perror("opendir"); // 디렉토리를 열 수 없는 경우 에러 출력
        return;
    }

    FileInfo files[MAX_FILES];     // 파일 정보를 저장할 배열
    int count = 0;                 // 파일 개수
    char subdirs[MAX_FILES][1024]; // 하위 디렉토리 저장 (재귀 탐색용)
    int subdir_count = 0;          // 하위 디렉토리 개수

    // 디렉토리 내 파일 탐색
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_name[0] == '.') // 숨김 파일(.으로 시작하는 파일) 제외
            continue;

        FileInfo file;
        snprintf(file.name, sizeof(file.name), "%s", entry->d_name); // 파일 이름 저장

        char full_path[1024]; // 파일의 전체 경로 생성
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        // 파일의 상세 정보(stat 구조체) 가져오기
        if (stat(full_path, &file.statbuf) == -1)
        {
            perror("stat"); // 파일 정보 가져오기 실패 시 에러 출력
            continue;
        }

        files[count++] = file; // 파일 정보를 배열에 저장

        // -R 옵션이 설정되어 있고, 현재 파일이 디렉토리라면 하위 디렉토리 목록에 추가
        if (flag_R && S_ISDIR(file.statbuf.st_mode))
        {
            snprintf(subdirs[subdir_count++], sizeof(subdirs[subdir_count]), "%s", full_path);
        }
    }
    closedir(dir); // 디렉토리 닫기

    // 파일 정렬 (기본 오름차순, -r 옵션 시 내림차순)
    qsort(files, count, sizeof(FileInfo), compare_files);

    // 첫 호출이 아닐 때만 경로 출력 (현재 디렉토리 `.`을 출력하지 않도록 수정)
    if (!first_call)
        printf("\n%s:\n", path);

    // 파일 목록 출력
    for (int i = 0; i < count; i++)
    {
        if (flag_l)
            print_file_info(&files[i]); // -l 옵션이 있으면 상세 정보 출력
        else
            printf("%s  ", files[i].name); // 기본 파일 이름만 출력
    }
    printf("\n");

    // 재귀 탐색 (-R 옵션)
    if (flag_R)
    {
        for (int i = 0; i < subdir_count; i++)
        {
            list_directory(subdirs[i], 0); // 재귀 호출 시 first_call = 0
        }
    }
}

// 옵션 처리 함수
void parse_arguments(int argc, char *argv[], char **target_path)
{
    for (int i = 1; i < argc; i++) // 첫 번째 인자는 프로그램 이름이므로 1부터 시작
    {
        if (argv[i][0] == '-') // 옵션이 포함된 인자인 경우
        {
            for (int j = 1; argv[i][j] != '\0'; j++) // 옵션 문자열을 하나씩 확인
            {
                switch (argv[i][j])
                {
                case 'l': // -l 옵션: 상세 정보 출력
                    flag_l = 1;
                    break;
                case 'r': // -r 옵션: 파일 정렬을 역순으로 변경
                    flag_r = 1;
                    break;
                case 'R': // -R 옵션: 하위 디렉토리까지 재귀적으로 출력
                    flag_R = 1;
                    break;
                default: // 지원하지 않는 옵션일 경우 에러 출력 후 종료
                    fprintf(stderr, "Unknown option: -%c\n", argv[i][j]);
                    exit(1);
                }
            }
        }
        else // 옵션이 아닌 경우 (디렉토리 경로)
        {
            *target_path = argv[i]; // 탐색할 경로 저장
        }
    }
}

// 프로그램 시작점
int main(int argc, char *argv[])
{
    char *path = ".";                   // 기본 경로: 현재 디렉토리(".")
    parse_arguments(argc, argv, &path); // 명령어 옵션 및 경로 처리
    list_directory(path, 1);            // 디렉토리 목록 출력 실행 (첫 호출 표시)
    return 0;
}