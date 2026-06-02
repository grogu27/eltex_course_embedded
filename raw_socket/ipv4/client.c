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

//     0                   1                   2                   3                
//     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//    |Version|  IHL  |Type of Service|          Total Length         |
//    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//    |         Identification        |Flags|      Fragment Offset    |
//    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//    |  Time to Live |    Protocol   |         Header Checksum       |
//    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//    |                       Source Address                          |
//    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//    |                    Destination Address  

// struct iphdr
//   {
// #if __BYTE_ORDER == __LITTLE_ENDIAN
//     unsigned int ihl:4;
//     unsigned int version:4;
// #elif __BYTE_ORDER == __BIG_ENDIAN
//     unsigned int version:4;
//     unsigned int ihl:4;
// #else
// # error	"Please fix <bits/endian.h>"
// #endif
//     uint8_t tos;
//     uint16_t tot_len;
//     uint16_t id;
//     uint16_t frag_off;
//     uint8_t ttl;
//     uint8_t protocol;
//     uint16_t check;
//     uint32_t saddr;
//     uint32_t daddr;
//     /*The options start here. */
//   };

            //   │ IP Header fields modified on sending by IP_HDRINCL │
            //   ├───────────────────────┬────────────────────────────┤
            //   │ IP Checksum           │ Always filled in           │
            //   ├───────────────────────┼────────────────────────────┤
            //   │ Source Address        │ Filled in when zero        │
            //   ├───────────────────────┼────────────────────────────┤
            //   │ Packet ID             │ Filled in when zero        │
            //   ├───────────────────────┼────────────────────────────┤
            //   │ Total Length          │ Always filled in           │

int main(){

    int sock;
    struct sockaddr_in serv;
    struct udphdr udp_header;
    struct iphdr ip_header;
    struct udphdr *udp_tmp = NULL;
    struct iphdr *ip_header_tmp = NULL;
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

    int flag = 1;
    if(setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &flag, sizeof(flag)) < 0){
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    serv.sin_family = AF_INET;
    serv.sin_port = htons(PORT_SERV);
    serv.sin_addr.s_addr = inet_addr(IP_SERV);

    ip_header.id = 0; // ядро ос сама заполнит
    ip_header.daddr = inet_addr(IP_SERV); // inet_addr работает только для ipv4
    ip_header.saddr = inet_addr(IP_CL);
    ip_header.frag_off = 0; // размер ip пакета не превышает MTU, так что фрагментации не будет
    ip_header.ihl = 5; // 5 * 5 = 20 байт (ip пакет без опций)
    ip_header.check = 0; // ядро ос сама заполнит
    ip_header.tos = 0; // вроде можно не указывать
    ip_header.protocol = 17; // код udp. вроде как можно использовать IPPROTO_UDP
    ip_header.ttl = 128;
    ip_header.version = IPVERSION;
    ip_header.tot_len = 0; // ядро ос сама заполнит

    udp_header.dest = htons(PORT_SERV);
    udp_header.source = htons(PORT_CL);
    udp_header.check = 0; // ядро ос сама заполнит
    udp_header.len = htons(sizeof(struct udphdr) + len_message + 1);

    buff_for_cl = malloc(ntohs(udp_header.len) + ip_header.ihl * 4); // В udp_header.len входит заголовок и сами данные. В ip_header.ihl только длина заголовка
    if(buff_for_cl == NULL){
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    memcpy(buff_for_cl, &ip_header, ip_header.ihl * 4);
    memcpy(buff_for_cl + ip_header.ihl * 4, &udp_header, sizeof(struct udphdr));
    memcpy(buff_for_cl + ip_header.ihl * 4 + sizeof(struct udphdr), message, len_message + 1);

    ssize_t len_ip_pack = ntohs(udp_header.len) + ip_header.ihl * 4;
    send_bytes = sendto(sock, buff_for_cl, len_ip_pack, 0, (struct sockaddr*)&serv, len_serv);
    if(send_bytes < 0){
        perror("sendto");
        exit(EXIT_FAILURE);
    }
    printf("Клиента отправил серверу udp-датаграмму размером %ld байт\nПолезная нагрузка: %s\n", send_bytes, buff_for_cl + ip_header.ihl * 4 + sizeof(struct udphdr));

    int count = 1;
    while(1){
        printf("%d)Попытка найти нужный пакет\n", count++);
        memset(buff_for_serv, 0, N);
        recv_bytes = recvfrom(sock, buff_for_serv, N, 0, (struct sockaddr*)&serv, &len_serv);
        if(recv_bytes < 0){
            perror("recvfrom");
            exit(EXIT_FAILURE);
        }
        ip_header_tmp = (struct iphdr*)buff_for_serv; // сначала идёт заголовок ipv4
        udp_tmp = (struct udphdr*)(buff_for_serv + ip_header_tmp->ihl * 4);
        if(ntohs(udp_tmp->dest) == PORT_CL){
            printf("Клиент получил нужный ip-пакет от сервера с ip адресом %s\n", inet_ntoa(serv.sin_addr));
            printf("tot_len: %u\nheader_len: %u\nsrc_ip: %s\ndest_ip: %s\ncheck: %u\nttl: %u\n", ntohs(ip_header_tmp->tot_len), ip_header_tmp->ihl, inet_ntoa(*(struct in_addr*)&ip_header_tmp->saddr), inet_ntoa(*(struct in_addr*)&ip_header_tmp->daddr), ntohs(ip_header_tmp->check), ip_header_tmp->ttl);
            printf("Клиент получил udp-датаграмму от процеса с портом %hu\n", udp_tmp->source); 
            printf("len: %u\ndest: %u\nsrc: %u\ncheck: %u\n", ntohs(udp_tmp->len), ntohs(udp_tmp->dest), ntohs(udp_tmp->source), ntohs(udp_tmp->check));
            printf("payload: %s\n", (char*)udp_tmp + sizeof(struct udphdr));
            break;
        }
    }

    


    free(buff_for_cl);
    return 0;
}