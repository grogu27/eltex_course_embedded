#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>  
#include<fcntl.h>
#include<sys/stat.h>

int main(){
    unlink("pipe2");
    if(mkfifo("pipe2", 0600) == -1){
        perror("mkfifo");
        exit(1);
    }
    
    int fd = open("pipe2", O_WRONLY | O_CREAT);
    if(fd == -1){
        perror("open");
        exit(1);
    }
    printf("Pipe is created\n");
    const char* message = "Hi!";
    int len_mes = 0 ;
    while (message[len_mes] != '\0')
    {
        len_mes++;
    }
    
    int n = write(fd, message, len_mes + 1);
    if(n == -1){
        perror("write");
        exit(1);
    }
    printf("Server send %d bytes\n", n);
    close(fd);
    return 0;
}