#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<signal.h>
#include <stdlib.h>


int main(){
    sigset_t set;
    int ret, sig_num;
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    sigaddset(&set, SIGUSR2);
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGTERM);
    ret = sigprocmask(SIG_BLOCK, &set, NULL);

    if (ret < 0)
    {
        perror("Ошибка брокировки сигнала\n");
        exit(0);
    }
    
    while (1)
    {
        sigwait(&set, &sig_num);
        printf("Номер сигнала: %d\n", sig_num);
        if (sig_num == SIGTERM){
            printf("Получен SIGTERM, выходим\n");
            exit(0);
        }
        else if (sig_num == SIGINT){
            printf("Получен SIGINT, продолжаем\n");
        }
        
        
    }
    
    return 0;
}