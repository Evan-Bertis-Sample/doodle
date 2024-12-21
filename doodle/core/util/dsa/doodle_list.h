#ifndef __DOODLE_LIST_H__
#define __DOODLE_LIST_H__

#include "doodle/core/util/dsa/doodle_container.h"

typedef struct doodle_list {
    void *__data;
    doodle_container_size_t __size;
    doodle_container_size_t __capacity;
    doodle_container_size_t __element_size;
} doodle_list_t;

#define doodle_list_create(type)       \
    (doodle_list_t) {                  \
        .__data = NULL,                \
        .__size = 0,                   \
        .__capacity = 0,               \
        .__element_size = sizeof(type) \
    }

void doodle_list_clear(doodle_list_t *list);
void doodle_list_destroy(doodle_list_t *list);

void __doodle_list_at(doodle_list_t *list, doodle_container_size_t index);
void __doodle_list_push_back(doodle_list_t *list, void *element);
void __doodle_list_grow(doodle_list_t *list);
void __doodle_list_shrink(doodle_list_t *list)

#define doodle_list_at(list, type, index) \
    (type *)__doodle_list_at(list, index)

#define doodle_list_push_back(list, type, element) \
    __doodle_list_push_back(list, (void *)&element)

#endif  // __DOODLE_LIST_H__