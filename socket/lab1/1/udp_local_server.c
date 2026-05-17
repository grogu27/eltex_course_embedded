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

    int fd_sock;
    struct sockaddr_un serv, cl;
    const char buff[] = "Hi";
    char cl_buff[BUFF_LEN];
    ssize_t bytes_from_cl;
    ssize_t bytes_from_serv;
    socklen_t client_len;
    unlink(FILE);

    if((fd_sock = socket(AF_LOCAL, SOCK_DGRAM, 0)) < 0){
        perror("sock");
        exit(1);
    }

    memset(&serv, 0, sizeof(struct sockaddr_un));

    serv.sun_family = AF_LOCAL;
    strcpy(serv.sun_path, FILE);

    if(bind(fd_sock, (struct sockaddr*)&serv, sizeof(struct sockaddr_un)) < 0){
        perror("bind");
        close(fd_sock);
        exit(1);
    }

    while(1){
        printf("Server running...\n");
        memset(&cl, 0, sizeof(cl));
        memset(cl_buff, 0, sizeof(cl_buff));
        client_len = sizeof(struct sockaddr_un);
        bytes_from_cl = recvfrom(fd_sock, cl_buff, sizeof(cl_buff), 0, (struct sockaddr*)&cl, &client_len);
        if(bytes_from_cl < 0){
            perror("recvfrom");
            continue;
        }

        printf("Server recv message: %s\nRecv bytes: %ld: \nRecv from: %s\n", cl_buff, bytes_from_cl, cl.sun_path);


        bytes_from_serv = sendto(fd_sock, buff, sizeof(buff), 0, (struct sockaddr*)&cl, client_len);
        if(bytes_from_serv < 0){
            perror("sendto");
            continue;
        }

        printf("Server send message: %s\nSend bytes: %ld: \nSend to: %s\n", buff, bytes_from_serv, cl.sun_path);

    }

    
    unlink(FILE);
    return 0;
}

