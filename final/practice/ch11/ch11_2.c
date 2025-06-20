#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>

// 송신측과 같아야함
struct mymsgbuf{
    long mtype;
    char mtext[80];
};

int main() {

    struct mymsgbuf inmsg;
    key_t key;
    int msgid, len;

    // P1이 이미 수행되었다고 가정
    key = ftok("keyfile", 1);
    msgid = msgget(key, 0);

    len = msgrcv(msgid, &inmsg, 80, 0, 0);
    printf("Received Msg = %s, Len = %d\n", inmsg.mtext, len);
}