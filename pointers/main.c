#include <stdio.h>
#include <string.h>
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

unsigned int change_third_byte(const unsigned int *value, const unsigned int *new_byte){
    unsigned int result = *value;
    unsigned char *ptr_value = (unsigned char*) &result;
    // for(int i = 0; i < sizeof(unsigned int); i++){
    //     printf("value: %u, value[%d]: %u, &value[%d]: %p\n", result, i, ptr_value[i], i, (void*)&ptr_value[i]);
    // }
    // printf("\n");
    ptr_value[2] = *new_byte;
    for(int i = 0; i < sizeof(unsigned int); i++){
        printf("value: %u, value[%d]: %u\n", result, i, ptr_value[i]); //65791(10) = 0000 0000 0000 0001 0000 0000 1111 1111(2), если value = 255, а new_byte = 1
    }
    printf("\n");
    return result;
}

void print_array(const int array[], const int size){ // создается копия указателя
    for(int i = 0; i < size; i++){
        printf("%d ", *array++); // после вывода всех элементов, указатель переместится на конец массива
    }
    // for(int i = 0; i < size; i++){
    //     printf("%d ", *(array + i)); // после вывода всех элементов, указатель не переместится на конец массива, так как это rvalue
    // }

    // int *ptr_array = array; // можно поменять элемент массива, не смотря на const
    // ptr_array[9] = 54;
    // printf("\n");
    // for(int i = 0; i < size; i++){
    //     printf("%d ", *ptr_array++);
    // }
    printf("\n");
}

int my_len(const char *string){
    if(string == NULL)
        return 0;
    int len_str = 0;
    for(int i = 0; string[i] != '\0'; i++)
        len_str++;
    return len_str;
}

const char* my_strstr(const char* string, const char* sub_string){
    if (string == NULL || sub_string == NULL)
    {
        printf("Строка или подстрока пустая\n");
        return NULL;
    }
    
    int len_str = my_len(string);
    int len_sub_str = my_len(sub_string);

    printf("Длины строк:\n1: %d, 2: %d\n", len_str, len_sub_str);
    if(len_sub_str > len_str){
        printf("Подстрока длинне строки\n");
        return NULL;
    }

    const char *res = NULL;
    int count = 0;
    for(int i = 0; i < len_str; i++){
        if(string[i] == sub_string[0]){
            for(int j = i; j < len_sub_str && count < len_sub_str; j++){
                if(string[j] == sub_string[j]){
                    count++;
                    continue;
                }
                else{
                    count = 0;
                    break;
                }
            }
            res = &string[i];
        } 
    }
    return res;
}

int main(){
    printf("1. \n");
    unsigned int value;
    unsigned int new_third_byte;
    printf("Введите целое положительное число [0-4294967295]\n");
    scanf("%u", &value);
    if (value < 0 || value > 4294967295){
        printf("Вы ввели неправильное целое положительное число\n");
        return 1;
    }
    printf("Введите новый третий байт [0-255]\n");
    scanf("%d", &new_third_byte);
    if (new_third_byte < 0 || new_third_byte > 255){
        printf("Вы ввели неправильный новый байт\n");
        return 1;
    }
    change_third_byte(&value, &new_third_byte);
    printf("-------------------------------\n");

    printf("2. \n");

    float x = 5.0;
    printf("x = %f, ", x);
    float y = 6.0;
    printf("y = %f\n", y);
    float *xp = &(float){x + 1};//нужна анонимная переменная, так как взять адрес можно только у lvalue // TODO: отредактируйте эту строку, и только правую часть уравнения
    float *yp = &y;
    printf("Результат: %f\n", *xp + *yp);
    printf("-------------------------------\n");

    printf("3. \n");

    int array[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    print_array(array, ARRAY_SIZE(array));
    printf("%d\n", array[1]);
    printf("-------------------------------\n");

    printf("4. \n");

    printf("Введите строку (без пробелов): ");
    char string[100];
    scanf("%s", string); // считывает до первого пробела или до \n

    while(getchar() != '\n'); // после scanf в буфере stdin остался \n 
    char sub_string[100];
    int i = 0;
    char c;
   
    printf("Введите подстроку: ");
    
    while(i < sizeof(sub_string) - 1 && (c = getchar()) != '\n' && c != EOF) {
        sub_string[i++] = c;
    }
    sub_string[i] = '\0'; 

    const char *res = my_strstr(string, sub_string);

    if(res != NULL){
        printf("Адреса сиволов:\n");
        for(int i = 0; i < my_len(string); i++){
            printf("string[%d]: %p\n", i, &string[i]);
        }
        printf("\n");
        printf("Адрес результата: %p\n", res);

        for (int i = 0; i < my_len(sub_string); i++){
            printf("%c", res[i]);
        }
        printf("\n");
    }
    printf("-------------------------------\n");


    return 0;
}