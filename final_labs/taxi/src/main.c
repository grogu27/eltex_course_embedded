#include "../include/utils.h"
#include "../include/commands.h"
#include "../include/manager.h"

#define MAX_LINE 512

int main(void)
{
    struct driver drivers[MAX_DRIVERS];
    int drivers_count = 0;
    char line[MAX_LINE];
    
    print_help();
    
    while (1) {
        char command[MAX_LINE];
        int pid, timer;
        
        printf("taxi> ");
        fflush(stdout);
        
        if (!fgets(line, sizeof(line), stdin)) break;
        if (sscanf(line, "%127s", command) != 1) continue;
        
        if (strcmp(command, "create_driver") == 0) {
            handle_create_driver(drivers, &drivers_count);
            
        } else if (strcmp(command, "send_task") == 0) {
            if (sscanf(line, "%*s %d %d", &pid, &timer) != 2 || timer <= 0) {
                printf("Usage: send_task <pid> <task_timer>\n");
                continue;
            }
            handle_send_task(drivers, drivers_count, pid, timer);
            
        } else if (strcmp(command, "get_status") == 0) {
            if (sscanf(line, "%*s %d", &pid) != 1) {
                printf("Usage: get_status <pid>\n");
                continue;
            }
            handle_get_status(drivers, drivers_count, pid);
            
        } else if (strcmp(command, "get_drivers") == 0) {
            handle_get_drivers(drivers, drivers_count);
            
        } else if (strcmp(command, "help") == 0) {
            print_help();
            
        } else if (strcmp(command, "exit") == 0) {
            break;
            
        } else {
            printf("Unknown command: %s\n", command);
        }
    }
    
    stop_drivers(drivers, drivers_count);
    return 0;
}