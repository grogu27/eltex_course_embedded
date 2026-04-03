//#include "../include/telephone_directory.h"
#include "telephone_directory.h"
#include <string.h>

int print_abonent_by_id(const TelephoneDirectory *dir, size_t id){

    if (id < 1 || id > dir->size)
    {
        return 2;
    }
    
    if (dir->abonents[id-1].exist){
        printf("%zu) Фамилия: %s\tИмя: %s\tНомер телефона: %s\n", id, dir->abonents[id-1].second_name, dir->abonents[id-1].name, dir->abonents[id-1].tel);
        return 0;
    }
    else
        return 1;
    
    
}

void print_tel_directory(const TelephoneDirectory *dir){

    if (dir->size == 0){
        printf("Телефонный справочник пуст\n");
        return;
    }

    printf("Список всех абонентов:\n");
    printf("Найдено абонентов: %zu\n", dir->size);
    for (size_t i = 0; i < dir->size; i++){
        if (dir->abonents[i].exist){
            printf("%zu) Фамилия: %s\tИмя: %s\tНомер телефона: %s\n", dir->abonents[i].id, dir->abonents[i].second_name, dir->abonents[i].name, dir->abonents[i].tel);
        }
        
    }
    
}
int add_abonent(TelephoneDirectory *dir, const char *name, const char *second_name, const char *phone ){
    
    if (dir->size >= TELEPHONE_DIRECTORY_SIZE){
        return 1;
    }
    
    size_t index = dir->size;
    Abonent new_abonent = {0};
    new_abonent.id = index+1;
    strcpy(new_abonent.second_name, second_name);
    strcpy(new_abonent.name, name);
    strcpy(new_abonent.tel, phone);
    dir->abonents[index] = new_abonent;
    dir->size++;
    dir->abonents[index].exist = 1;
    return 0;
}

static void compact_tel_dir(TelephoneDirectory *dir){
    int new_index = 0;
    for (int i = 0; i < dir->size; i++){
        if (dir->abonents[i].exist){
            if(i != new_index){
                dir->abonents[new_index] = dir->abonents[i];
            }
            dir->abonents[new_index].id = new_index + 1;
            new_index++;
        }   
    }
    // for (size_t i = new_index; i < dir->size; i++) {
    //     dir->abonents[i] = (Abonent){0};
    // }
    dir->size = new_index;
}
    


int remove_abonent(TelephoneDirectory *dir, const size_t id){
    if(id < 1 || id > 99){
        return 1;
    }
    if (dir->abonents[id-1].exist == 0){
        return 1;
    }
    
    dir->abonents[id-1].exist = 0;
    //dir->size--;
    compact_tel_dir(dir);
    return 0;
}

int find_abonents_by_second_name(const TelephoneDirectory *dir, const char* second_name){
    TelephoneDirectory res_dir = {0};
    memset(&res_dir, 0, sizeof(res_dir));
    for (size_t i = 0; i < dir->size; i++){
        if(dir->abonents[i].exist){
            if (strcmp(dir->abonents[i].second_name, second_name) == 0){
                res_dir.abonents[res_dir.size] = dir->abonents[i];
                res_dir.size++;
            }
        }
    }
    if (res_dir.size == 0){
        return 1;
    }
    print_tel_directory(&res_dir);
    return 0;
    
}
