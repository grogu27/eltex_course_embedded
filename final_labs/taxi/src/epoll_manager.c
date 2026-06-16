#include "../include/epoll_manager.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int epoll_init(void)
{
    int epfd = epoll_create1(0);
    if (epfd == -1) {
        perror("epoll_create1");
        return -1;
    }
    return epfd;
}

int epoll_add_fd(int epfd, int fd, uint32_t events)
{
    struct epoll_event ev;
    ev.events = events;
    ev.data.fd = fd;
    
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
        perror("epoll_ctl ADD");
        return -1;
    }
    return 0;
}

int epoll_del_fd(int epfd, int fd)
{
    if (epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL) == -1) {
        perror("epoll_ctl DEL");
        return -1;
    }
    return 0;
}

int epoll_wait_events(int epfd, struct epoll_event *events, int timeout)
{
    int nfds = epoll_wait(epfd, events, MAX_EVENTS, timeout);
    if (nfds == -1) {
        if (errno == EINTR) return 0;
        perror("epoll_wait");
        return -1;
    }
    return nfds;
}