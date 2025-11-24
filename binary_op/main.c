#include <stdio.h>

void print_bits(int arr[]){
    for (int i = 0; i < 32; i++){
        if (i % 4 == 0 && i != 0)
            printf(" ");
        printf("%d", arr[i]);
    }
    printf("\n");
}

// void print_bin_without_bin_op( unsigned int val){
//     if (val == 0) {
//         printf("0\n");
//         return;
//     }
//     int bits[32] = {0};
//     for (int i = 31; i >= 0; i--){
//         if (val == 0)
//             break;
//         bits[i] = val % 2;
//         val /= 2;
//     }
//     print_bits(bits);
// }

void print_bin_with_bin_op(unsigned int val){
    if (val == 0) {
        printf("0\n");
        return;
    }
    int bits[32] = {0};
    for (int i = 31; i >= 0 && val; i--){
        bits[i] = (val & 1);   
        // if (val == 0)
        //     break;
        val >>= 1;
    }
    print_bits(bits);
}

void print_dop_code_with_bin_op(int val){
    int bits[32];
    for (int i = 31; i >= 0; i--){
        bits[i] = val & 1;   
        val >>= 1;
    }
    print_bits(bits);
    
}

unsigned int count_ones(unsigned int n){  // 0b1 & 0b1 -> 1; 0b10 & 0b1 -> 0;  
    if (n == 0)
        return 0;
    unsigned int count = 0;
    while(n){
        if (n & 1)
            count++;    
        n >>= 1;
    }
    return count;
}

void change_third_byte_in_num(int *val, int new_byte){
    int bits[8] = {0};
    for(int i = 7; i >= 0 && new_byte; i--){
        bits[i] = (new_byte & 1);
        new_byte >>= 1;
    }
    // for (int i = 0; i < 8; i++){
    //     if (i % 4 == 0 && i != 0)
    //         printf(" ");
    //     printf("%d", bits[i]);
    // }
    // printf("\n");
    *val &= ~(0xFF << 16); //зануляем 3 байт

    for(int i = 0; i < 8; i++){
        *val |= (bits[i] << (23 - i));
    }
}

// void print_binary(unsigned int n) {
//     for (int i = 31; i >= 0; i--) {
//         printf("%d", (n >> i) & 1);
//     }
// }

// void print_bin_without_bin_op_signed(int val){
//     if (val == 0) {
//         printf("0\n");
//         return;
//     }
//     int bits[32] = {0};
//     for (int i = 31; i >= 0; i--){
//         if (val == 0)
//             break;
//         bits[i] = val % 2;
//         val /= 2;
//         
//     }
//     print_bits(bits);
// }

// void print_bin_without_bin_op_recursive(unsigned int val) {
//     if (val > 1) {
//         print_bin_without_bin_op_recursive(val / 2);
//     }
//     printf("%d", val % 2);
// }

int main(int argc, char* argv[]){
    printf("Выберите номер задания [1-4]\n");
    int task;
    scanf("%d", &task);
    if (task < 1 || task > 4){
        printf("Вы ввели неправильный номер задания\n");
        return 1;
    }
    switch (task)
    {
    case  1:
        unsigned int num;
        printf("Введите целое положительное число [0-4294967295]\n");
        scanf("%u", &num);
        if (num < 0 || num > 4294967295){
            printf("Вы ввели неправильное целое положительное число\n");
            return 1;
        }
        print_bin_with_bin_op(num);
        break;
    case 2:
        int num2;
        printf("Введите целое отрицательное число [-2147483648 - -1]\n");
        scanf("%d", &num2);
        if (num2 > 0 || num2 < -2147483648){
            printf("Вы ввели неправильное целое отрицательное число\n");
            return 1;
        } 
        print_dop_code_with_bin_op(num2);       
        break;
    case 3:
        unsigned int num3;
        printf("Введите целое положительное число [0-4294967295]\n");
        scanf("%u", &num3);
        if (num3 < 0 || num3 > 4294967295){
            printf("Вы ввели неправильное целое положительное число\n");
            return 1;
        }
        unsigned int res = count_ones(num3);
        printf("Количество единиц в числе %u: %u\n", num3, res);
        break;
    case 4:
        int num4;
        int new_byte;
        printf("Введите целое положительное число [0-2147483647]\n");
        scanf("%d", &num4);
        if (num4 < 0 || num4 > 2147483647){
            printf("Вы ввели неправильное целое положительное число\n");
            return 1;
        }

        printf("Введите новый третий байт [0-255]\n");
        scanf("%d", &new_byte);
        if (new_byte < 0 || new_byte > 255){
            printf("Вы ввели неправильный новый байт\n");
            return 1;
        }
        printf("Новый третий байт в двоичой системе счисления: ");
        print_bin_with_bin_op(new_byte);
        printf("Изначальное число в двоичой системе счисления: ");
        print_bin_with_bin_op(num4);
        printf("Изначальное число в двоичой системе счисления после смены третьего байта: ");
        change_third_byte_in_num(&num4, new_byte);
        print_bin_with_bin_op(num4);
        printf("Измененное число: %d\n", num4);

        break;
    
    default:
        break;
    }

    return 0;
}