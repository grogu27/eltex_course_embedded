#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<signal.h>
#include <stdlib.h>



int main(){
    
    sigset_t set;
    
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    int ret = sigprocmask(SIG_BLOCK, &set, NULL);
    if (ret < 0)
    {
        perror("Ошибка брокировки сигнала\n");
        exit(0);
    }
    //signal(SIGINT, SIG_IGN);
  
    int count = 1;
    while (1)
    {
        printf("%d)Бесконечный цикл...\n", count++);
        
        sleep(1);
    }
    
    return 0;
}