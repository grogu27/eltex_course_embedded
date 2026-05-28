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

#define SERV_PORT 7777
#define SERV_IP "127.0.0.1"



int main(){

    int sock;
    struct sockaddr_in serv;
    socklen_t size_serv = sizeof(serv);
    char message[255] = "hello";
    char message_from_serv[255];
    ssize_t bytes_from_serv;
    ssize_t bytes_from_cl;

    if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&serv, 0, size_serv);
    serv.sin_family = AF_INET;
    serv.sin_port = htons(SERV_PORT);
    serv.sin_addr.s_addr = inet_addr(SERV_IP);


    if(connect(sock, (struct sockaddr*)&serv, size_serv) < 0){
        perror("connect");
        exit(EXIT_FAILURE);
    }

    
        memset(message_from_serv, 0, sizeof(message_from_serv));

        bytes_from_cl = send(sock, message, strlen(message)+1, 0);
        if(bytes_from_cl < 0){
                perror("send");
                exit(EXIT_FAILURE);
        }        
        printf("КЛИЕНТ: Клиент отправил сообщение серверу: %s\n", message);
        
        while(1){
            bytes_from_serv = recv(sock, message_from_serv, 1, 0);
            printf("КЛИЕНТ: Клиент получил сообщение %s %ld байт от сервера\n", message_from_serv, bytes_from_serv);
            if(bytes_from_serv < 0){
                perror("recv");
                exit(EXIT_FAILURE);
            }
            if(bytes_from_serv == 0){ // Истина, если сервер закрыл сокет.
                printf("КЛИЕНТ: Клиент больше не получает данные\n");
                break;
            }

        }
        

    



}