#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<signal.h>
#include <stdlib.h>
#include <string.h>
void handler_sigusr1(int signal, siginfo_t* info, void* args){
    printf("Получен сигнал:%d\tОт процесса:%d\n", signal, info->si_pid);
    pause(); // переход в спящий режим до следующего сигнала. не нагружает цп
    //while(1); // нагружает цп
}

void handler_sigcont(int signal, siginfo_t* info, void* args){
    printf("Получен сигнал:%d\tОт процесса:%d\nВозобновление процесса...\n", signal, info->si_pid);
}

int main(){
    struct sigaction sa, sa2;
    memset(&sa, 0, sizeof(sa));
    memset(&sa2, 0, sizeof(sa2));
    sigset_t set;
    int ret, ret2;
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    sigaddset(&set, SIGCONT);
    sa.sa_sigaction = handler_sigusr1;
    sa2.sa_sigaction = handler_sigcont;
    sa.sa_flags = SA_SIGINFO;
    sa2.sa_flags = SA_SIGINFO;
    ret = sigaction(SIGUSR1, &sa, NULL);
    ret2 = sigaction(SIGCONT, &sa2, NULL);
    if (ret < 0 || ret2 < 0)
    {
        perror("Ошибка переопределения сигнала\n");
        exit(0);
    }
    
    int count = 1;
    while (1)
    {
        printf("%d)Ожидание сигнала...\n", count++);
        
        sleep(1);
    }
    
    return 0;
}