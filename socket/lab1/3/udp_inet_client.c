#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#define BUFF_LEN 255
#define IP "127.0.0.1"
#define PORT 7777


int main(){

    int fd_sock;
    struct sockaddr_in serv;
    const char buff[] = "Hello";
    char cl_buff[BUFF_LEN];
    ssize_t bytes_from_cl;
    ssize_t bytes_from_serv;
    socklen_t len_serv = sizeof(struct sockaddr_in);

    if((fd_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&serv, 0, len_serv);
    serv.sin_family = AF_INET;
    serv.sin_port = htons(PORT);

    if(inet_pton(AF_INET, IP, &serv.sin_addr) != 1){
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }

    if(connect(fd_sock, (struct sockaddr*)&serv, len_serv) < 0){  //Клиент получил свои эндпоинты от ОС и сокет получил эндпоинты сервера
        perror("connect");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in tmp_serv;
    socklen_t len_tmp_serv = sizeof(struct sockaddr_in);
    if(getpeername(fd_sock, (struct sockaddr*)&tmp_serv, &len_tmp_serv) < 0){ // Благодаря connect(), клиент может узнать к какому серверу он подключился. Хотя udp не устанавливает соединение
        perror("getpeername");
        exit(EXIT_FAILURE);
    }

    char buff_in[INET_ADDRSTRLEN];
    if(inet_ntop(AF_INET, &tmp_serv.sin_addr, buff_in, sizeof(buff_in)) == NULL){
        perror("inet_ntop");
        exit(1);
    }
    printf("Client connected %s:%d\n", buff_in, ntohs(tmp_serv.sin_port));

    bytes_from_cl = send(fd_sock, buff, sizeof(buff), 0);

    if(bytes_from_cl < 0){
        perror("send");
    }

    printf("Client send message: %s\nSend bytes: %ld: \nSend to: %s:%d\n", buff, bytes_from_cl, buff_in, ntohs(serv.sin_port));

    bytes_from_serv = recv(fd_sock, cl_buff, sizeof(cl_buff), 0);

    if(bytes_from_serv < 0){
        perror("recv");
    }

    printf("Client recv message: %s\nRecv bytes: %ld: \nRecv from: %s:%d\n", cl_buff, bytes_from_serv, buff_in, ntohs(serv.sin_port));

    

    close(fd_sock);
    return 0;
}