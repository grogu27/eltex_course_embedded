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
    struct sockaddr_in serv, cl;
    const char buff[] = "Hi";
    char serv_buff[BUFF_LEN];
    ssize_t bytes_from_cl;
    ssize_t bytes_from_serv;
    socklen_t len_serv = sizeof(struct sockaddr_in);
    socklen_t len_cl = sizeof(struct sockaddr_in);

    if((fd_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&serv, 0, len_serv);
    memset(&cl, 0, len_cl);
    serv.sin_family = AF_INET;
    serv.sin_port = htons(PORT);

    if(inet_pton(AF_INET, IP, &serv.sin_addr) != 1){
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }

    if(bind(fd_sock, (struct sockaddr*)&serv, len_serv) < 0){
        perror("bind");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in tmp_serv;
    socklen_t len_tmp_serv = sizeof(struct sockaddr_in);
    getsockname(fd_sock, (struct sockaddr*)&tmp_serv, &len_tmp_serv);
    char buff_in[INET_ADDRSTRLEN];
    if(inet_ntop(AF_INET, &tmp_serv.sin_addr, buff_in, sizeof(buff_in)) == NULL){
        perror("inet_ntop");
        exit(EXIT_FAILURE);
    }
    printf("Server have endpoint: %s:%d\n", buff_in, ntohs(tmp_serv.sin_port));

    while(1){
        printf("Server running...\n");
        len_cl = sizeof(cl);
        bytes_from_cl = recvfrom(fd_sock, serv_buff, sizeof(serv_buff), 0, (struct sockaddr*)&cl, &len_cl);

        if(bytes_from_cl < 0){
            perror("recvfrom");
            continue;
        }
        serv_buff[bytes_from_cl+1] = '\0'; // на всякий случай

        char buff_in[INET_ADDRSTRLEN];
        if(inet_ntop(AF_INET, &cl.sin_addr, buff_in, sizeof(buff_in)) == NULL){
            perror("inet_ntop");
            exit(EXIT_FAILURE);
        }
        printf("Server recv message: %s\nRecv bytes: %ld: \nRecv from: %s:%d\n", serv_buff, bytes_from_cl, buff_in, ntohs(cl.sin_port));

        bytes_from_serv = sendto(fd_sock, buff, sizeof(buff), 0, (struct sockaddr*)&cl, len_cl);

        
        if(bytes_from_serv < 0){
            perror("sendto");
            continue;
        }

        printf("Server send message: %s\nSend bytes: %ld: \nSend to: %s:%d\n\n", buff, bytes_from_serv, buff_in, ntohs(cl.sin_port));


    }




    close(fd_sock);

    return 0;
}