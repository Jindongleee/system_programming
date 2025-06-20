#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(){
    int shmid, i;
    char *shmaddr, *shmaddr2;

    // 공유 메모리 20byte 생성 및 공유
    shmid = shmget(IPC_PRIVATE, 20, IPC_CREAT | 0644);

    switch (fork()) {
        case -1:
            perror("fork");
            exit(1);
            break;
        case 0:
            shmaddr = (char *)shmat(shmid, (char *)NULL, 0); // 공용 메모리 붙이기 + 주소 저장
            printf("Child Process ====\n");
            for (int i = 0; i < 10;i++)
                shmaddr[i] = 'a' + i;
            shmdt((char *)shmaddr);
            exit(0);
            break;
        default:
            wait(0); // 자식이 죽었다 가정
            shmaddr2 = (char *)shmat(shmid, (char *)NULL, 0); // shmaddr 변수 이름이 같아도 무방 fork는 나뉨
            printf("Parent Process ====\n");
            for (int i = 0; i < 10;i++)
                printf("%c ", shmaddr2[i]);
            printf("\n");
            sleep(5);
            shmdt((char *)shmaddr2);
            shmctl(shmid, IPC_RMID, (struct shmid_ds *)NULL);
            break;
    }
}