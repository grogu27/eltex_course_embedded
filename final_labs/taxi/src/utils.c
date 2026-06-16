#include "../include/utils.h"

ssize_t write_from_pipe(int fd, const void *buf, size_t count)
{
    const char *ptr = buf;
    while (count > 0) {
        ssize_t written = write(fd, ptr, count);
        if (written == -1) {
            if (errno == EINTR) continue;
            return -1;
        }
        ptr += written;
        count -= written;
    }
    return 0;  // Возвращаем 0 при успехе
}

ssize_t read_from_pipe(int fd, void *buf, size_t count)
{
    char *ptr = buf;
    while (count > 0) {
        ssize_t bytes_read = read(fd, ptr, count);
        if (bytes_read == 0) return 0;
        if (bytes_read == -1) {
            if (errno == EINTR) continue;
            return -1;
        }
        ptr += bytes_read;
        count -= bytes_read;
    }
    return 1;  // Возвращаем 1 при успешном чтении
}