#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>  
#include<fcntl.h>
#include<sys/stat.h>

int main(){
    // if(mkfifo("pipe", 0600) == -1){
    //     perror("mkfifo");
    //     exit(1);
    // }

    int fd = open("pipe2", O_RDONLY);
    if(fd == -1){
        perror("open");
        exit(1);
    }

    char buf[255];

    int n;
    while((n = read(fd, buf, sizeof(buf))) > 0){// пока есть данные в канале
        write(STDOUT_FILENO, buf, n);
    }
    if(n == -1){
        perror("read");
        exit(1);
    }

    close(fd);
    unlink("pipe2");
    return 0;
}