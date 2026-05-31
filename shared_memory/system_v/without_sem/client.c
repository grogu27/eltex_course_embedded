#include "shm.h"

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

    shm_ds = open_shm(shm_key, SHM_SIZE);

    address = attach_shm(shm_ds, NULL, 0);

    server_data = (struct data*)address;

    client_data = (struct data *)(1024 + (char *)server_data); // смещение на 1024 байта

    printf("Server pid: %d\nServer text: %s\nServer data address: %p\n", server_data->pid, server_data->text, server_data);
    printf("Client data address: %p\n", client_data);

    client_data->pid = getpid();
    snprintf(client_data->text, sizeof(client_data->text), "%s", "Hello server!");

    if(shmdt(address) < 0){
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    return 0;

}