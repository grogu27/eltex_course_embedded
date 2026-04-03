//#include "../include/telephone_directory.h"
#include "telephone_directory.h"
#include <unistd.h>
#include <string.h>
//#include <stdlib.h>
#define CLEAR_TERMINAL "\033[2J\033[H"
#define RED "\033[31m"

#define DASHES "---------------------------\n"

void clear_input_buffer(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF){
    }
    
}

int read_string(char *str, size_t size)
{
    if (fgets(str, size, stdin) == NULL) {
        return 1;
    }

    int index = -1;
    for (int i = 0; i < size; i++){
        if (str[i] == '\n'){
            index = i;
        }  
    }

    if (index != -1){
        str[index] = '\0';
        return 0;
    }
    
    // char *newline = strchr(str, '\n');
    // if (newline) {
    //     *newline = '\0';
    //     return 0;
    // }

    clear_input_buffer();
    return 2; // слишком длинная строка
}

typedef enum{
    MENU_ADD = 1,
    MENU_DELETE = 2,
    MENU_SEARCH = 3,
    MENU_SHOW_ALL = 4,
    MENU_SHOW_ABONENT = 5,
    MENU_CLEAR = 6,
    MENU_EXIT = 7

} MenuAction;

TelephoneDirectory dir = {0};


int main(){
    printf("Абонентский справочник\n\n");
    printf(DASHES);

    int op;
    char tail;
    dir.size = 0;
    while(1){
        printf("Введите пункт меню [1-4]\n");
        printf("1) Добавить абонента\n2) Удалить абонента\n3) Поиск абонентов по фамилии\n4) Вывод всех записей \n5) Вывод абонента \n6) Очистить терминал\n7) Выход\n");
        if(scanf("%d%c", &op, &tail) != 2 || tail != '\n'){
            printf("Ошибка ввода: нужно ввести число [1-6]\n");
            clear_input_buffer();
            sleep(2);
            printf(CLEAR_TERMINAL);
            continue;
        }

        if(op == MENU_CLEAR){
            printf(CLEAR_TERMINAL);
            continue;
        }

        if (op == MENU_EXIT){
            printf("Вы вышли из программы\n");
            break;

        }

        if (op < 1 || op > 6){   
            printf("Вы ввели неверный пункт меню. Подождите 2 секунды...\n");
            clear_input_buffer();
            sleep(2);
            printf(CLEAR_TERMINAL);
            continue;
        }
        
        switch (op)
        {
            case MENU_ADD:
            {
                printf("Введите фамилию (максимальная длина 19 символов):");
                char second_name[20];
                char phone[20];
                char name[20];
                
                // if(fgets(second_name, sizeof(second_name), stdin) == NULL){
                //     printf("Ошибка ввода\n");
                //     sleep(2);
                //     printf(CLEAR_TERMINAL);
                //     continue;
                // }
                // second_name[strcspn(second_name, "\n")] = '\0';
                // clear_input_buffer();

                int res_second_name = read_string(second_name, sizeof(second_name));
                if(res_second_name == 1){
                    printf("Ошибка ввода\n");
                    clear_input_buffer();
                    sleep(2);
                    printf(CLEAR_TERMINAL);
                    continue;
                }
                if(res_second_name == 2){
                    printf("Строка слишком длинная\n");
                    clear_input_buffer();
                    sleep(2);
                    printf(CLEAR_TERMINAL);
                    continue;
                }

                printf("Введите имя (максимальная длина 19 символов):");
                int res_name = read_string(name, sizeof(name));
                if(res_name == 1){
                    printf("Ошибка ввода\n");
                    clear_input_buffer();
                    sleep(2);
                    printf(CLEAR_TERMINAL);
                    continue;
                }
                if(res_name == 2){
                    printf("Строка слишком длинная\n");
                    clear_input_buffer();
                    sleep(2);
                    printf(CLEAR_TERMINAL);
                    continue;
                }
                // if(fgets(name, sizeof(name), stdin) == NULL){
                //     printf("Ошибка ввода\n");
                //     sleep(2);
                //     printf(CLEAR_TERMINAL);
                //     continue;
                // }
                // name[strcspn(name, "\n")] = '\0';
                // clear_input_buffer();

                printf("Введите номер телефона (максимальная длина 19 символов):");
                int res_phone = read_string(phone, sizeof(phone));
                if(res_phone == 1){
                    printf("Ошибка ввода\n");
                    clear_input_buffer();
                    sleep(2);
                    printf(CLEAR_TERMINAL);
                    continue;
                }
                else if(res_phone == 2){
                    printf("Строка слишком длинная\n");
                    clear_input_buffer();
                    sleep(2);
                    printf(CLEAR_TERMINAL);
                    continue;
                }
                // if(fgets(phone, sizeof(phone), stdin) == NULL){
                //     printf("Ошибка ввода\n");
                //     sleep(2);
                //     printf(CLEAR_TERMINAL); 
                //     continue;
                // }
                // phone[strcspn(phone, "\n")] = '\0';
                // clear_input_buffer();

                if(add_abonent(&dir, name, second_name, phone)){
                    printf("Телефонный справочник переполнен\n");
                    printf(DASHES);
                    continue;
                }
                printf("Абонент добавлен\n");
                printf(DASHES);
                break;
            }
            case MENU_DELETE:
            {
                printf("Введите id абонента которого надо удалить [1-100]:");
                size_t id;
                char tail;
                if (scanf("%zu%c", &id, &tail) != 2 || tail != '\n'){   
                    printf("Ошибка ввода\n");
                    clear_input_buffer();
                    sleep(2);
                    printf(CLEAR_TERMINAL);
                    continue;
                }

                int res = remove_abonent(&dir, id);
                if(res == 1){
                    printf("Абонент с id:%zu не существует или уже удалён\n", id);
                    printf(DASHES);
                    continue;
                }
                else if (res == 2){
                    printf("Неверный id\n");
                    printf(DASHES);
                    continue;
                }
                
                printf("Абонент с id:%zu удален\n", id);
                printf(DASHES);
                break;
            }
            case MENU_SEARCH:
            {
                printf("Введите фамилию (максимальная длина 19 символов):");
                char second_name[20];
                if (fgets(second_name, sizeof(second_name), stdin) == NULL){   
                    printf("Ошибка ввода\n");
                    clear_input_buffer();
                    sleep(2);
                    printf(CLEAR_TERMINAL);
                    continue;
                }
                second_name[strcspn(second_name, "\n")] = '\0';
                //clear_input_buffer();
                
                if (find_abonents_by_second_name(&dir, second_name)){
                    printf("Абоненты с такой фамилией не найдены\n");
                    printf(DASHES);
                    continue;
                }
                printf(DASHES);
                break;
            }
            case MENU_SHOW_ALL:
                print_tel_directory(&dir);
                printf(DASHES);
                break;
            case MENU_SHOW_ABONENT:
            {
                printf("Введите id абонента которого надо вывести в терминал [1-100]:");
                size_t id;
                char tail;
                if (scanf("%zu%c", &id, &tail) != 2 || tail != '\n'){   
                    printf("Ошибка ввода\n");
                    clear_input_buffer();
                    sleep(2);
                    printf(CLEAR_TERMINAL);
                    continue;
                }
                int res = print_abonent_by_id(&dir, id);
                if(res == 1){
                    printf("Абонента с таким id не найден или его не существует\n");
                    printf(DASHES);
                    continue;
                }
                else if (res == 2){
                    printf("Неверный id\n");
                    printf(DASHES);
                    continue;
                }
                
                printf(DASHES);
                break;
            }
            default:
                break;
            }
    
    }
 



    return 0;
}