#include "vector.h"
#include "common.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// Создание нового вектора
Vector* vector_create(size_t elem_size) {
    Vector *vec = (Vector*)malloc(sizeof(Vector));
    if (!vec) return NULL;
    
    vec->data = NULL;
    vec->size = 0;
    vec->capacity = 0;
    vec->elem_size = elem_size;
    
    return vec;
}

// Уничтожение вектора
void vector_destroy(Vector *vec) {
    if (vec) {
        if (vec->data) free(vec->data);
        free(vec);
    }
}

// Очистка вектора
void vector_clear(Vector *vec) {
    if (vec && vec->data) {
        vec->size = 0;
    }
}

// Получение размера
size_t vector_size(const Vector *vec) {
    return vec ? vec->size : 0;
}

// Получение вместимости
size_t vector_capacity(const Vector *vec) {
    return vec ? vec->capacity : 0;
}

// Проверка на пустоту
bool vector_empty(const Vector *vec) {
    return vec ? (vec->size == 0) : true;
}

// Указатель на данные
void* vector_data(Vector *vec) {
    return vec ? vec->data : NULL;
}

// Внутренняя функция для увеличения capacity
static int vector_grow(Vector *vec, size_t min_capacity) {
    size_t new_capacity = vec->capacity;
    if (new_capacity == 0) new_capacity = 4;
    
    while (new_capacity < min_capacity) {
        new_capacity *= 2;
    }
    
    void *new_data = realloc(vec->data, new_capacity * vec->elem_size);
    if (!new_data) return -1;
    
    vec->data = new_data;
    vec->capacity = new_capacity;
    return 0;
}

// Добавление элемента
int vector_push_back(Vector *vec, const void *element) {
    if (!vec) return -1;
    
    if (vec->size >= vec->capacity) {
        if (vector_grow(vec, vec->size + 1) != 0) return -1;
    }
    
    char *dest = (char*)vec->data + vec->size * vec->elem_size;
    memcpy(dest, element, vec->elem_size);
    vec->size++;
    
    return 0;
}

// Удаление последнего элемента
void vector_pop_back(Vector *vec) {
    if (vec && vec->size > 0) {
        vec->size--;
    }
}

// Доступ к элементу с проверкой
void* vector_at(Vector *vec, size_t index) {
    if (!vec || index >= vec->size) return NULL;
    return (char*)vec->data + index * vec->elem_size;
}

// Доступ к элементу без проверки
void* vector_get(Vector *vec, size_t index) {
    if (!vec) return NULL;
    return (char*)vec->data + index * vec->elem_size;
}

// Изменение элемента
int vector_set(Vector *vec, size_t index, const void *element) {
    if (!vec || index >= vec->size) return -1;
    void *dest = (char*)vec->data + index * vec->elem_size;
    memcpy(dest, element, vec->elem_size);
    return 0;
}

// Вставка элемента
int vector_insert(Vector *vec, size_t index, const void *element) {
    if (!vec || index > vec->size) return -1;
    
    if (vec->size >= vec->capacity) {
        if (vector_grow(vec, vec->size + 1) != 0) return -1;
    }
    
    char *data = (char*)vec->data;
    if (index < vec->size) {
        void *dest = data + (index + 1) * vec->elem_size;
        void *src = data + index * vec->elem_size;
        size_t bytes_to_move = (vec->size - index) * vec->elem_size;
        memmove(dest, src, bytes_to_move);
    }
    
    void *dest = data + index * vec->elem_size;
    memcpy(dest, element, vec->elem_size);
    vec->size++;
    
    return 0;
}

// Удаление элемента
int vector_erase(Vector *vec, size_t index) {
    if (!vec || index >= vec->size) return -1;
    
    char *data = (char*)vec->data;
    if (index < vec->size - 1) {
        void *dest = data + index * vec->elem_size;
        void *src = data + (index + 1) * vec->elem_size;
        size_t bytes_to_move = (vec->size - index - 1) * vec->elem_size;
        memmove(dest, src, bytes_to_move);
    }
    
    vec->size--;
    return 0;
}

// Резервирование памяти
int vector_reserve(Vector *vec, size_t new_capacity) {
    if (!vec || new_capacity <= vec->capacity) return 0;
    
    void *new_data = realloc(vec->data, new_capacity * vec->elem_size);
    if (!new_data) return -1;
    
    vec->data = new_data;
    vec->capacity = new_capacity;
    return 0;
}

// Изменение размера
int vector_resize(Vector *vec, size_t new_size) {
    if (!vec) return -1;
    
    if (new_size > vec->size) {
        if (new_size > vec->capacity) {
            if (vector_reserve(vec, new_size) != 0) return -1;
        }
        
        char *data = (char*)vec->data;
        void *dest = data + vec->size * vec->elem_size;
        size_t bytes_to_init = (new_size - vec->size) * vec->elem_size;
        memset(dest, 0, bytes_to_init);
    }
    
    vec->size = new_size;
    return 0;
}

// Поиск клиента в векторе
int vector_find_client(Vector *vec, uint32_t ip, uint16_t port) {
    if (!vec) return -1;
    
    ClientInfo *clients = (ClientInfo*)vec->data;
    for (size_t i = 0; i < vec->size; i++) {
        if (clients[i].ip == ip && clients[i].port == port) {
            return (int)i;
        }
    }
    
    return -1;
}