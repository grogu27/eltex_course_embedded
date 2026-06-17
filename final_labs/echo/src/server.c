#include "server.h"
#include "common.h"
#include "vector.h"

// Получение или создание клиента
static int get_client(Vector *clients, uint32_t ip, uint16_t port) {
    int index = vector_find_client(clients, ip, port);
    
    if (index != -1) {
        return index;
    }
    
    ClientInfo new_client = {
        .ip = ip,
        .port = port,
        .count = 0
    };
    
    if (vector_push_back(clients, &new_client) != 0) {
        return -1;
    }
    
    return (int)(clients->size - 1);
}

// Удаление клиента по индексу
static void remove_client(Vector *clients, int index) {
    if (index >= 0 && index < (int)clients->size) {
        vector_erase(clients, (size_t)index);
    }
}

// Отправка ответа клиенту
static int send_answer(int sock_fd, uint32_t dst_ip, uint16_t dst_port, const char *text) {
    char packet[sizeof(struct udphdr) + MAX_TEXT + 32];
    struct udphdr *udp_header = (struct udphdr *)packet;
    struct sockaddr_in addr;
    size_t text_len = strlen(text);

    memset(packet, 0, sizeof(packet));
    udp_header->source = htons(SERVER_PORT);
    udp_header->dest = htons(dst_port);
    udp_header->len = htons((uint16_t)(sizeof(*udp_header) + text_len));
    memcpy(udp_header + 1, text, text_len);

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(dst_port);
    addr.sin_addr.s_addr = dst_ip;

    if (sendto(sock_fd, packet, sizeof(*udp_header) + text_len, 0,
        (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("sendto");
        return 1;
    }

    return 0;
}

int server_run(void) {
    int sock_fd;
    Vector *clients;
    char packet[MAX_PACKET];
    
    clients = vector_create(sizeof(ClientInfo));
    if (!clients) {
        fprintf(stderr, "Failed to create clients vector\n");
        return 1;
    }

    sock_fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (sock_fd == -1) {
        perror("socket");
        vector_destroy(clients);
        return 1;
    }

    printf("Raw echo server started on port %d\n", SERVER_PORT);


    while (1) {
        struct iphdr *ip_header;
        struct udphdr *udp_header;
        char text[MAX_TEXT];
        char answer[MAX_TEXT + 32];
        char ip_text[INET_ADDRSTRLEN];
        ssize_t bytes_read = recv(sock_fd, packet, sizeof(packet), 0);
        size_t text_len;
        int index;

        if (bytes_read == -1) {
            perror("recv");
            continue;
        }

        ip_header = (struct iphdr *)packet;
        if (ip_header->protocol != IPPROTO_UDP) {
            continue;
        }

        udp_header = (struct udphdr *)(packet + ip_header->ihl * 4);
        if (ntohs(udp_header->dest) != SERVER_PORT) {
            continue;
        }

        text_len = ntohs(udp_header->len) - sizeof(*udp_header);
        if (text_len >= sizeof(text)) {
            text_len = sizeof(text) - 1;
        }

        memcpy(text, udp_header + 1, text_len);
        text[text_len] = '\0';

        uint32_t client_ip = ip_header->saddr;
        uint16_t client_port = ntohs(udp_header->source);

        // Обработка close-сообщения
        if (strcmp(text, "close") == 0) {
            index = vector_find_client(clients, client_ip, client_port);
            if (index != -1) {
                remove_client(clients, index);
                inet_ntop(AF_INET, &client_ip, ip_text, sizeof(ip_text));
                printf("Client %s:%u closed (removed from vector)\n", 
                       ip_text, client_port);
                printf("Active clients: %zu\n", vector_size(clients));
            }
            continue;
        }

        index = get_client(clients, client_ip, client_port);
        if (index == -1) {
            fprintf(stderr, "Failed to add client\n");
            continue;
        }

        ClientInfo *client = (ClientInfo*)vector_at(clients, index);
        if (!client) continue;
        
        client->count++;
        
        snprintf(answer, sizeof(answer), "%s %d", text, client->count);

        inet_ntop(AF_INET, &client_ip, ip_text, sizeof(ip_text));
        printf("%s:%u -> %s (client #%d, total clients: %zu)\n", 
               ip_text, client_port, answer, index, vector_size(clients));

        send_answer(sock_fd, client_ip, client_port, answer);
    }

    vector_destroy(clients);
    close(sock_fd);
    return 0;
}