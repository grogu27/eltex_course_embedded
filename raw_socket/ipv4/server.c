#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define PORT_SERV 7777
//#define PORT_CL 8888
#define IP_SERV "127.0.0.1"
//#define IP_CL "127.0.0.1"
#define N 255

int main(){
    int sock;
    struct sockaddr_in serv, client;
    ssize_t recv_bytes;
    ssize_t send_bytes;
    char message[N];
    char buff[N];
    socklen_t len_serv = sizeof(struct sockaddr_in);
    socklen_t len_client = sizeof(struct sockaddr_in);
    const char *chars = "qwertyuiop[]{}asdfghjkl;'|zxcvbnm,./?1234567890-=+_";
    size_t i, j, count;
    size_t len_chars = strlen(chars);
    srand(time(NULL));

    if((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&serv, 0, len_serv);
    memset(&client, 0, len_client);

    serv.sin_family = AF_INET;
    serv.sin_port = htons(PORT_SERV);

    if(inet_pton(AF_INET, IP_SERV, &serv.sin_addr) != 1){
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }

    if(bind(sock, (struct sockaddr*)&serv, len_serv) < 0){
        perror("bind");
        exit(EXIT_FAILURE);
    }

    count = 1;
    printf("Сервер работает....\n");
    while(1){
        printf("%ld)------------------\n", count++);
        memset(buff, 0, sizeof(buff));

        recv_bytes = recvfrom(sock, buff, N, 0, (struct sockaddr*)&client, &len_client);
        if(recv_bytes < 0){
            perror("recvfrom");
            exit(EXIT_FAILURE);
        }

        printf("Сервер получил от клиента (%s:%hu) сообщение: %s размером: %ld байт\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port), buff, recv_bytes);

        size_t len_buff = strlen(buff);
        if (len_buff == 0)
            continue;
        buff[len_buff] = '\0';
        strncpy(message, buff, len_buff+1);
        i = rand() % len_buff;   // генерация псевдослучайной дискретной величины в диапазоне от 0 до len_buff. 
        j = rand() % len_chars;
        message[i] = chars[j];

        printf("Сервер изменил один символ в полученной строке: %s\ni = %ld, j = %ld\nСервер поменял символ '%c' на символ '%c'\n", message, i, j, buff[i], chars[j]);

        send_bytes = sendto(sock, message, len_buff+1, 0, (struct sockaddr*)&client, len_client);
        if(send_bytes < 0){
            perror("sendto");
            exit(EXIT_FAILURE);
        }
        printf("Сервер отправил клиенту сообщение: %s размером: %ld байт\n", message, send_bytes);

    }

    return 0;
}