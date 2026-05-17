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


int main(){

    int fd_sock, fd_new_sock;
    const char buff[] = "Hi";
    char serv_buff[BUFF_LEN];
    struct sockaddr_un serv, cl;
    socklen_t len_cl;
    socklen_t len_serv = sizeof(struct sockaddr_un);
    ssize_t bytes_from_cl;
    ssize_t bytes_from_serv;

    unlink(FILE);
    if((fd_sock = socket(AF_LOCAL, SOCK_STREAM, 0)) < 0){
        perror("socket");
        exit(EXIT_FAILURE);
    }

    serv.sun_family = AF_LOCAL;
    strcpy(serv.sun_path, FILE);

    if(bind(fd_sock, (struct sockaddr*)&serv, len_serv) < 0){
        perror("bind");
        exit(EXIT_FAILURE);
    }

    printf("Server running...\n");
    if(listen(fd_sock, 5) < 0){
        perror("listen");
        exit(EXIT_FAILURE);
    }
    while(1){
        printf("Server wait new client...\n");
        len_cl = sizeof(cl);
        if((fd_new_sock = accept(fd_sock, (struct sockaddr*)&cl, &len_cl)) < 0){
            perror("accept");
            continue;
        }

        printf("New client: %s\n", cl.sun_path);

        bytes_from_cl = recv(fd_new_sock, serv_buff, sizeof(serv_buff), 0);

        if(bytes_from_cl < 0){
            perror("recv");
            continue;
        }

        printf("Server recv message from client: %s\nRecv bytes: %ld\n", serv_buff, bytes_from_cl);

        bytes_from_serv = send(fd_new_sock, buff, sizeof(buff), 0);

        if(bytes_from_serv < 0){
            perror("send");
            continue;
        }

        printf("Server send message to client: %s\nSend bytes: %ld\n", buff, bytes_from_serv);


        close(fd_new_sock);
        printf("The connection with the client was finish\n\n");

    }

    close(fd_sock);
    unlink(FILE);
    return 0;
}