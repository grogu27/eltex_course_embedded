#include "../include/manager.h"
#include "../include/driver.h"
#include "../include/utils.h"
#include <sys/wait.h>

struct driver* find_driver(struct driver *drivers, int count, pid_t pid)
{
    for (int i = 0; i < count; i++) {
        if (drivers[i].pid == pid) return &drivers[i];
    }
    return NULL;
}

int ask_driver(struct driver *driver, const struct command *cmd, struct answer *ans)
{
    if (write_from_pipe(driver->fd_to_driver, cmd, sizeof(*cmd)) == -1) {
        perror("write_from_pipe");
        return -1;
    }
    
    ssize_t bytes = read_from_pipe(driver->fd_from_driver, ans, sizeof(*ans));
    if (bytes <= 0) {
        perror("read_from_pipe");
        return -1;
    }
    
    return 0;
}

void print_answer(const struct answer *ans)
{
    switch (ans->type) {
        case ANSWER_OK:
            printf("Task accepted\n");
            break;
        case ANSWER_AVAILABLE:
            printf("Available\n");
            break;
        case ANSWER_BUSY:
            printf("Busy %d\n", ans->timer);
            break;
        case ANSWER_ERROR:
            printf("Error\n");
            break;
        default:
            printf("Unknown answer\n");
    }
}

void stop_drivers(struct driver *drivers, int count)
{
    struct command cmd = {.type = CMD_EXIT, .timer = 0};
    
    for (int i = 0; i < count; i++) {
        write_from_pipe(drivers[i].fd_to_driver, &cmd, sizeof(cmd));
        close(drivers[i].fd_to_driver);
        close(drivers[i].fd_from_driver);
        waitpid(drivers[i].pid, NULL, 0);
    }
}