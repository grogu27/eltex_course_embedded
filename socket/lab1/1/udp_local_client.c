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
    struct sockaddr_un serv;
    struct sockaddr_un cl;
    const char buff[] = "Hello";
    char serv_buf[BUFF_LEN];
    unlink(FILE2);

    if((fd_sock = socket(AF_LOCAL, SOCK_DGRAM, 0)) < 0){
        perror("socket");
        exit(1);
    }
    memset(&cl, 0, sizeof(cl));
    memset(&serv, 0, sizeof(serv));
    serv.sun_family = AF_LOCAL;
    strcpy(serv.sun_path, FILE);

    cl.sun_family = AF_LOCAL;
    strcpy(cl.sun_path, FILE2);
    if(bind(fd_sock, (struct sockaddr*)&cl, sizeof(struct sockaddr_un)) < 0){
        perror("bind");
        exit(1);
    }

    // if(connect(fd_sock, (struct sockaddr*)&serv, sizeof(struct sockaddr_un)) < 0){
    //     perror("connect");
    //     exit(1);
    // }

    ssize_t bytes_from_cl = sendto(fd_sock, buff, sizeof(buff), 0, (struct sockaddr*)&serv, sizeof(struct sockaddr_un));
    //ssize_t bytes_from_cl = send(fd_sock, buff, sizeof(buff), 0);

    if(bytes_from_cl < 0){
        perror("sendto");
    }

    printf("Client send message: %s\nSend bytes: %ld: \nSend to: %s\n", buff, bytes_from_cl, serv.sun_path);
    socklen_t client_len = sizeof(struct sockaddr_un);
    ssize_t bytes_from_serv = recvfrom(fd_sock, serv_buf, sizeof(serv_buf), 0, (struct sockaddr*)&serv, &client_len);
    //ssize_t bytes_from_serv = recv(fd_sock, serv_buf, sizeof(serv_buf), 0);

    if(bytes_from_serv < 0){
        perror("recvfrom");
    }

    printf("Client recv message: %s\nRecv bytes: %ld: \nRecv from: %s\n", serv_buf, bytes_from_serv, serv.sun_path);

    unlink(FILE2);
    close(fd_sock);
    return 0;
}