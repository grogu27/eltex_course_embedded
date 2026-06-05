#include "abonent.h"

void print_menu_tabs(){
    printf("----Абонентский справочник [%ld]----\n", get_abonents_size());
    printf("1) Добавить абонента \n");
    printf("2) Удалить абонента \n");
    printf("3) Поиск абонентов по имени \n");
    printf("4) Вывод всех записей \n");
    printf("5) Выход \n");
}

void print_error_id(){
    printf("Ошибка, некорректный номер (1 - 5)\n");
}

void abonent_handbook(){

    uint menu_id = 0;
    system("clear"); // Создаётся дочерний процесс, и бинарная часть меняется на clear

    while(menu_id != 5) {
        char name_abonent[10] = "";
       
        print_menu_tabs();

       
        if(scanf("%u", &menu_id) != 1){
            menu_id = 0;
            clear_input();
        }
        system("clear");

        switch (menu_id)
        {
        case 1: 
            struct abonent new_abonent = {0};

            printf("name[10]: " );           
            scanf("%9s", new_abonent.name); 
            clear_input();

            printf("second_name[10]: " );   
            scanf("%9s", new_abonent.second_name); 
            clear_input();

            printf("tel[10]: " );            
            scanf("%9s", new_abonent.tel);
            clear_input();

            system("clear");

            if(!abonent_add(new_abonent)) {
                printf(" Абонент \"%s\" был добавлен\n", new_abonent.name); 
            }

            break;

        case 2: 
            printf("Введите Имя абонента: " ); scanf("%9s", name_abonent); 
            clear_input(); 

            system("clear"); 

            if(!abonent_remove(name_abonent)) {
                printf(" Абонент [id: %s] был удален\n", name_abonent);
            }

            break;
        case 3: 
            printf("Введите имя абонента: " ); scanf("%9s", name_abonent);
            clear_input(); 
            
            system("clear"); 
            abonent_find(name_abonent);
            break;
        case 4: 
            abonents_print();
            break;
        case 5: 
            abonents_clear();
            break;
        default: 
            print_error_id();
            break;
        }

    }
}