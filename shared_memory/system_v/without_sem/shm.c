#include "shm.h"


key_t gen_key(const char *pathname, int proj_id){
    key_t key;
    key = ftok(pathname, proj_id);
    if(key < 0){
        perror("ftok");
        exit(EXIT_FAILURE);
    }
    return key;
}


int creat_shm(key_t key, size_t size){
    int shm_ds;
    shm_ds = shmget(key, size, IPC_CREAT | 0666);
    if(shm_ds < 0){
        perror("shmget");
        exit(EXIT_FAILURE);
    }
    return shm_ds;
}

int open_shm(key_t key, size_t size){
    int shm_ds;
    shm_ds = shmget(key, size, 0666);
    if(shm_ds < 0){
        perror("shmget");
        exit(EXIT_FAILURE);
    }
    return shm_ds;
}

void *attach_shm(int shmid, const void *shmaddr, int shmflg){
    void *address = NULL;
    address = shmat(shmid, NULL, 0); // 0 значит чтение и запись
    if(address == (void*)-1){
        perror("shmat");
        exit(EXIT_FAILURE);
    }
    return address;
}