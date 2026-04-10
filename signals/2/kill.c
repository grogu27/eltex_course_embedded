#include<signal.h>
#include<stdlib.h>
#include<stdio.h>
int main(int argc, char** argv){
    int pid;

    if (argc > 1) {
        pid = atoi(argv[1]);
    } 
    else {
        printf("Введите pid:");
        if(scanf("%d", &pid) != 1){
            fprintf(stderr, "Ошибка ввода scanf()!\n");
            exit(0);
        }
    }

    if(kill(pid, SIGINT) != 0){
        fprintf(stderr, "Ошибка отправки сигнала!\n");
        exit(0);
    }
    printf("Сигнал SIGINT отправлен процессу %d\n", pid);
    return 0;
}