#ifndef SHM_H
#define SHM_H

#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/types.h>

#define SHM_NAME "server"
#define SEM_NAME "server"
#define SHM_SIZE 2048 // выделится одна страница озу(4KB)

struct data{
    pid_t pid;
    char text[255];
};

key_t gen_key(const char *pathname, int proj_id);
int creat_shm(key_t key, size_t size);
int open_shm(key_t key, size_t size);
void *attach_shm(int shmid, const void *shmaddr, int shmflg);

#endif