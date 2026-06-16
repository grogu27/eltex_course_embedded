#ifndef MANAGER_H
#define MANAGER_H

#include "driver.h"
#include "utils.h"

int create_driver(struct driver *drivers, int *count);
int ask_driver(struct driver *driver, const struct command *cmd, struct answer *ans);
void stop_drivers(struct driver *drivers, int count);
struct driver* find_driver(struct driver *drivers, int count, pid_t pid);
void print_answer(const struct answer *ans);

#endif