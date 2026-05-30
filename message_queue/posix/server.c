#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

#define SERVER_MQ "/mq_serv.mq"
#define CL_MQ "/mq_cl.mq"
        //    struct mq_attr {
        //        long mq_flags;       /* Flags (ignored for mq_open()) */
        //        long mq_maxmsg;      /* Max. # of messages on queue */
        //        long mq_msgsize;     /* Max. message size (bytes) */
        //        long mq_curmsgs;     /* # of messages currently in queue
        //                                (ignored for mq_open()) */
        //    };

        
// сообщения от сервера имеют приоритет 10
// сообщения от клиента имеют приоритет 20

// grogu@localhost:~/eltex/embedded_c/eltex_course_embedded/message_queue/posix copy$ cat /proc/sys/fs/mqueue/msgsize_max
// 8192

// в моей системе по умолчанию размер одного сообщения 8Kb
struct message{
    pid_t pid;
    char text[6];
};

int main(){
    printf("pid=%d\n", getpid());
    mq_unlink(SERVER_MQ);
    mq_unlink(CL_MQ);
    mqd_t mq_fd_serv, mq_fd_cl;
    struct mq_attr attr;
    memset(&attr, 0, sizeof(attr));
    attr.mq_msgsize = 1024;
    attr.mq_maxmsg = 10;

    mq_fd_serv = mq_open(SERVER_MQ, O_CREAT | O_WRONLY, 0666, &attr);
    if(mq_fd_serv < 0){
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    mq_fd_cl = mq_open(CL_MQ, O_CREAT | O_RDONLY, 0666, NULL);
    if(mq_fd_cl < 0){
        perror("mq_open");
        exit(EXIT_FAILURE);
    }
    struct message message = {.pid = getpid(), .text = "hi!"};

    int res_mq_send = mq_send(mq_fd_serv, (char*)&message, sizeof(message), 10);
    if(res_mq_send < 0){
        perror("mq_send");
        exit(EXIT_FAILURE);
    }
    printf("Сервер записал в очередь сообщений сообщение: %s\tразмером %ld байт\tс приоритетом %u\n", message.text, sizeof(message), 10);
    
    mq_getattr(mq_fd_cl, &attr);

    char *buff_message_from_cl = malloc(attr.mq_msgsize); 
    unsigned int prio;
    ssize_t res_mq_receive = mq_receive(mq_fd_cl, buff_message_from_cl, attr.mq_msgsize, &prio);
    struct message *message_from_cl = (struct message*)buff_message_from_cl;
    
    printf("Сервер получил от клиента(pid=%d) сообщение: %s\t%ld байт\tс приоритетом %u\n", message_from_cl->pid, message_from_cl->text, res_mq_receive, prio);


    printf("(CL_MQ)Макс кол-ва сообщений в очереди: %ld\tМакс размер одного сообщения: %ld\n", attr.mq_maxmsg, attr.mq_msgsize);

    free(message_from_cl);
    mq_close(mq_fd_serv);
    mq_close(mq_fd_cl);
    mq_unlink(SERVER_MQ);
    mq_unlink(CL_MQ);
    return 0;
}