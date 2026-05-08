#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
// Реализовать программу, которая порождает процесс1 и процесс2, ждет
// завершения дочерних процессов. Процесс1 в свою очередь порождает
// процесс3 и процесс4 и ждет их завершения. Процесс2 порождает
// процесс5 и ждет его завершения. Все процессы выводят на экран свой
// pid, ppid.
void print_proc(const char* name){
    printf("-------------------\n");
    printf("%s: pid=%d, ppid=%d, gid=%d, sid=%d\n", name, getpid(), getppid(), getgid(), getsid(getpid()));
}

void wait_child(){
    pid_t pid;
    int status;
    while((pid = waitpid(-1, &status, 0)) > 0)
        printf("child %d finished with status %d\n", pid, WEXITSTATUS(status));
}

int main(){
    const char *names[] = {
        "process1",
        "process2",
        "process3",
        "process4",
        "process5"
    };
    pid_t pid;
    print_proc("parent");
    //parent создаёт process1 и process2
    for(int i = 0; i <= 1; i++){
        pid = fork();
    
        if (pid == -1){
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if(pid == 0){
            //процесс1
            if(i == 0){
                print_proc(names[0]);

                //процесс3 и процесс4
                for(int j = 0; j <= 1; j++){
                    pid_t pid = fork();

                    if (pid == -1){
                        perror("fork");
                        exit(EXIT_FAILURE);
                    }
                    if(pid == 0){
                        print_proc(names[j+2]);
                        exit(j+3);
                    }
                }
                wait_child();
                exit(1);
            }
            //процесс2
            if(i == 1){
                print_proc(names[1]);
                pid_t pid;

                //процесс5
                pid = fork();

                if (pid == -1){
                    perror("fork");
                    exit(EXIT_FAILURE);
                }
                if(pid == 0){
                    print_proc(names[4]);
                    exit(5);
                }
                wait_child();
                exit(2);
            }
        }
}
    wait_child();

    printf("\n");
        
    printf("All children finished\n");
    return 0;
}



// вариант 2

// #include <stdio.h>
// #include <stdlib.h>
// #include <sys/types.h>
// #include <sys/wait.h>
// #include <unistd.h>

// static void print_process(const char *name)
// {
//     printf("%s: pid=%d, ppid=%d\n", name, getpid(), getppid());
//     fflush(stdout);
// }

// static void wait_for_children(int count)
// {
//     for (int i = 0; i < count; ++i) {
//         wait(NULL);
//     }
// }

// int main(void)
// {
//     print_process("Parent");

//     pid_t process1 = fork();
//     if (process1 < 0) {
//         perror("fork");
//         return 1;
//     }

//     if (process1 == 0) {
//         print_process("Process1");

//         pid_t process3 = fork();
//         if (process3 < 0) {
//             perror("fork");
//             exit(1);
//         }

//         if (process3 == 0) {
//             print_process("Process3");
//             exit(0);
//         }

//         pid_t process4 = fork();
//         if (process4 < 0) {
//             perror("fork");
//             exit(1);
//         }

//         if (process4 == 0) {
//             print_process("Process4");
//             exit(0);
//         }

//         wait_for_children(2);
//         exit(0);
//     }

//     pid_t process2 = fork();
//     if (process2 < 0) {
//         perror("fork");
//         return 1;
//     }

//     if (process2 == 0) {
//         print_process("Process2");

//         pid_t process5 = fork();
//         if (process5 < 0) {
//             perror("fork");
//             exit(1);
//         }

//         if (process5 == 0) {
//             print_process("Process5");
//             exit(0);
//         }

//         wait_for_children(1);
//         exit(0);
//     }

//     wait_for_children(2);
//     return 0;
// }