#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

#define PORT_CL 7777

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

int main(){

    int fd_socket;
    ssize_t recv_bytes;

    if((fd_socket = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) < 0){
        perror("socket");
        exit(EXIT_FAILURE);
    }
    struct iphdr *ip = NULL;
    struct udphdr *udp = NULL;
    char *payload = NULL;
    size_t count = 1;
    int ip_len;
    char buf[65535];
    char src[INET_ADDRSTRLEN];
    char dst[INET_ADDRSTRLEN];
    while(1){
        printf("%lu)------------------------------------\n", count++);
        memset(buf, 0, sizeof(buf));
        recv_bytes = recvfrom(fd_socket, buf, sizeof(buf), 0, NULL, NULL);
        if(recv_bytes < 0){
            perror("recvfrom");
            exit(EXIT_FAILURE);
        }
        ip = (struct iphdr*)buf;

        inet_ntop(AF_INET, &ip->saddr, src, sizeof(src));
        inet_ntop(AF_INET, &ip->daddr, dst, sizeof(dst));

        printf("ip version: %u\nihl: %u\ntos: %u\nid: %u\nfrag off: %u\nttl: %u\nprotocol: %u\nip_check: %u\ns_addr: %s\nd_addr: %s\n", \
            ip->version, ip->ihl, ip->tos, ip->id, ip->frag_off, ip->ttl, ip->protocol, ip->check, src, dst);
        ip_len = ip->ihl * 4; //ihl хранится в 32-битных словах.
        udp = (struct udphdr*)(buf + ip_len);
        payload = buf + ip_len + sizeof(struct udphdr); 

        printf("s_port: %u\nd_port: %u\nudp_len: %u\nudp_check: %u\n", udp->source, udp->dest, udp->len, udp->check);

        printf("payload: %s\n", payload);
    }


    return 0;
}
