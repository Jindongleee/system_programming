#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX 100

// 숫자 스택 구조체 정의
typedef struct
{
    int data[MAX];
    int top;
} Stack;

// 스택 초기화 함수
void initStack(Stack *s) { s->top = -1; }
// 스택이 비었는지 확인하는 함수
int isEmpty(Stack *s) { return s->top == -1; }
// 스택에 값 추가
void push(Stack *s, int val) { s->data[++(s->top)] = val; }
// 스택에서 값 제거 및 반환
int pop(Stack *s) { return isEmpty(s) ? 0 : s->data[(s->top)--]; }


int main()
{
    // 사용자로부터 수식을 입력받을 버퍼 동적 할당
    char *input=(char *)malloc(256);
    fgets(input, 256, stdin); // 사용자로부터 한 줄 입력 받음

    // 부모 프로세스: 입력받은 수식을 파일로 저장
    FILE *fp = fopen("input.txt", "w"); // 파일 열기
    fputs(input, fp); // 입력한 수식 파일에 쓰기
    fclose(fp); // 파일 닫기

    // 자식 프로세스 생성
    pid_t pid = fork();

    if (pid == 0)
    {
        // 자식 프로세스: 파일에서 수식을 읽어와 계산 수행
        fp = fopen("input.txt", "r"); // 파일 열기

        char expr[256];
        fgets(expr, sizeof(expr), fp); // 파일에서 수식 한 줄 읽기
        fclose(fp); // 파일 닫기

        // (1) 수식 토큰 분리
        // 공백과 개행 기준으로 토큰 분리하여 token 배열에 저장
        char token[100][20];
        int count = 0;
        char *p = strtok(expr, " \n");
        while (p != NULL)
        {
            strcpy(token[count++], p);
            p = strtok(NULL, " \n");
        }

        // (2) 1차 연산: *, % 연산자 먼저 계산
        // *와 % 연산자를 만나면 앞의 숫자와 뒤의 숫자를 계산하여 newToken에 결과 저장
        char newToken[100][20];
        int newCount = 0;

        for (int i = 0; i < count; i++)
        {
            if (strcmp(token[i], "*") == 0 || strcmp(token[i], "%") == 0)
            {
                // 연산자 좌우의 숫자 추출 및 연산
                int left = atoi(newToken[--newCount]);
                int right = atoi(token[++i]);
                int result = strcmp(token[i - 1], "*") == 0 ? left * right : left % right;
                sprintf(newToken[newCount++], "%d", result); // 결과를 문자열로 저장
            }
            else
            {
                // 연산자가 아니면 그대로 newToken에 저장
                strcpy(newToken[newCount++], token[i]);
            }
        }

        // (3) 2차 연산: 남은 +, - 연산자 순차적으로 계산
        // newToken 배열을 앞에서부터 순차적으로 처리
        int result = atoi(newToken[0]);
        for (int i = 1; i < newCount; i += 2)
        {
            int num = atoi(newToken[i + 1]);
            if (strcmp(newToken[i], "+") == 0)
                result += num;
            else if (strcmp(newToken[i], "-") == 0)
                result -= num;
        }

        // 최종 결과 출력
        printf("결과: %d\n", result);
        exit(0); // 자식 프로세스 종료
    }
    else if (pid > 0)
    {
        // 부모 프로세스: 자식 프로세스 종료 대기
        wait(NULL);
    }

    return 0;
}