#ifndef ABONENT_H
#define ABONENT_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "utils.h"

/*
   
  
   Хранит имя, фамилию и телефон (char [10]) 
   с учетом '\0' всего 9 символов на каждое поле
*/
struct abonent {
    char name[10];
    char second_name[10];
    char tel[10];
}; 

/*
   
  
  Двухсвязный список, хранит абонента, указатель на предыдущий узел
и указатель на следующий
*/
struct node {
    struct abonent data;
    struct node *next;
    struct node *prev;
};

void abonent_handbook();

bool abonent_add(struct abonent data);

bool abonent_remove(char name[10]);

void abonent_find(char name[10]);

void abonents_print();

size_t get_abonents_size();

void abonents_clear();

#endif 