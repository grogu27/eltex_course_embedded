#include "shm.h"

//mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0); // Анонимная shared memory

int creat_shm(const char *name, off_t len){
    int fd;
    fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if(fd < 0){
        perror("shm_open");
        exit(EXIT_FAILURE);
    }
    if(ftruncate(fd, SHM_SIZE) < 0){
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }
    
    return fd;
}

int open_shm(const char *name){
    int fd;
    fd = shm_open(name, O_RDWR, 0666);
    if(fd < 0){
        perror("shm_open");
        exit(EXIT_FAILURE);
    }
    
    return fd;
}

void *attach_shm(int shmid){
    void *address = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shmid, 0);
    if(address == (void*)-1){
        perror("mmap");
        exit(EXIT_FAILURE);
    }
    return address;
}