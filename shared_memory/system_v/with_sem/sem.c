#include "sem.h"
#include "shm.h"

int creat_sem(key_t key, int nsems){
    int semid;
    semid = semget(key, nsems, IPC_CREAT | 0666);
    if(semid < 0){
        perror("shmget");
        exit(EXIT_FAILURE);
    }
    return semid;
}

int open_sem(key_t key, int nsems){
    int semid;
    semid = semget(key, nsems, 0666);
    if(semid < 0){
        perror("shmget");
        exit(EXIT_FAILURE);
    }
    return semid;
}
// struct sembuf{
//     unsigned short sem_num;  /* semaphore number */
//     short          sem_op;   /* semaphore operation */
//     short          sem_flg;  /* operation flags */
// }

// 0 - enable 1 - disable

void sem_lock(int sem_ds){
    struct sembuf lock[2] = {{0, 0, 0}, {0, 1, 0}};
    if(semop(sem_ds, lock, 2) < 0){
        perror("semop lock");
        exit(EXIT_FAILURE);
    }
}

void sem_unlock(int sem_ds){
    struct sembuf unlock = {0, -1, 0};
    if(semop(sem_ds, &unlock, 1) < 0){
        perror("semop unlock");
        exit(EXIT_SUCCESS);
    }
}