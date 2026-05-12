// Реализовать аналог командного интерпретатора bash. При запуске
// программы пользователю предлагается ввести имя программы и опции
// запуска программы. Программа порождает процесс и в нем выполняет
// введенную программу с заданными опциями, ждет завершения
// дочернего процесса. Снова возвращается к вводу следующей
// программы. Выход из интерпретатора по команде exit.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#define MAX 255
#define ARGS 64
#define PATH_MAX 255

int main(){

    int flag_pipe = 0;
    char buf[MAX];
    pid_t pid;
    char cwd[PATH_MAX];
    char hostname[256];
    char *user_name;
    if(gethostname(hostname, sizeof(hostname)) == -1){ 
        sprintf(hostname, "%s", "localhost");
    } 

    if((user_name = getenv("USER")) == NULL){
        user_name = "user";
    }  
    //printf("mini_bash:\n");
    while (1){
        getcwd(cwd, sizeof(cwd));

        //printf("mini_bash:~%s$ ", cwd);
        
        printf("%s@%s:~%s$ ", user_name, hostname, cwd);
        fflush(stdout);
        
        if(fgets(buf, sizeof(buf), stdin) == NULL){
            printf("Ошибка ввода\n");
            continue;
        }

        for(int i = 0; buf[i] != '\0'; i++){
            if(buf[i] == '\n')
                buf[i] = '\0';
        }
        //printf("%s\n", buf);

        if(strcmp("exit", buf) == 0){
            printf("Завершение программы\n");
            exit(0);
        }

        char* pipe_pos = strchr(buf, '|');

        //канала нет
        if(pipe_pos == NULL){

            char *args[ARGS];
            int i = 0;
            char *token = strtok(buf, " ");
            for (; token != NULL; i++){
                args[i] = token;
                token = strtok(NULL, " ");
            }
            args[i] = NULL;

            if(strcmp("cd", buf) == 0){
                chdir(args[1]);
                continue;
            }

            pid_t pid = fork();

            if(pid == -1){
                perror("fork");
                continue;
            }
            if(pid == 0){
                execvp(args[0], args);    // Сначала ищет в PATH
                perror("exec failed"); // если exec не сработал
                exit(1);
            }
            else{
                wait(NULL);
                continue;
            }
            
        }
        //канал есть
        else{
            char* letf_args[ARGS];
            char* right_args[ARGS];
            char* left = buf;
            *pipe_pos = '\0'; // теперь в буфере две строки
            char* right = pipe_pos + 1;
            char* token;
            pid_t left_pid;
            pid_t right_pid;
            
            int i = 0;
            token = strtok(left, " ");
            for(; token != NULL && i < ARGS - 1; i++){
                letf_args[i] = token;
                token = strtok(NULL, " ");
            }
            letf_args[i] = NULL;

            // for(int i = 0; letf_args[i] != NULL; i++){
            //     printf("%s\n", letf_args[i]);
            // }

            int j = 0;
            token = strtok(right, " ");
            for(; token != NULL && j < ARGS - 1; j++){
                right_args[j] = token;
                token = strtok(NULL, " ");
            }
            right_args[j] = NULL;

            // for(int i = 0; right_args[i] != NULL; i++){
            //     printf("%s\n", right_args[i]);
            // }
            int fd[2];

            if(pipe(fd) == 1){
                perror("pipe");
                exit(1);
            }
            //создание первого процесса
            left_pid = fork();

            if(pid == -1){
                perror("fork");
                exit(1);
            }

            if(left_pid == 0){
                close(fd[0]); // закрытие на чтение
                if(dup2(fd[1], STDOUT_FILENO) == -1){
                    perror("dup2");
                    exit(1);
                }
                execvp(letf_args[0], letf_args);
                perror("execvp");
                exit(1);
            }
            //создание второго процесса
            right_pid = fork();

            if(right_pid == -1){
                perror("fork");
                exit(1);
            }
            
            if(right_pid == 0){
                close(fd[1]); // закрытие на запись
                if(dup2(fd[0], STDIN_FILENO) == -1){
                    perror("dup2");
                    exit(1);
                }
                execvp(right_args[0], right_args);
                perror("execvp");
                exit(1);
            }

            //родитель
            close(fd[0]);
            close(fd[1]);
            // waitpid(left_pid, NULL, 0);
            // waitpid(right_pid, NULL, 0);
            while(waitpid(-1, NULL, 0) > 0){}
            continue;
            
        }
        
    }
    


    return 0;
}