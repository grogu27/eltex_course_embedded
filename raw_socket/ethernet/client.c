#include <sys/socket.h>
#include <arpa/inet.h>
// #include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/udp.h>
#include <netinet/ip.h>
#include <net/ethernet.h>
#include <linux/if_packet.h>
#include <net/if.h>

#define PORT_SERV 7777
#define PORT_CL 8888
#define IP_SERV "192.168.202.11"
#define IP_CL "192.168.202.10"
#define N 255
#define IF_NAME "eth0"

uint16_t ip_checksum(void *buf, int len);

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

// struct sockaddr_ll {
//                unsigned short sll_family;   /* Always AF_PACKET */
//                unsigned short sll_protocol; /* Physical-layer protocol */
//                int            sll_ifindex;  /* Interface number */
//                unsigned short sll_hatype;   /* ARP hardware type */
//                unsigned char  sll_pkttype;  /* Packet type */
//                unsigned char  sll_halen;    /* Length of address */
//                unsigned char  sll_addr[8];  /* Physical-layer address */
// }

// struct ethhdr {
// 	unsigned char	h_dest[ETH_ALEN];	/* destination eth addr	*/
// 	unsigned char	h_source[ETH_ALEN];	/* source ether addr	*/
// 	__be16		h_proto;		/* packet type ID field	*/
// } __attribute__((packed));

int main(){

    int sock;
    struct sockaddr_ll serv;
    struct udphdr udp_header;
    struct iphdr ip_header;
    struct ethhdr eth_header;
    struct ethhdr *eth_header_tmp = NULL;
    struct udphdr *udp_tmp = NULL;
    struct iphdr *ip_header_tmp = NULL;
    const char *message = "Hello server123";
    char buff_for_serv[N];
    char *buff_for_cl;
    ssize_t send_bytes, recv_bytes;
    socklen_t len_serv = sizeof(struct sockaddr_ll);
    size_t len_message = strlen(message);

    if((sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0){
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // int flag = 1;
    // if(setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &flag, sizeof(flag)) < 0){
    //     perror("setsockopt");
    //     exit(EXIT_FAILURE);
    // }

    unsigned char dst_mac[6] =
    {
        0x08,0x00,0x27,0x8d,0x96,0xc2
    };

    unsigned char src_mac[6] =
    {
        0x08,0x00,0x27,0x94,0x39,0x7a
    };

    //When you send packets, it is enough to specify sll_family, sll_addr, sll_halen, sll_ifindex, and sll_protocol.  The other fields should be 0.  sll_hatype and sll_pkttype are set on received
    //  packets for your information.
    //Если не вызвать bind(), сокет слушает все интерфейсы.



    memset(&serv, 0, sizeof(struct sockaddr_ll));
    serv.sll_family = AF_PACKET;
    serv.sll_halen = ETH_ALEN; // 6
    serv.sll_ifindex = if_nametoindex(IF_NAME);
    serv.sll_protocol = htons(ETH_P_ALL);
    memcpy(serv.sll_addr, dst_mac, 6);

    eth_header.h_proto = htons(ETH_P_IP); // ipv4 ETH_P_IP
    memcpy(eth_header.h_dest, dst_mac, 6);
    memcpy(eth_header.h_source, src_mac, 6);
    
    memset(&ip_header, 0, sizeof(ip_header));
    ip_header.id = htons(rand()); 
    ip_header.daddr = inet_addr(IP_SERV); // inet_addr работает только для ipv4
    ip_header.saddr = inet_addr(IP_CL);
    ip_header.frag_off = 0; // размер ip пакета не превышает MTU, так что фрагментации не будет
    ip_header.ihl = 5; // 5 * 5 = 20 байт (ip пакет без опций)
    ip_header.check = 0; 
    ip_header.tos = 0; // вроде можно не указывать
    ip_header.protocol = IPPROTO_UDP; // код udp. вроде как можно использовать IPPROTO_UDP
    ip_header.ttl = 128;
    ip_header.version = IPVERSION;
    ip_header.tot_len = htons(sizeof(struct udphdr) + sizeof(struct iphdr) + len_message + 1); 

    ip_header.check = ip_checksum(&ip_header, sizeof(ip_header));


    udp_header.dest = htons(PORT_SERV);
    udp_header.source = htons(PORT_CL);
    udp_header.check = 0; // ядро ос сама заполнит
    udp_header.len = htons(sizeof(struct udphdr) + len_message + 1);

    size_t total_len = sizeof(struct ethhdr) + (ip_header.ihl * 4) + ntohs(udp_header.len);
    buff_for_cl = malloc(total_len); // В udp_header.len входит заголовок и сами данные. В ip_header.ihl только длина заголовка
    if(buff_for_cl == NULL){
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    memcpy(buff_for_cl, &eth_header, sizeof(struct ethhdr));
    memcpy(buff_for_cl + sizeof(struct ethhdr), &ip_header, ip_header.ihl * 4);
    memcpy(buff_for_cl + ip_header.ihl * 4 + sizeof(struct ethhdr), &udp_header, sizeof(struct udphdr));
    memcpy(buff_for_cl + ip_header.ihl * 4 + sizeof(struct udphdr) + sizeof(struct ethhdr), message, len_message + 1);

    send_bytes = sendto(sock, buff_for_cl, total_len, 0, (struct sockaddr*)&serv, len_serv);
    if(send_bytes < 0){
        perror("sendto");
        exit(EXIT_FAILURE);
    }
    printf("Клиента отправил серверу udp-датаграмму размером %ld байт\nПолезная нагрузка: %s\n", send_bytes, buff_for_cl + ip_header.ihl * 4 + sizeof(struct udphdr));

    int count = 1;
    while (1) {
        printf("%d) Попытка найти нужный пакет\n", count++);
        memset(buff_for_serv, 0, N);
        
        recv_bytes = recvfrom(sock, buff_for_serv, N, 0, (struct sockaddr*)&serv, &len_serv);
        if (recv_bytes < 0) {
            perror("recvfrom");
            break;
        }
        
        // Получаем указатели, учитывая Ethernet-заголовок
        eth_header_tmp = (struct ethhdr*)buff_for_serv;
        
        // Проверяем, что это IP-пакет
        if (ntohs(eth_header_tmp->h_proto) != ETH_P_IP) {
            continue;  // Не IP-пакет, пропускаем
        }
        
        // Указатель на IP-заголовок (после Ethernet)
        ip_header_tmp = (struct iphdr*)(buff_for_serv + sizeof(struct ethhdr));
        
        // Проверяем, что это UDP
        if (ip_header_tmp->protocol != IPPROTO_UDP) {
            continue;
        }
        
        // Указатель на UDP-заголовок
        udp_tmp = (struct udphdr*)(buff_for_serv + sizeof(struct ethhdr) + 
                                   (ip_header_tmp->ihl * 4));
        
        // Проверяем, что пакет предназначен нашему клиентскому порту
        if (ntohs(udp_tmp->dest) == PORT_CL) {
            printf("\n=== Получен ответ от сервера ===\n");
            
            // Вывод информации об IP-заголовке
            char src_ip[INET_ADDRSTRLEN];
            char dst_ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &ip_header_tmp->saddr, src_ip, sizeof(src_ip));
            inet_ntop(AF_INET, &ip_header_tmp->daddr, dst_ip, sizeof(dst_ip));
            
            printf("IP-заголовок:\n");
            printf("  tot_len: %u байт\n", ntohs(ip_header_tmp->tot_len));
            printf("  ihl: %u (%u байт)\n", ip_header_tmp->ihl, ip_header_tmp->ihl * 4);
            printf("  src_ip: %s\n", src_ip);
            printf("  dst_ip: %s\n", dst_ip);
            printf("  check: 0x%04x\n", ntohs(ip_header_tmp->check));
            printf("  ttl: %u\n", ip_header_tmp->ttl);
            
            printf("\nUDP-заголовок:\n");
            printf("  src_port: %u\n", ntohs(udp_tmp->source));
            printf("  dst_port: %u\n", ntohs(udp_tmp->dest));
            printf("  len: %u байт\n", ntohs(udp_tmp->len));
            printf("  check: 0x%04x\n", ntohs(udp_tmp->check));
            
            printf("\nПолезная нагрузка: %s\n", 
                   (char*)udp_tmp + sizeof(struct udphdr));
            break;
        }
    }
    

    



    free(buff_for_cl);
    close(sock);
    return 0;
}

uint16_t ip_checksum(void *buf, int len) {
    uint32_t sum = 0;
    uint16_t *ptr = (uint16_t *)buf;
    int i;
    
    // Суммируем по 2 байтам
    for (i = 0; i < len / 2; i++) {
        sum += *ptr;  
        ptr++;
    }
    
    if (len % 2 != 0) {
        sum += *(uint8_t *)ptr;
    }
    
    while (sum >> 16) {
        sum = (sum & 0xFFFF) + (sum >> 16);
    }
    
    return (uint16_t)~sum;
}
