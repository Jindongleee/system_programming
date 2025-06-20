#include "common.h"

void process_highest_score(struct SharedMemory *shm)
{
    int highest_score = -1;
    int highest_index = -1;

    // 최고 성적을 받은 학생 찾기
    for (int i = 0; i < shm->student_count; i++)
    {
        if (shm->students[i].score > highest_score)
        {
            highest_score = shm->students[i].score;
            highest_index = i;
        }
    }

    // 최고 성적을 받은 학생 이름과 성적 저장
    if (highest_index != -1)
    {
        strcpy(shm->result_name, shm->students[highest_index].name);
        shm->result_score = highest_score;
    }
}

// 평균 성적 계산
void process_average_score(struct SharedMemory *shm)
{

    // 합계 계산
    double sum = 0;

    // 학생 수가 0인 경우 평균 성적은 0
    if (shm->student_count == 0)
    {
        shm->average_score = 0;
        return;
    }

    // 학생 수만큼 반복하여 합계 계산
    for (int i = 0; i < shm->student_count; i++)
    {
        sum += shm->students[i].score;
    }
    shm->average_score = sum / shm->student_count;
}

int main()
{
    int shmid;
    struct SharedMemory *shm;

    // 공유 메모리 생성
    shmid = shmget(SHM_KEY, sizeof(struct SharedMemory), 0666);
    if (shmid == -1)
    {
        perror("공유 메모리 생성 실패");
        exit(1);
    }

    // 공유 메모리 연결
    shm = (struct SharedMemory *)shmat(shmid, NULL, 0);
    if (shm == (void *)-1)
    {
        perror("공유 메모리 연결 실패");
        exit(1);
    }

    // 메인 처리 루프
    while (1)
    {
        // 데이터가 준비되었는지 확인
        if (shm->is_data_ready)
        {

            switch (shm->command)
            {
            case 1:
                process_highest_score(shm);
                break;
            case 2:
                process_average_score(shm);
                break;
            case 3:
                // Exit
                shmdt(shm);
                return 0;
            }
            shm->is_data_ready = 0;
            shm->is_result_ready = 1;
        }
        usleep(100000); // 100ms 대기
    }

    return 0;
}