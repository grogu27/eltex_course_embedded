#include "shm.h"
// #include <string.h>

// диапазон адресов для сервера: 0 - 1023. для клиента: 1024-2047
// сервер использует первые 1024 байта;
// клиент использует вторые 1024 байта.

int main(){
    printf("PID: %d\n", getpid());
    int shm_ds;
    int sem_ds;
    key_t shm_key;
    key_t sem_key;
    void *address = NULL;
    struct data *server_data = NULL;
    struct data *client_data = NULL;

    shm_key = gen_key(SHM_NAME, 1);
    sem_key = gen_key(SEM_NAME, 2);

    shm_ds = creat_shm(shm_key, SHM_SIZE);

    address = attach_shm(shm_ds, NULL, 0);

    server_data = (struct data*)address;

    server_data->pid = getpid();
    snprintf(server_data->text, sizeof(server_data->text) ,"%s", "Hi!");

    sleep(10);

    client_data = (struct data *)(1024 + (char *)server_data); // смещение на 1024 байта
    printf("Server data address: %p\n", server_data);
    printf("Client pid: %d\nClient text: %s\nClient data address: %p\n", client_data->pid, client_data->text, client_data);

    if(shmdt(address) < 0){
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    if(shmctl(shm_ds, IPC_RMID, NULL) < 0){
        perror("shmctl");
        exit(EXIT_FAILURE);
    }

    return 0;
}





