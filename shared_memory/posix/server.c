#include <sys/mman.h>
#include "shm.h"
#include "sem.h"
#include <signal.h>

void sigusr1_handler(int, siginfo_t *, void *);
void change_sig_act();
    //int shm_open(const char *name, int oflag, mode_t mode);
    //int ftruncate(int fd, off_t length);
    //int shm_unlink(const char *name);
    //void *mmap(void addr[.length], size_t length, int prot, int flags, int fd, off_t offset);
    //int munmap(void addr[.length], size_t length);

int main(){
    change_sig_act();
    shm_unlink(SHM_NAME);
    printf("PID: %d\n", getpid());
    sem_t *semid;
    int shm_ds;
    void *address = NULL;

    shm_ds = creat_shm(SHM_NAME, SHM_SIZE);
    semid = creat_sem(SEM_NAME);

    struct data message_serv = {.pid = getpid(), .text = "Hi!"};
    struct data *ptr_message_serv = NULL;

    address = attach_shm(shm_ds);
    ptr_message_serv = (struct data *)address;
    ptr_message_serv->pid = message_serv.pid;
    strcpy(ptr_message_serv->text, message_serv.text);
    printf("Сервер пишет в разделяемую память\n");
    sem_lock(semid);
    sem_unlock(semid);
    printf("Сервер записал: pid = %d\ttext = %s\n", ptr_message_serv->pid, ptr_message_serv->text);

    pause();

    printf("Сервер проверяет изменение: pid = %d\ttext = %s\n", ptr_message_serv->pid, ptr_message_serv->text);

    if(sem_close(semid) < 0){
        perror("sem_close");
        exit(EXIT_FAILURE);
    }

    munmap(address, SHM_SIZE);
    sem_unlink(SEM_NAME);
    shm_unlink(SHM_NAME);

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