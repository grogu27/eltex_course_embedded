#include "sem.h"
//#include "shm.h"


    //    sem_t *sem_open(const char *name, int oflag);
    //    sem_t *sem_open(const char *name, int oflag,
    //                    mode_t mode, unsigned int value);


sem_t* creat_sem(const char *name){
    sem_t *fd;
    fd = sem_open(name, O_CREAT, 0666, 0); // можно указать value = 1 и тогда семафор изначально будет равен 1
    if(fd == SEM_FAILED){
        perror("sem_open creat");
        exit(EXIT_FAILURE);
    }
    sem_post(fd);
    return fd;

}

sem_t* open_sem(const char *name){
    sem_t *fd;
    fd = sem_open(name, 0);
    if(fd == SEM_FAILED){
        perror("sem_open open");
        exit(EXIT_FAILURE);
    }
    return fd;
}

void sem_lock(sem_t* sem_ds){
    sem_wait(sem_ds);  
}

void sem_unlock(sem_t* sem_ds){
    sem_post(sem_ds);
}