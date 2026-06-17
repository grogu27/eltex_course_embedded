#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct {
    void *data;          // Указатель на массив данных
    size_t size;         // Текущее количество элементов
    size_t capacity;     // Вместимость
    size_t elem_size;    // Размер одного элемента
} Vector;

// Создание и уничтожение
Vector* vector_create(size_t elem_size);
void vector_destroy(Vector *vec);
void vector_clear(Vector *vec);

// Доступ к информации
size_t vector_size(const Vector *vec);
size_t vector_capacity(const Vector *vec);
bool vector_empty(const Vector *vec);
void* vector_data(Vector *vec);

// Работа с элементами
int vector_push_back(Vector *vec, const void *element);
void vector_pop_back(Vector *vec);
void* vector_at(Vector *vec, size_t index);
void* vector_get(Vector *vec, size_t index);
int vector_set(Vector *vec, size_t index, const void *element);
int vector_insert(Vector *vec, size_t index, const void *element);
int vector_erase(Vector *vec, size_t index);

// Управление памятью
int vector_reserve(Vector *vec, size_t new_capacity);
int vector_resize(Vector *vec, size_t new_size);

// Поиск (для нашего конкретного типа)
int vector_find_client(Vector *vec, uint32_t ip, uint16_t port);

#endif 