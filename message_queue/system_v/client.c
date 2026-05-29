#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>

#define MQ_SERV "server" 

// сообщения от сервера имеют приоритет 10
// сообщения от клиента имеют приоритет 20

typedef struct {
    long mtyp;
    pid_t pid;
    char mtext[255];
}msgbuf;

int main(){

    key_t key_serv; 
    int ds_mq_serv;

    char path_server[255];
    char cwd[255];
    snprintf(path_server, sizeof(path_server), "%s/%s", getcwd(cwd, sizeof(cwd)), MQ_SERV);


    if((key_serv = ftok(path_server, 1)) < 0){
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    if((ds_mq_serv = msgget(key_serv, 0666)) < 0){
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    msgbuf message, message_from_serv;
    sprintf(message.mtext, "%s", "Hello!");
    message.mtyp = 20;
    message.pid = getpid();

    memset(&message_from_serv, 0, sizeof(message_from_serv));
    ssize_t res_recv = msgrcv(ds_mq_serv, &message_from_serv, sizeof(message_from_serv) - sizeof(long), 10, 0);
    if(res_recv < 0){
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }
    printf("PID: %d\n", getpid());
    printf("Клиент получил от %d сообщение: %s\t%ld байт\n", message_from_serv.pid, message_from_serv.mtext, res_recv);

    int res_send = msgsnd(ds_mq_serv, &message, sizeof(msgbuf) - sizeof(long), 0);
    if(res_send < 0){
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }
    printf("Клиент отправил сообщение в очередь сообщений: %s\n", message.mtext);

    struct msqid_ds mq_cl_ds;
    struct ipc_perm mq_cl_perm;

    if(msgctl(ds_mq_serv, IPC_STAT, &mq_cl_ds) < 0){
        perror("msgctl");
        exit(EXIT_FAILURE);
    }
    msglen_t len_mq = mq_cl_ds.msg_qbytes;
    mq_cl_perm = mq_cl_ds.msg_perm;
    printf("Информация о очереди сообщений, которую создал сервер: Размер одного сообщения: %zu\nКоличество сообщений в очереди: %lu\nПрава: gid(%d) uid(%d) mode(%hu)\n", len_mq, mq_cl_ds.msg_qnum, mq_cl_perm.gid, mq_cl_perm.uid, mq_cl_perm.mode);

    return 0;
}