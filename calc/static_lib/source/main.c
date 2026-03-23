#include <stdio.h>
#include <stdlib.h>
#include "../include/static_calc.h"
#include <unistd.h>
#define CLEAR_TERMINAL "\033[2J\033[H"
#define RED "\033[31m"

int main(){

    int op;
    long res;
    int operand2, operand1;
    while(1){
        printf("Введите номер бинарной операции [1-4]\n");
        printf("1)Сложение\n2)Вычитание\n3)Умножение\n4)Деление\n5)Очистить терминал\n6)Выход\n");
        scanf("%d", &op);

        if(op == 6){
            printf("Вы вышли из программы\n");
            break;
        }

        if(op == 5){
            printf(CLEAR_TERMINAL);
            continue;
        }
            
        if (op > 5 || op < 1){
            printf("Вы ввели неправильный номер бинарной операции. Подождите 2 секунды...\n");
            sleep(2);
            printf(CLEAR_TERMINAL);
            continue;
        }

        printf("Введите два операнда в диапазоне [-2147483648, 2147483647]\n");
        scanf("%d %d", &operand1, &operand2);
        switch(op){
            case 1:
                res = calc_add(operand1, operand2);
                printf("sum(%d,%d) = %ld\n", operand1, operand2, res);
                printf("=======================================\n");
                break;
            case 2:
                res = calc_sub(operand1, operand2);
                printf("sub(%d,%d) = %ld\n", operand1, operand2, res);
                printf("=======================================\n");
                break;
            case 3:
                res = calc_mul(operand1, operand2);
                printf("mul(%d,%d) = %ld\n", operand1, operand2, res);
                printf("=======================================\n");
                break;
            case 4:
                if(operand2 == 0){
                    printf("Ошибка: делить на 0 нельзя\n");
                    printf("=======================================\n");
                    continue;
                }
                res = calc_div(operand1, operand2);
                printf("div(%d,%d) = %ld\n", operand1, operand2, res);
                printf("=======================================\n");
                break;
            default: 
                break;
        }
        
    }
    return 0;
}
