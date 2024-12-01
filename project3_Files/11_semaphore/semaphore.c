#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define SHM_SIZE 1024 // 공유 메모리 크기
#define SEM_PARENT 0  // 부모(읽기) 세마포어 인덱스
#define SEM_CHILD 1   // 자식(쓰기) 세마포어 인덱스

// 세마포어 락 함수
void sem_lock(int semid, int sem_num) {
    struct sembuf sb = {sem_num, -1, 0}; // 세마포어 감소
    semop(semid, &sb, 1);
}

// 세마포어 언락 함수
void sem_unlock(int semid, int sem_num) {
    struct sembuf sb = {sem_num, 1, 0}; // 세마포어 증가
    semop(semid, &sb, 1);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source_file> <destination_file>\n", argv[0]);
        exit(1);
    }

    // 공유 메모리 및 세마포어 생성
    key_t shm_key = ftok(".", 'A');
    key_t sem_key = ftok(".", 'B');

    int shmid = shmget(shm_key, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    int semid = semget(sem_key, 2, IPC_CREAT | 0666); // 두 개의 세마포어 생성
    if (semid == -1) {
        perror("semget");
        exit(1);
    }

    // 세마포어 초기화
    semctl(semid, SEM_PARENT, SETVAL, 1); // 부모 세마포어: 1 (읽기 가능)
    semctl(semid, SEM_CHILD, SETVAL, 0); // 자식 세마포어: 0 (쓰기 대기)

    // 공유 메모리 연결
    char *shared_mem = (char *)shmat(shmid, NULL, 0);
    if (shared_mem == (char *)-1) {
        perror("shmat");
        exit(1);
    }

    pid_t pid = fork();

    if (pid == 0) {
        // 자식 프로세스: 쓰기
        FILE *dest_file = fopen(argv[2], "w");
        if (!dest_file) {
            perror("fopen (destination)");
            exit(1);
        }

        while (1) {
            sem_lock(semid, SEM_CHILD); // 쓰기 잠금
            if (strcmp(shared_mem, "EOF") == 0) {
                break; // 읽기 종료
            }
            fprintf(dest_file, "%s", shared_mem); // 공유 메모리 내용 파일에 쓰기
            sem_unlock(semid, SEM_PARENT); // 읽기 잠금 해제
        }

        fclose(dest_file);
        shmdt(shared_mem); // 공유 메모리 분리
        exit(0);
    } else if (pid > 0) {
        // 부모 프로세스: 읽기
        FILE *src_file = fopen(argv[1], "r");
        if (!src_file) {
            perror("fopen (source)");
            exit(1);
        }

        while (fgets(shared_mem, SHM_SIZE, src_file)) {
            sem_lock(semid, SEM_PARENT); // 읽기 잠금
            sem_unlock(semid, SEM_CHILD); // 쓰기 잠금 해제
        }

        // EOF 처리
        sem_lock(semid, SEM_PARENT); // 읽기 잠금
        strcpy(shared_mem, "EOF");
        sem_unlock(semid, SEM_CHILD); // 쓰기 잠금 해제

        fclose(src_file);
        wait(NULL); // 자식 프로세스 종료 대기
        shmdt(shared_mem); // 공유 메모리 분리
        shmctl(shmid, IPC_RMID, NULL); // 공유 메모리 삭제
        semctl(semid, 0, IPC_RMID);   // 세마포어 삭제
    } else {
        perror("fork");
        exit(1);
    }

    return 0;
}
