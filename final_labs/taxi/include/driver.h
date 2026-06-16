#ifndef DRIVER_H
#define DRIVER_H

#include <sys/types.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/wait.h>
#include <unistd.h>
#include <poll.h>
#include <time.h>

#define MAX_DRIVERS 32

// Типы команд
enum command_type {
    CMD_TASK = 1,
    CMD_STATUS,
    CMD_EXIT
};

// Типы ответов
enum answer_type {
    ANSWER_OK = 1,
    ANSWER_AVAILABLE,
    ANSWER_BUSY,
    ANSWER_ERROR
};

// Структуры для обмена данными
struct command {
    int type;
    int timer;
};

struct answer {
    int type;
    int timer;
};

// Информация о водителе
struct driver {
    pid_t pid;
    int fd_to_driver;      // Запись в водителя
    int fd_from_driver;    // Чтение от водителя
};

// Создание водителя
int create_driver(struct driver *drivers, int *count);

// Запуск цикла водителя
void driver_loop(int read_fd, int write_fd);

#endif