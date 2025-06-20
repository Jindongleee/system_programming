#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// 메시지 담고 있는 버퍼 구조체
struct mymsgbuf{
    long mtype;
    char mtext[80];
};

int main(){

    // 키 값 = 식별자
    key_t key;
    
    // 메시지큐 식별자
    int msgid;
    struct mymsgbuf mesg;

    // 메시지 큐 생성
    key = ftok("keyfile", 1);
    msgid = msgget(key, IPC_CREAT | 0644);

    // 메시지 큐 실패
    if (msgid == -1){
        perror("msgget");
        exit(1);
    }

    // 1번 타입 (메시지 큐 안에 여러개의 타입의 메시지가 들어가있음)
    mesg.mtype = 1;
    strcpy(mesg.mtext, "Message Q Test");

    //전송
    if(msgsnd(msgid, (void *)&mesg, 80, IPC_NOWAIT) == -1){
        perror("msgsnd");
        exit(1);
    }
}