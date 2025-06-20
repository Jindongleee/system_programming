/*
    단, 구현할 때 파이프 이외의 기법을 사용하세요.

    과제 1.
    - 프로세스 1은 n개의 숫자를 입력받아 프로세스 2에게 전달합니다.
    - 프로세스 2는 입력받은 전체 숫자의 합을 계산하여 출력합니다.

    과제 2. (메시지 큐, 공유 메모리) -> 아마 메시지 큐로 할듯
    - 터미널 1에서 사용자로부터 공백을 입력할 때까지 임의의 개수의 이름, 성적 데이터를 입력받습니다. 홍길동 90
    - 입력받은 데이터는 다른 터미널 2의 프로세스에게 전달합니다. 터미널 2에서는 입력받은 데이터를 저장합니다.
    - 터미널 1에서 입력이 종료되면 아래와 같이 3가지 명령 중 하나를 사용자로부터 입력받습니다.

        1) 최고 성적을 받은 학생 이름과 성적 출력
        2) 평균 성적
        3) 종료

    - 터미널 2에서는 터미널 1의 명령에 적합한 데이터를 생성하여 터미널 1에게 전달합니다.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/msg.h>

struct mymsgbuf
{
    long mtype;
    char mtext[256];
};

int main()
{
    // 메시지 큐 생성

    key_t key;
    char buf[256];
    char msg[1024];
    int msgid;
    struct mymsgbuf mesg;
    char answer;

    key = ftok("keyfile", 1);              // 고유한 키값을 생성하기 위한 함수
    msgid = msgget(key, IPC_CREAT | 0644); // 메시지 큐 식별자를 얻기 위함

    while (1)
    {
        memset(buf, 0, sizeof(buf));
        fgets(buf, sizeof(buf), stdin);

        if(strcmp(buf, "\n")==0)
            break;

        mesg.mtype = 1;
        strcpy(mesg.mtext, buf);
        msgsnd(msgid, (void *)&mesg, sizeof(mesg.mtext), IPC_NOWAIT);
    }

    // int file = open("input.txt", O_RDONLY);
    // printf("원하는 것을 입력하시오");
    // scanf("%c", &answer);



}