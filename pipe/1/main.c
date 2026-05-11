#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<string.h>
void read_pipe(int fd){

    char buf[255];
    int n = read(fd, buf, sizeof(buf));
    buf[n] = '\n';
    write(1, buf, n + 1);
    close(fd);  
}

void write_pipe(int fd, const char* message){
    write(fd, message,  strlen(message) + 1);
    close(fd);
}

int main(){

    int fd[2];
    pid_t pid;

    if(pipe(fd) == -1){
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if(pid == -1){
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if(pid == 0){
        close(fd[1]);
        read_pipe(fd[0]);
        exit(EXIT_SUCCESS);
    }
    else{
        close(fd[0]);
        const char *message = "Hi!";
        write_pipe(fd[1], message);
        int status;
        wait(&status);
        printf("Exit status: %d\n", WEXITSTATUS(status));

    }
    exit(EXIT_SUCCESS);

}