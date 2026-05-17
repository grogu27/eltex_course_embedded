#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
//#include <netinet/in.h>
#include <unistd.h>
#include <sys/un.h>
#include <stdlib.h>
#define BUFF_LEN 255
#define FILE "tmp_file"
#define FILE2 "tmp_file2"

int main(){

    int fd_sock;
    const char buff[] = "Hello";
    char cl_buff[BUFF_LEN];
    struct sockaddr_un serv, cl;
    socklen_t len_cl = sizeof(struct sockaddr_un);;
    socklen_t len_serv = sizeof(struct sockaddr_un);
    ssize_t bytes_from_cl;
    ssize_t bytes_from_serv;
    unlink(FILE2);

    if((fd_sock = socket(AF_LOCAL, SOCK_STREAM, 0)) < 0){
        perror("socket");
        exit(EXIT_FAILURE);
    }

    serv.sun_family = AF_LOCAL;
    strcpy(serv.sun_path, FILE);

    cl.sun_family = AF_LOCAL;
    strcpy(cl.sun_path, FILE2);

    if(bind(fd_sock, (struct sockaddr*)&cl, len_cl) < 0){
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if(connect(fd_sock, (struct sockaddr*)&serv, len_serv) < 0){
        perror("connect");
        exit(EXIT_FAILURE);
    }

    bytes_from_cl = send(fd_sock, buff, sizeof(buff), 0);

    if(bytes_from_cl < 0){
        perror("send");
        exit(EXIT_FAILURE);
    }

    printf("Client send message to server: %s\nSend bytes: %ld\n", buff, bytes_from_cl);

    bytes_from_serv = recv(fd_sock, cl_buff, sizeof(cl_buff), 0);

    printf("Client recv message from server: %s\nRecv bytes: %ld\n", cl_buff, bytes_from_serv);

    unlink(FILE2);
    close(fd_sock);
    return 0;
}