#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>

#define IP_CL "255.255.255.255"
#define PORT_CL 7777
#define N 10
const char* message = "Hello from server!!!";
const char* message_stop = "_STOP_";

int main(){

    int fd_sock;
    struct sockaddr_in cl;
    ssize_t bytes_from_serv;

    if((fd_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        perror("socket");
        exit(EXIT_FAILURE);
    }

    int flag = 1;
    if(setsockopt(fd_sock, SOL_SOCKET, SO_BROADCAST, &flag, sizeof(flag)) < 0){
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    int check;
    socklen_t len = sizeof(check);
    if(getsockopt(fd_sock, SOL_SOCKET, SO_BROADCAST, &check, &len) < 0){
        perror("getsockopt");
        exit(EXIT_FAILURE);
    }
    printf("Broadcast is working?: %d\n", check);

    memset(&cl, 0, sizeof(cl));
    cl.sin_family = AF_INET;
    cl.sin_port = htons(PORT_CL);
    if(inet_pton(AF_INET, IP_CL, &cl.sin_addr) != 1){
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }

    printf("Server running...\n");
    for(int i = 0; i < N; i++){
        bytes_from_serv = sendto(fd_sock, message, strlen(message)+1, 0, (struct sockaddr*)&cl, sizeof(cl));
        if(bytes_from_serv < 0){
            perror("sendto");
            continue;
        }
        printf("%d) Server send broadcast message: %s\nSend bytes: %ld\n", i+1, message, bytes_from_serv);
    }
    sendto(fd_sock, message_stop, strlen(message_stop)+1, 0, (struct sockaddr*)&cl, sizeof(cl));
    if(bytes_from_serv < 0){
        perror("sendto");
    }

    close(fd_sock);
    return 0;
}