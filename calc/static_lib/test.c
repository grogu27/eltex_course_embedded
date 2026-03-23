// #include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define CLEAR "\033[2J\033[H"
//#define HOME "\033[H"
#define RED "\033[31m"

int main() {
    // system("clear");
    // system("ls");
    //printf("\033[2J\033[H");
    // printf(RED"REDHello\n");
    // printf("helloo""wordl\n");
    // printf("%c\n", 2["ABCDE"]);
    // printf("%c\n", "ABCDE"[2]);
    //printf(CLEAR);
    //printf("hello world");
    //printf(CLEAR HOME);
    char res[100];
    printf("%s\n", getcwd(res, sizeof(res)));
    write(1, "Hello\n", 6);
    div_t result = div(10, 3);
    printf("Rs: %d(%d)\n", result.quot, result.rem);

}