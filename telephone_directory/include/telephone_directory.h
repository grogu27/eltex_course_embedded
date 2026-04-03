#ifndef TELEPHONE_DIRECTORY_H
#define TELEPHONE_DIRECTORY_H

#include<stdio.h>
#define TELEPHONE_DIRECTORY_SIZE 100
typedef struct  {
    char name[20];
    char second_name[20];
    char tel[20];
    int exist; // 0 or 1
    size_t id;
} Abonent;

typedef struct {
    Abonent abonents[TELEPHONE_DIRECTORY_SIZE];
    size_t size;
} TelephoneDirectory;

int add_abonent(TelephoneDirectory *dir, const char *name, const char *second_name, const char *phone);
void print_tel_directory(const TelephoneDirectory *dir);
int print_abonent_by_id(const TelephoneDirectory *dir, size_t id);
int remove_abonent(TelephoneDirectory *dir, const size_t id);
int find_abonents_by_second_name(const TelephoneDirectory *dir, const char* second_name);
//int find_abonent_by_id(const TelephoneDirectory *dir, const int id);
//int change_abonent_phone(TelephoneDirectory *dir, const char *name, const char *second_name,  const char *phone, const char *new_phone);
//int change_abonent_phone_by_id(TelephoneDirectory *dir, const int id, const char *new_phone);
//int change_abonent_name(TelephoneDirectory *dir, const char *name, const char *second_name,  const char *phone, const char *new_name, const char *new_second_name);
//int change_abonent_name_by_id(TelephoneDirectory *dir, const int id, const char *new_name, const char *new_second_name);




#endif