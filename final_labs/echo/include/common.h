#ifndef COMMON_H
#define COMMON_H

#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 7777
#define DEFAULT_CLIENT_PORT 7776
#define MAX_TEXT 256
#define MAX_PACKET 512

// Структура клиента
typedef struct {
    uint32_t ip;        // IP-адрес в сетевом порядке
    uint16_t port;      // Порт клиента в хостовом порядке
    int count;          // Счетчик сообщений
} ClientInfo;

#endif 