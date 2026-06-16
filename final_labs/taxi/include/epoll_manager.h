#ifndef EPOLL_MANAGER_H
#define EPOLL_MANAGER_H

#include <sys/epoll.h>
#include <errno.h>
#define MAX_EVENTS 32

int epoll_init(void);
int epoll_add_fd(int epfd, int fd, uint32_t events);
int epoll_del_fd(int epfd, int fd);
int epoll_wait_events(int epfd, struct epoll_event *events, int timeout);

#endif