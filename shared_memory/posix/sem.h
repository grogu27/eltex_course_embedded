#ifndef SEM_H
#define SEM_H
//#include <sys/sem.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>

sem_t* creat_sem(const char *name);
sem_t* open_sem(const char *name);
void sem_unlock(sem_t* sem_ds);
void sem_lock(sem_t* sem_ds);


#endif