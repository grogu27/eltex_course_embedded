#include "shm.h"
#include "sem.h"
#include <signal.h>

int main(){
    
    printf("PID: %d\n", getpid());
    int shm_ds;
    int sem_ds;
    key_t shm_key;
    key_t sem_key;
    void *address = NULL;
    struct data *server_data = NULL;
    struct data *client_data = NULL;

    shm_key = gen_key(SHM_NAME, 1);
    sem_key = gen_key(SEM_NAME, 2);

    shm_ds = open_shm(shm_key, SHM_SIZE);
    sem_ds = open_sem(sem_key, 1);

    address = attach_shm(shm_ds, NULL, 0);

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

    if(shmdt(address) < 0){
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    return 0;

}