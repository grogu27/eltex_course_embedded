#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>


#define PORT 7777
#define SIZE 255
#define MULTICAST_IP "224.0.0.5"
#define N 10
const char* message_stop = "_STOP_";
const char* message = "Hello from server!!!";

int main(){

    int fd_sock;
    struct sockaddr_in cl;
    ssize_t bytes_from_serv;
    socklen_t len_cl = sizeof(cl);

    if((fd_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&cl, 0, sizeof(cl));
    cl.sin_family = AF_INET;
    cl.sin_port = htons(PORT);
    cl.sin_addr.s_addr = inet_addr(MULTICAST_IP);

    printf("Server running...\n");
    for(int i = 0; i < N; i++){
        bytes_from_serv = sendto(fd_sock, message, strlen(message)+1, 0, (struct sockaddr*)&cl, len_cl);
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