#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <time.h>

#define SERV_PORT 7777
#define SERV_IP "127.0.0.1"

void sigchld_hadler(int, siginfo_t *, void *);
int change_sig_act(struct sigaction, sigset_t);
void client_handler(int, struct sockaddr_in);

int main(){

    pid_t pid;
    int sock_cl, sock_serv;
    struct sockaddr_in serv, client;
    socklen_t size_serv = sizeof(serv);
    socklen_t size_client = sizeof(client);

    struct sigaction  sa_sigchild;
    sigset_t set;
    int res_sig = change_sig_act(sa_sigchild, set);
    if(res_sig == -1){
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    memset(&serv, 0, size_serv);
    memset(&client, 0, size_client);

    if((sock_serv = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("socket");
        exit(EXIT_FAILURE);
    }

    serv.sin_family = AF_INET;
    serv.sin_port = htons(SERV_PORT);
    if(inet_pton(AF_INET, SERV_IP, &serv.sin_addr) != 1){
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }

    if(bind(sock_serv, (struct sockaddr*)&serv, size_serv) < 0){
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if(listen(sock_serv, 5) < 0){
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while(1){
        memset(&client, 0, size_client);
        if((sock_cl = accept(sock_serv, (struct sockaddr*)&client, &size_client)) < 0){
            if(errno == EINTR){ //Ошибка: прерванный системный вызов
                continue; // Когда приходит сигнал от дочеренго процесса, то процесс может прерваться. Надо игнорировать
            }
            perror("accept");
            close(sock_serv);
            exit(EXIT_FAILURE);
        }
        printf("СЕРВЕР: Подключен клиент %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

        pid = fork();
        if(pid == 0){
            close(sock_serv);
            client_handler(sock_cl, client);
        }
        else if(pid > 0){
            close(sock_cl);
        }
        else{
            perror("fork");
            close(sock_cl);
            continue;
        }

        
    }




    return 0;
}

void client_handler(int sock_cl, struct sockaddr_in client){

    ssize_t bytes_from_cl;
    ssize_t bytes_from_serv;
    char buff_cl[255];
    char message[255];
    //char *message = NULL;
    //int len_buff_cl;
    int count = 1;
    time_t timer;

    while(1){
        memset(buff_cl, 0, sizeof(buff_cl));
        memset(message, 0, sizeof(message));
        bytes_from_cl = recv(sock_cl, buff_cl, sizeof(buff_cl), 0);
        if(bytes_from_cl < 0){
            perror("recv");
            exit(EXIT_FAILURE);
        }
        if(bytes_from_cl == 0){
            printf("СЕРВЕР: Клиент перестал отправлять данные\n");
            break;
        }
        printf("СЕРВЕР: Cервер получил от клиента[%s:%d] сообщение №%d: %s\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port), count++, buff_cl);
        
        // len_buff_cl = strlen(buff_cl);
        // snprintf(message, sizeof(message), "%s",  buff_cl);
        // message[len_buff_cl] = '5';
        // message[len_buff_cl+1] = '\0';
        timer = time(NULL);
       // message = (char*)timer;
        struct tm *tm_info = localtime(&timer);
        printf("Время: %ld\n", timer);
        strftime(message, 255, "%Y-%m-%d %H:%M:%S", tm_info);
        printf("message: %s\n", message);
        printf("Hour: %d, Min: %d, Sec: %d\nDay: %d, Month: %d, Year: %d\n", tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec, tm_info->tm_mday, tm_info->tm_mon, tm_info->tm_year);
        bytes_from_serv = send(sock_cl, message, strlen(message)+1, 0);
        if(bytes_from_serv < 0){
            perror("send");
            exit(EXIT_FAILURE);
        }

    }

    close(sock_cl);
    exit(EXIT_SUCCESS);
}

int change_sig_act(struct sigaction sa_sigchild, sigset_t set){
    memset(&sa_sigchild, 0, sizeof(sa_sigchild));
    sigemptyset(&set);
    sigaddset(&set, SIGCHLD);
    sa_sigchild.sa_sigaction = sigchld_hadler;
    sa_sigchild.sa_mask = set;
    sa_sigchild.sa_flags = SA_SIGINFO;
    int res_sigchld = sigaction(SIGCHLD, &sa_sigchild, NULL);
    if(res_sigchld < 0){
        return -1;
    }
    return 0;
}


void sigchld_hadler(int sig, siginfo_t *info, void *context){
    int status;
    pid_t pid;
    while((pid = waitpid(-1, &status, WNOHANG)) > 0){
        if(WIFEXITED(status)){
            printf("Завершился процесс: %d с кодом: %d\n", pid, WEXITSTATUS(status));
        }
        
    }
}
