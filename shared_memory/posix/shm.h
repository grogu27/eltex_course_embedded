#ifndef SHM_H
#define SHM_H

#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
 #include <string.h>
 
#define SHM_NAME "/server"
#define SEM_NAME "/server"
#define SHM_SIZE 2048 // выделится одна страница озу(4KB)

struct data{
    pid_t pid;
    char text[255];
};

int creat_shm(const char *name, off_t len);
int open_shm(const char *name);
void *attach_shm(int shmid);

#endif