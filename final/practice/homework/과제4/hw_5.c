#include <stdio.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

struct mymsgbuf {
    long mtype;
    char mtext[256];
};

struct studentData{
    char name[256];
    float score;
};

int main()
{

    struct mymsgbuf inmsg;
    key_t key;
    int msgid;
    int file = open("input.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);

    key = ftok("keyfile", 1);
    msgid = msgget(key, 0);

    // 데이터 저장
    while(1){
        msgrcv(msgid, &inmsg, 256, 0, 0);
        printf("%s ", inmsg.mtext);

        if(strcmp(inmsg.mtext, "\n") == 0)
            break;
        write(file, inmsg.mtext, strlen(inmsg.mtext));
    }
    close(file);

    char answer[2];
    scanf("%s", answer);

    // 최고 성적을 받은 학생 이름과 성적 출력 -> 성적과 
    if(atoi(answer) == 1){
        char buf[256];
        int file = open("input.txt", O_RDONLY);
        struct studentData data[3];
        int i = 0;
        while (strcmp(buf, "\n") != 0)
        {
            read(file, buf, 7);
            char *tok = strtok(buf, " ");
            while(tok != NULL);{
                strcpy(data[i].name, tok);
                tok = strtok(NULL, " ");
                data[i].score = atoi(tok);
            }

            printf("%s", buf);
        }

        int max = data[0].score;

        for (int i = 0; i < 3;i++){
            if(data[i].score > max )
                max = data[i].score;
        }

        
    }
    // 평균 성적
    if(atoi(answer)==2){
        printf("Hello 2");
    }

    // 종료
    if(atoi(answer)==3){
        exit(0);
    }
}