#ifndef COMMANDS_H
#define COMMANDS_H

#include "driver.h"
#include "utils.h"

void handle_create_driver(struct driver *drivers, int *count);
void handle_send_task(struct driver *drivers, int count, pid_t pid, int timer);
void handle_get_status(struct driver *drivers, int count, pid_t pid);
void handle_get_drivers(struct driver *drivers, int count);
void print_help(void);

#endif