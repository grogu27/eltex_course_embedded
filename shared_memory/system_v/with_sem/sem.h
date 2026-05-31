#ifndef SEM_H
#define SEM_H
#include <sys/sem.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


int creat_sem(key_t key, int nsems);
int open_sem(key_t key, int nsems);
void sem_lock();
void sem_unlock();


#endif