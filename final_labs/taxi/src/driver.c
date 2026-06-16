#define _POSIX_C_SOURCE 200809L

#include "../include/driver.h"
#include "../include/utils.h"

static volatile sig_atomic_t g_timer_done = 0;
static volatile sig_atomic_t g_need_stop = 0;

static void timer_handler(int signo)
{
    (void)signo;
    g_timer_done = 1;
}

static void stop_handler(int signo)
{
    (void)signo;
    g_need_stop = 1;
}

// Добавляем функцию seconds_left
static int seconds_left(time_t busy_until)
{
    time_t now = time(NULL);
    if (busy_until <= now) {
        return 0;
    }
    return (int)(busy_until - now);
}

static void send_answer(int fd, int type, int timer)
{
    struct answer answer = {type, timer};
    write_from_pipe(fd, &answer, sizeof(answer));
}

static void setup_driver_signals(void)
{
    struct sigaction sa = {0};
    
    sa.sa_handler = timer_handler;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGALRM, &sa, NULL);
    
    sa.sa_handler = stop_handler;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGTERM, &sa, NULL);
}

void driver_loop(int read_fd, int write_fd)
{
    int is_busy = 0;
    time_t busy_until = 0;
    int epoll_fd;
    struct epoll_event ev, events[1];
    
    setup_driver_signals();
    
    // Создаем epoll
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_create1");
        exit(1);
    }
    
    ev.events = EPOLLIN;
    ev.data.fd = read_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, read_fd, &ev) == -1) {
        perror("epoll_ctl");
        exit(1);
    }
    
    while (!g_need_stop) {
        if (g_timer_done) {
            g_timer_done = 0;
            is_busy = 0;
            busy_until = 0;
        }
        
        // Ожидаем события
        int nfds = epoll_wait(epoll_fd, events, 1, -1);
        if (nfds == -1) {
            if (errno == EINTR) continue;
            perror("epoll_wait");
            break;
        }
        
        if (nfds > 0 && (events[0].events & EPOLLIN)) {
            struct command command;
            ssize_t bytes = read_from_pipe(read_fd, &command, sizeof(command));
            
            if (bytes <= 0) break;
            
            // Проверяем таймер
            if (g_timer_done) {
                g_timer_done = 0;
                is_busy = 0;
                busy_until = 0;
            }
            
            if (command.type == CMD_TASK) {
                if (is_busy) {
                    send_answer(write_fd, ANSWER_BUSY, seconds_left(busy_until));
                    continue;
                }
                
                is_busy = 1;
                busy_until = time(NULL) + command.timer;
                alarm((unsigned int)command.timer);
                send_answer(write_fd, ANSWER_OK, command.timer);
                
            } else if (command.type == CMD_STATUS) {
                if (is_busy) {
                    send_answer(write_fd, ANSWER_BUSY, seconds_left(busy_until));
                } else {
                    send_answer(write_fd, ANSWER_AVAILABLE, 0);
                }
                
            } else if (command.type == CMD_EXIT) {
                break;
                
            } else {
                send_answer(write_fd, ANSWER_ERROR, 0);
            }
        }
    }
    
    close(epoll_fd);
    close(read_fd);
    close(write_fd);
    exit(0);
}

int create_driver(struct driver *drivers, int *count)
{
    int to_driver[2], from_driver[2];
    pid_t pid;
    
    if (*count >= MAX_DRIVERS) {
        printf("Too many drivers\n");
        return -1;
    }
    
    if (pipe(to_driver) == -1 || pipe(from_driver) == -1) {
        perror("pipe");
        return -1;
    }
    
    pid = fork();
    if (pid == -1) {
        perror("fork");
        close(to_driver[0]); close(to_driver[1]);
        close(from_driver[0]); close(from_driver[1]);
        return -1;
    }
    
    if (pid == 0) {
        close(to_driver[1]);
        close(from_driver[0]);
        driver_loop(to_driver[0], from_driver[1]);
    }
    
    close(to_driver[0]);
    close(from_driver[1]);
    
    drivers[*count].pid = pid;
    drivers[*count].fd_to_driver = to_driver[1];
    drivers[*count].fd_from_driver = from_driver[0];
    ++(*count);
    
    printf("Created driver %d\n", pid);
    return 0;
}