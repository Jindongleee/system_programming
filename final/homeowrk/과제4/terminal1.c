#include "common.h"

int main()
{
    int shmid;
    struct SharedMemory *shm;

    // 공유 메모리 생성
    shmid = shmget(SHM_KEY, sizeof(struct SharedMemory), IPC_CREAT | 0666);
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

    // 공유 메모리 초기화
    shm->is_data_ready = 0;
    shm->is_result_ready = 0;
    shm->student_count = 0;

    // terminal2 프로세스 생성 (fork)
    pid_t pid = fork();
    if (pid == 0)
    {
        // 자식 프로세스 - terminal2 실행
        execl("./terminal2", "terminal2", NULL);
        perror("실행 실패");
        exit(1);
    }

    // 부모 프로세스 - 사용자 입력 처리
    printf("학생 데이터를 입력하세요 (이름 성적). 빈 줄을 입력하면 종료:\n");
    char input[100];

    while (1)
    {
        if (fgets(input, sizeof(input), stdin) == NULL)
            break;
        if (input[0] == '\n')
            break;

        char name[MAX_NAME_LEN];
        int score;
        if (sscanf(input, "%s %d", name, &score) == 2)
        {
            strcpy(shm->students[shm->student_count].name, name);
            shm->students[shm->student_count].score = score;
            shm->student_count++;
        }
    }

    // 데이터 준비 완료 신호 설정
    shm->is_data_ready = 1;

    // terminal2가 데이터 처리할 때까지 대기
    while (!shm->is_result_ready)
    {
        usleep(100000); // 100ms 대기
    }

    // 메인 명령어 루프
    while (1)
    {
        printf("\nCommands:\n");
        printf("1. 최고 성적을 받은 학생 이름과 성적\n");
        printf("2. 평균 성적\n");
        printf("3. 종료\n");
        printf("명령어를 입력하세요 (1-3): ");

        int cmd;
        scanf("%d", &cmd);

        shm->command = cmd;
        shm->is_data_ready = 1;

        if (cmd == 3)
            break;

        // 결과가 준비될 때까지 대기
        while (!shm->is_result_ready)
        {
            usleep(100000);
        }

        // 결과 출력
        switch (cmd)
        {
        case 1:
            printf("최고 성적: %s with %d points\n",
                   shm->result_name, shm->result_score);
            break;
        case 2:
            printf("평균 성적: %.2f\n", shm->average_score);
            break;
        }

        // 결과 처리 완료 신호 초기화
        shm->is_result_ready = 0;
    }

    // 공유 메모리 분리 및 제거
    shmdt(shm);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}