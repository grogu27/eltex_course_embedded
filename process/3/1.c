#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
int main() {
    const char *str = "15abc12345";
    char *end;
    long val = strtol(str, &end, 10);
    
    printf("Число: %ld\n", val);       // Выведет 12345
    printf("Остаток: %s\n", end);     // Выведет "abc"
    for (size_t i = 0; str[i] != '\0'; i++)
    {
        printf("%c", str[i]);
    }
    printf("\n");


    char *args[10] = {"ls", "-l", NULL};
    args[2] = "-v";
    args[sizeof(args) / sizeof(args[0]) - 1 ] = "-v2";
    for (int i = 0; i < sizeof(args) / sizeof(args[0]) - 1 + 1; i++)
    {if (args[i] != NULL)
        printf("%s\n", args[i]);
    }
    // args[sizeof(args) / sizeof(args[0]) + 1] = NULL;
    // for (int i = 0; i < sizeof(args) / sizeof(args[0]) - 1; i++)
    // {
    //     printf("%s\n", args[i]);
    // }
    
    return 0;
}
