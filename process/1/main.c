#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>


int main(){

    pid_t pid;
    int status;
 
    pid = fork();
    
    if (pid == -1){
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0){
        printf("child PID = %d\nchild PPID = %d\nchild GID = %d\nchild SID = %d\n", getpid(), getppid(), getgid(), getsid(getpid()));
        //execl("/bin/dir", "dir", NULL);
        //perror("exec failed"); // выполнится только при ошибке
        //exit(1);
        exit(EXIT_SUCCESS);
        
    }
    else{
        printf("parent PID = %d\nparent PPID = %d\nparent GID = %d\nparent SID = %d\n", getpid(), getppid(), getgid(), getsid(getpid()));
        wait(&status);
        printf("status = %d\n", WEXITSTATUS(status));

    }
    
    exit(EXIT_SUCCESS);
}