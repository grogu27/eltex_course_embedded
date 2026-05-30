#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <signal.h>

#define MQ_SERV "server" 

// сообщения от сервера имеют приоритет 10
// сообщения от клиента имеют приоритет 20

typedef struct {
    long mtyp; //служебное поле. остальное playload
    pid_t pid;
    char mtext[255];
}msgbuf;

int ds_mq_serv;

//Если запустить только сервер и сразу его закрыть с помощью сигнала, то очередь сообщений не удалится. Поэтому было принято решение создать обработчики сигналов.
void sigint_handler(int sig) {
    msgctl(ds_mq_serv, IPC_RMID, NULL);
    printf("\nОчередь удалена\n");
    exit(EXIT_SUCCESS);
}

void sigterm_handler(int sig) {
    msgctl(ds_mq_serv, IPC_RMID, NULL);
    printf("\nОчередь удалена\n");
    exit(EXIT_SUCCESS);
}

void sigquit_handler(int sig) {
    msgctl(ds_mq_serv, IPC_RMID, NULL);
    printf("\nОчередь удалена\n");
    exit(EXIT_SUCCESS);
}

int main(){
    // Быстрое решение
    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, sigquit_handler);
    signal(SIGTERM, sigterm_handler);
    key_t key_serv;

    if((key_serv = ftok(MQ_SERV, 1)) < 0){
        perror("ftok");
        exit(EXIT_FAILURE);
    }
    

    if((ds_mq_serv = msgget(key_serv, IPC_CREAT | 0666)) < 0){
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    msgbuf message;
    sprintf(message.mtext, "%s", "Hi!");
    message.mtyp = 10;
    message.pid = getpid();
    
    int res_send = msgsnd(ds_mq_serv, &message, sizeof(msgbuf) - sizeof(long), 0);
    if(res_send < 0){
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }
    printf("PID: %d\n", getpid());
    printf("Сервер отправил сообщение в очередь сообщений: %s\n", message.mtext);

    msgbuf message_from_cl;
    memset(&message_from_cl, 0, sizeof(message_from_cl));
    ssize_t res_recv = msgrcv(ds_mq_serv, &message_from_cl, sizeof(message_from_cl) - sizeof(long), 20, 0);
    if(res_recv < 0){
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }
    printf("Сервер получил от %d сообщение: %s\t%ld байт\n", message_from_cl.pid, message_from_cl.mtext, res_recv);

    
    struct msqid_ds mq_serv_ds;
    struct ipc_perm mq_serv_perm;

    if(msgctl(ds_mq_serv, IPC_STAT, &mq_serv_ds) < 0){
        perror("msgctl");
        exit(EXIT_FAILURE);
    }
    msglen_t len_mq = mq_serv_ds.msg_qbytes;
    mq_serv_perm = mq_serv_ds.msg_perm;
    printf("Информация о очереди сообщений, которую создал сервер: Размер одного сообщения: %zu\nКоличество сообщений в очереди: %lu\nПрава: gid(%d) uid(%d) mode(%hu)\n", len_mq, mq_serv_ds.msg_qnum, mq_serv_perm.gid, mq_serv_perm.uid, mq_serv_perm.mode);

    msgctl(ds_mq_serv, IPC_RMID, NULL);

    return 0;
}