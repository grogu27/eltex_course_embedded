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

    char buf[MAX];
    pid_t pid;
    char cwd[PATH_MAX];
    while (1){
        getcwd(cwd, sizeof(cwd));
        printf("mini_bash:~%s$ ", cwd);
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
    


    return 0;
}