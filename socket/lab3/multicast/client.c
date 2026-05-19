#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>

#define PORT 7777
#define N 255
#define MULTICAST_IP "224.0.0.5"
const char* message_stop = "_STOP_";

int main(){

    int fd_sock;
    struct sockaddr_in cl, serv;
    ssize_t bytes_from_serv;
    socklen_t len_cl = sizeof(cl);
    socklen_t len_serv = sizeof(cl);
    char buffer[N];

    if((fd_sock = socket(AF_INET, SOCK_DGRAM, 0))< 0){
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&cl, 0, len_cl);
    memset(&serv, 0, len_serv);
    cl.sin_family = AF_INET;
    cl.sin_port = htons(PORT);
    cl.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(fd_sock, (struct sockaddr*)&cl, len_cl) < 0){
        perror("bind");
        exit(EXIT_FAILURE);
    }

    struct ip_mreqn mreq;
    mreq.imr_multiaddr.s_addr = inet_addr(MULTICAST_IP);
    mreq.imr_address.s_addr = htonl(INADDR_ANY);
    mreq.imr_ifindex = 0;

    if(setsockopt(fd_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0){
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i < N; i++){

        bytes_from_serv = recvfrom(fd_sock, buffer, N, 0, (struct sockaddr*)&serv, &len_serv);
        if(bytes_from_serv < 0){
            perror("recvfrom");
            break;
        }
        if(strcmp(buffer, message_stop) == 0){
            break;
        }

        printf("%d)Client recv message from server: %s\nRecv bytes: %ld\n", i, buffer, bytes_from_serv);
    }




    close(fd_sock);
    return 0;
}
