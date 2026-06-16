#ifndef UTILS_H
#define UTILS_H

#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Объявляем функции (без static)
ssize_t write_from_pipe(int fd, const void *buf, size_t count);
ssize_t read_from_pipe(int fd, void *buf, size_t count);

#endif