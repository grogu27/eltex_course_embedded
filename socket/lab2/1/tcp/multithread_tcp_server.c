#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>

#define SERV_PORT 7777
#define SERV_IP "127.0.0.1"
#define MAX_THREADS 100

// Структура для передачи данных в поток
typedef struct {
    int sock_cl;
    struct sockaddr_in client;
} client_info_t;
void *client_handler(void *arg);

int main() {
    int sock_cl, sock_serv;
    struct sockaddr_in serv, client;
    socklen_t size_serv = sizeof(serv);
    socklen_t size_client = sizeof(client);
    pthread_t thread_id;
    client_info_t *client_info;


    memset(&serv, 0, size_serv);
    memset(&client, 0, size_client);

    if ((sock_serv = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    serv.sin_family = AF_INET;
    serv.sin_port = htons(SERV_PORT);
    if (inet_pton(AF_INET, SERV_IP, &serv.sin_addr) != 1) {
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }

    if (bind(sock_serv, (struct sockaddr*)&serv, size_serv) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(sock_serv, 5) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("СЕРВЕР: Запущен на %s:%d\n", SERV_IP, SERV_PORT);

    while (1) {
        memset(&client, 0, size_client);
        if ((sock_cl = accept(sock_serv, (struct sockaddr*)&client, &size_client)) < 0) {
            if (errno == EINTR) {
                continue;
            }
            perror("accept");
            close(sock_serv);
            exit(EXIT_FAILURE);
        }
        
        printf("СЕРВЕР: Подключен клиент %s:%d\n", 
               inet_ntoa(client.sin_addr), ntohs(client.sin_port));

        // Выделяем память для структуры с данными клиента
        client_info = (client_info_t*)malloc(sizeof(client_info_t));
        if (client_info == NULL) {
            perror("malloc");
            close(sock_cl);
            continue;
        }
        
        client_info->sock_cl = sock_cl;
        client_info->client = client;

        // Создаем поток для обработки клиента
        if (pthread_create(&thread_id, NULL, client_handler, (void*)client_info) != 0) {
            perror("pthread_create");
            free(client_info);
            close(sock_cl);
            continue;
        }
        
        pthread_detach(thread_id);
    }

    close(sock_serv);
    return 0;
}

void *client_handler(void *arg) {
    client_info_t *info = (client_info_t*)arg;
    int sock_cl = info->sock_cl;
    struct sockaddr_in client = info->client;
    ssize_t bytes_from_cl;
    ssize_t bytes_from_serv;
    char buff_cl[255];
    char message[255];
    int len_buff_cl;
    int count = 1;
    
    free(info); // Освобождаем память, так как она больше не нужна

    printf("ПОТОК %lu: Начал обработку клиента %s:%d\n", 
           pthread_self(), inet_ntoa(client.sin_addr), ntohs(client.sin_port));

    while (1) {
        memset(buff_cl, 0, sizeof(buff_cl));
        memset(message, 0, sizeof(message));
        
        bytes_from_cl = recv(sock_cl, buff_cl, sizeof(buff_cl) - 1, 0);
        
        if (bytes_from_cl < 0) {
            perror("recv");
            break;
        }
        if (bytes_from_cl == 0) {
            printf("ПОТОК %lu: Клиент %s:%d перестал отправлять данные\n",
                   pthread_self(), inet_ntoa(client.sin_addr), ntohs(client.sin_port));
            break;
        }
        
        printf("ПОТОК %lu: Сервер получил от клиента[%s:%d] сообщение №%d: %s\n",
               pthread_self(), inet_ntoa(client.sin_addr), 
               ntohs(client.sin_port), count++, buff_cl);
        
        len_buff_cl = strlen(buff_cl);
        snprintf(message, sizeof(message), "%s", buff_cl);
        message[len_buff_cl] = '5';
        message[len_buff_cl + 1] = '\0';

        bytes_from_serv = send(sock_cl, message, strlen(message) + 1, 0);
        if (bytes_from_serv < 0) {
            perror("send");
            break;
        }
    }

    close(sock_cl);
    printf("ПОТОК %lu: Завершил обработку клиента %s:%d\n",
           pthread_self(), inet_ntoa(client.sin_addr), ntohs(client.sin_port));
    
    pthread_exit(NULL);
}