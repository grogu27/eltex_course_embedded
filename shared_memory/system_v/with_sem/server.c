#include "shm.h"
#include "sem.h"
#include <signal.h>

void sigusr1_handler(int, siginfo_t *, void *);
void change_sig_act();

int main(){
    change_sig_act();
    
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

    shm_ds = creat_shm(shm_key, SHM_SIZE);
    sem_ds = creat_sem(sem_key, 1);

    address = attach_shm(shm_ds, NULL, 0);

    server_data = (struct data*)address;
  
    sem_lock(sem_ds);

    server_data->pid = getpid();
    snprintf(server_data->text, sizeof(server_data->text) ,"%s", "Hi!");

    sem_unlock(sem_ds);
    printf("Server write data\n");
    printf("Server text: %s\nServer pid: %d\n", server_data->text, server_data->pid);

    pause(); // сервер ждет сигнала от клиента

    printf("Client change data\n");
    printf("Client text: %s\nClient pid: %d\n", server_data->text, server_data->pid);

    if(shmdt(address) < 0){
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    if(shmctl(shm_ds, IPC_RMID, NULL) < 0){
        perror("shmctl");
        exit(EXIT_FAILURE);
    }

    if(semctl(sem_ds, 0, IPC_RMID) < 0){
        perror("semctl");
        exit(EXIT_FAILURE);
    }

    return 0;
}



void sigusr1_handler(int signum, siginfo_t *info, void * context){
    printf("Пришел сигнал: %d\tот процесса(pid): %d\n", signum, info->si_pid);
}

        //    struct sigaction {
        //        void     (*sa_handler)(int);
        //        void     (*sa_sigaction)(int, siginfo_t *, void *);
        //        sigset_t   sa_mask;
        //        int        sa_flags;
        //        void     (*sa_restorer)(void);
        //    };

void change_sig_act(){
    struct sigaction sa;
    sigset_t set;
    if(sigemptyset(&set) < 0){
        perror("sigemptyset");
        exit(EXIT_FAILURE);
    }
    if(sigaddset(&set, SIGUSR1) < 0){
        perror("sigaddset");
        exit(EXIT_FAILURE);
    }
    sa.sa_sigaction = sigusr1_handler;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_mask = set;
    if(sigaction(SIGUSR1, &sa, NULL) < 0){
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

}

