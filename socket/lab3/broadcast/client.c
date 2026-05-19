#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#define PORT_CL 7777
#define IP "255.255.255.255"
#define N 255

const char* message_stop = "_STOP_";

int main(){

    int fd_sock;
    struct sockaddr_in cl, serv;
    ssize_t bytes_from_serv;
    socklen_t len_serv = sizeof(serv);
    char recv_buff[N];

    if((fd_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // struct timeval tv = {0};
    // tv.tv_sec = 5;

    // if(setsockopt(fd_sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0){
    //     perror("setsockopt");
    //     exit(EXIT_FAILURE);
    // }
    

    memset(&serv, 0, sizeof(struct sockaddr_in));
    memset(&cl, 0, sizeof(struct sockaddr_in));
    cl.sin_family = AF_INET;
    cl.sin_port = htons(PORT_CL);

    if(inet_pton(AF_INET, IP, &cl.sin_addr) != 1){
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }

    if(bind(fd_sock, (struct sockaddr*)&cl, sizeof(cl)) < 0){
        perror("bind");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in tmp_cl;
    socklen_t len_tmp_cl = sizeof(tmp_cl);
    getsockname(fd_sock, (struct sockaddr*)&tmp_cl, &len_tmp_cl);

    char buff_cl_in[INET_ADDRSTRLEN];
    if(inet_ntop(AF_INET, &tmp_cl.sin_addr, buff_cl_in, INET_ADDRSTRLEN) == NULL){
        perror("inet_ntop");
        exit(EXIT_FAILURE);
    }
    printf("Client have endpoint: %s:%d\n", buff_cl_in, ntohs(tmp_cl.sin_port));

    int i = 1;
    while(1){
        memset(recv_buff, 0, sizeof(recv_buff));
        bytes_from_serv = recvfrom(fd_sock, recv_buff, sizeof(recv_buff), 0, (struct sockaddr*)&serv, &len_serv);
        
        if(bytes_from_serv < 0){
            perror("recvfrom");
            break;
        }
        if(strcmp(recv_buff, message_stop) == 0){
            break;
        }

        printf("%d)Client recv message from server: %s\nRecv bytes: %ld\n", i, recv_buff, bytes_from_serv);
        i++;
    }

    close(fd_sock);
    return 0;
}