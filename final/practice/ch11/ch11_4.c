#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>

int main(){
    key_t key;
    int shmid;

    key = ftok("shmfile", 1);

    shmid = shmget(key, 1024, IPC_CREAT | 0644);
    
}