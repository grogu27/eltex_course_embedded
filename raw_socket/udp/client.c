#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/udp.h>
#include <netinet/ip.h>

#define PORT_SERV 7777
#define PORT_CL 8888
#define IP_SERV "127.0.0.1"
#define IP_CL "127.0.0.1"
#define N 255

                //   0      7 8     15 16    23 24    31                         
                //  +--------+--------+--------+--------+             
                //  |     Source      |   Destination   |
                //  |      Port       |      Port       |
                //  +--------+--------+--------+--------+
                //  |                 |                 |
                //  |     Length      |    Checksum     |
                //  +--------+--------+--------+--------+

// struct udphdr
// {
//   __extension__ union
//   {
//     struct
//     {
//       uint16_t uh_sport;	/* source port */
//       uint16_t uh_dport;	/* destination port */
//       uint16_t uh_ulen;		/* udp length */
//       uint16_t uh_sum;		/* udp checksum */
//     };
//     struct
//     {
//       uint16_t source;
//       uint16_t dest;
//       uint16_t len;
//       uint16_t check;
//     };
//   };
// };

int main(){

    int sock;
    struct sockaddr_in serv;
    struct udphdr udp;
    struct udphdr *udp_tmp = NULL;
    struct iphdr *ip_header = NULL;
    const char *message = "Hello server123";
    char buff_for_serv[N];
    char *buff_for_cl;
    ssize_t send_bytes, recv_bytes;
    socklen_t len_serv = sizeof(struct sockaddr_in);
    size_t len_message = strlen(message);

    if((sock = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) < 0){
        perror("socket");
        exit(EXIT_FAILURE);
    }

    serv.sin_family = AF_INET;
    serv.sin_port = htons(PORT_SERV);
    serv.sin_addr.s_addr = inet_addr(IP_SERV);


    udp.dest = htons(PORT_SERV);
    udp.source = htons(PORT_CL);
    udp.check = 0;
    udp.len = htons(sizeof(struct udphdr) + len_message + 1);

    printf("len: %u\ndest: %u\nsrc: %u\ncheck: %u\n", ntohs(udp.len), ntohs(udp.dest), ntohs(udp.source), ntohs(udp.check));

    buff_for_cl = malloc(udp.len);
    if(buff_for_cl == NULL){
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    memcpy(buff_for_cl, &udp, sizeof(struct udphdr));
    memcpy(buff_for_cl + sizeof(struct udphdr), message, len_message+1);

    // выводит в литл индиян, так как x86_64
    printf("Full datagram (%d bytes):\n", ntohs(udp.len));  
    for(int i = 0; i < ntohs(udp.len); i++){
        printf("%02X ", (unsigned char)buff_for_cl[i]);
        if((i + 1) % 16 == 0) printf("\n");
    }
    printf("\n");

    
    send_bytes = sendto(sock, buff_for_cl, ntohs(udp.len), 0, (struct sockaddr*)&serv, len_serv);
    if(send_bytes < 0){
        perror("sendto");
        exit(EXIT_FAILURE);
    }
    printf("Клиента отправил серверу udp-датаграмму размером %ld байт\nПолезная нагрузка: %s\n", send_bytes, buff_for_cl + sizeof(struct udphdr));

    int count = 1;
    while(1){
        printf("%d)Попытка найти нужны пакет\n", count++);
        memset(buff_for_serv, 0, N);
        recv_bytes = recvfrom(sock, buff_for_serv, N, 0, (struct sockaddr*)&serv, &len_serv);
        if(recv_bytes < 0){
            perror("recvfrom");
            exit(EXIT_FAILURE);
        }
        ip_header = (struct iphdr*)buff_for_serv; // сначала идёт заголовок ipv4
        udp_tmp = (struct udphdr*)(buff_for_serv + ip_header->ihl * 4);
        if(ntohs(udp_tmp->dest) == PORT_CL){
            printf("Клиент получил udp-датаграмму от сервера [%s:%hu]\n", inet_ntoa(serv.sin_addr), ntohs(serv.sin_port)); 
            printf("len: %u\ndest: %u\nsrc: %u\ncheck: %u\n", ntohs(udp_tmp->len), ntohs(udp_tmp->dest), ntohs(udp_tmp->source), ntohs(udp_tmp->check));
            printf("payload: %s\n", (char*)udp_tmp + sizeof(struct udphdr));
            break;
        }
    }

    


    free(buff_for_cl);
    return 0;
}