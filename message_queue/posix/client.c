#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

#define SERVER_MQ "/mq_serv.mq"
#define CL_MQ "/mq_cl.mq"

struct message{
    pid_t pid;
    char text[6];
};

// сообщения от сервера имеют приоритет 10
// сообщения от клиента имеют приоритет 20
int main(){
    printf("pid=%d\n", getpid());
    mqd_t mq_fd_cl, mq_fd_serv;
    struct message message = {.pid = getpid(), .text = "hi2"};
    struct message message_from_server = {0};
    unsigned int prio;
    struct mq_attr attr;
    memset(&attr, 0, sizeof(attr));
    mq_fd_cl = mq_open(CL_MQ, O_WRONLY);
    if(mq_fd_cl < 0){
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    mq_fd_serv = mq_open(SERVER_MQ, O_RDONLY);
    if(mq_fd_serv < 0){
        perror("mq_open");
        exit(EXIT_FAILURE);
    }
        
    mq_getattr(mq_fd_serv, &attr);

    ssize_t res_mq_recv = mq_receive(mq_fd_serv, (char*)&message_from_server, attr.mq_msgsize, &prio);
    if(res_mq_recv < 0){
        perror("mq_receive");
        exit(EXIT_FAILURE);
    }
    printf("Клиент получил от сервера(pid=%d) сообщение: %s\t%ld байт\tс приоритетом %u\n",message_from_server.pid, message_from_server.text, res_mq_recv, prio);

    int res_mq_send = mq_send(mq_fd_cl, (char *)&message, sizeof(message), 20);
    if(res_mq_send < 0){
        perror("mq_send");
        exit(EXIT_FAILURE);
    }   
    printf("Клиент записал в очередь сообщений сообщение: %s\tразмером %ld байт\tс приоритетом %u\n", message.text, sizeof(message), 20);

    printf("(SERVER_MQ)Макс кол-ва сообщений в очереди: %ld\tМакс размер одного сообщения: %ld\n", attr.mq_maxmsg, attr.mq_msgsize);
    
    mq_close(mq_fd_serv);
    mq_close(mq_fd_cl);
    return 0;
}