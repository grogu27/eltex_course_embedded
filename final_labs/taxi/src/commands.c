#include "../include/commands.h"
#include "../include/manager.h"

void handle_create_driver(struct driver *drivers, int *count)
{
    create_driver(drivers, count);
}

void handle_send_task(struct driver *drivers, int count, pid_t pid, int timer)
{
    struct driver *drv = find_driver(drivers, count, pid);
    if (!drv) {
        printf("Driver not found\n");
        return;
    }
    
    struct command cmd = {.type = CMD_TASK, .timer = timer};
    struct answer ans;
    
    if (ask_driver(drv, &cmd, &ans) == 0) {
        print_answer(&ans);
    }
}

void handle_get_status(struct driver *drivers, int count, pid_t pid)
{
    struct driver *drv = find_driver(drivers, count, pid);
    if (!drv) {
        printf("Driver not found\n");
        return;
    }
    
    struct command cmd = {.type = CMD_STATUS, .timer = 0};
    struct answer ans;
    
    if (ask_driver(drv, &cmd, &ans) == 0) {
        print_answer(&ans);
    }
}

void handle_get_drivers(struct driver *drivers, int count)
{
    struct command cmd = {.type = CMD_STATUS, .timer = 0};
    
    for (int i = 0; i < count; i++) {
        struct answer ans;
        printf("%d: ", drivers[i].pid);
        if (ask_driver(&drivers[i], &cmd, &ans) == 0) {
            print_answer(&ans);
        }
    }
}

void print_help(void)
{
    printf("Commands:\n");
    printf("  create_driver\n");
    printf("  send_task <pid> <task_timer>\n");
    printf("  get_status <pid>\n");
    printf("  get_drivers\n");
    printf("  exit\n");
}