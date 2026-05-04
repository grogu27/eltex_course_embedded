#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>


int main(){

    pid_t pid;
    int status;

    printf("Hello!!");
    //puts("Hello2");
    printf("Hello!!\n");
    sleep(5);

    return 0;
}