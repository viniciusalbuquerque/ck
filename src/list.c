#include "include/list.h"
#include <stdio.h>

list* list_init(size_t item_size) {
    printf("list_init called\n");
    list* lst = malloc(sizeof(list));
    lst->size = 0;
    lst->items = 0;
    lst->item_size = item_size;
    return lst;
}

void list_add_item(list* lst, void* item) {
    if (!item) return;
    lst->size += 1;
    if (!lst->items) {
        lst->items = calloc(1, lst->item_size);
    } else {
        lst->items = realloc(lst->items, lst->item_size * (lst->size));
    }
    lst->items[lst->size - 1] = item;
}

void* list_get_item(list* lst, int position) {
    if (position > lst->size - 1) {
        printf("Index out of bounds\n");
        exit(1);
    }
    return lst->items[position];
}
