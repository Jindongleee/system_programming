#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define SHM_KEY 1234
#define MAX_STUDENTS 100
#define MAX_NAME_LEN 50

struct Student {
    char name[MAX_NAME_LEN];
    int score;
};

struct SharedMemory {
    int is_data_ready;
    int is_result_ready;
    int student_count;
    struct Student students[MAX_STUDENTS];
    int command;
    int result_score;
    char result_name[MAX_NAME_LEN];
    double average_score;
};

#endif 