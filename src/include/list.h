#ifndef LIST_H
#define LIST_H
#include <stdlib.h>

typedef struct LIST_STRUCT {
    size_t size;
    void** items;
    size_t item_size;
} list;

list* list_init(size_t item_size);
void list_add_item(list* my_list, void* item);
#endif
