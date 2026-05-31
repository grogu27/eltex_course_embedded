#include "shm.h"
#include "sem.h"
#include <signal.h>

int main(){
    
    printf("PID: %d\n", getpid());
    int shm_ds;
    sem_t *sem_ds;
    void *address = NULL;
    struct data *server_data = NULL;
    struct data *client_data = NULL;


    shm_ds = open_shm(SHM_NAME);
    sem_ds = open_sem(SEM_NAME);

    address = attach_shm(shm_ds);

    server_data = (struct data*)address;
    printf("Server write data\n");
    printf("Server pid: %d\nServer text: %s\n", server_data->pid, server_data->text);
    int pid_serv = server_data->pid;
    
    sem_lock(sem_ds);

    client_data = server_data;
    client_data->pid = getpid();
    sprintf(client_data->text, "%s", "Hello!");

    sem_unlock(sem_ds);

    if(kill(pid_serv, SIGUSR1) < 0){
        perror("kill");
        exit(EXIT_FAILURE);
    }

    printf("Client change data\n");
    printf("Client text: %s\nClient pid: %d\n", server_data->text, server_data->pid);

    if(sem_close(sem_ds) < 0){
        perror("sem_close");
        exit(EXIT_FAILURE);
    }

    munmap(address, SHM_SIZE);
    return 0;

}