#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

#include <stdio.h>
#include <string.h>
#include "node.h"

typedef struct linked_list {
    node *head;
    node *tail;
    size_t num_nodes;
} linked_list;

/** calloc 0s out that memory  */
linked_list* create_linked_list() {
    return (linked_list *)calloc(1, sizeof(linked_list));
}

void push_back(linked_list *list, node *node) {
    if (list->head == NULL) {
        list->head = list->tail = node;
    } else {
        list->tail->next = node;
        node->prev = list->tail;
        list->tail = node;
    }
    list->num_nodes++;
}

void push_front(linked_list *list, node *node) {
    if (list->head == NULL) {
        list->head = list->tail = node;
    } else {
        list->head->prev = node;
        node->next = list->head;
        list->head = node;
    }
    list->num_nodes++;
}

size_t count_nodes(linked_list *list) {
    size_t count = 0;
    node *current = list->head;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

/** can go from tail here to find but if not doubly-linked we would go forward until 
we are n away from size using 2 pointers until the one thats going first hits null */
node *nth_from_end(linked_list *list, int n) {
    if (n > list->num_nodes) {
        return NULL;
    }
    node *current = list->tail;
    for (int i = 0; i < n; i++) {
        current = current->prev;
    }
    return current;
}

node *pop_front(linked_list *list) {
    node *cur = list->head;
    if (cur == NULL) {
        return NULL;
    }
    if (cur->next == NULL) {
        list->head = NULL;
        list->tail = NULL;
    } else {
        list->head = list->head->next;
        list->head->prev = NULL;
        cur->next = NULL;
    }
    list->num_nodes--;
    return cur;
}




node *pop_back(linked_list *list) {
    node *cur = list->tail;
    if (cur == NULL) {
        return NULL;
    }
    if (cur->prev == NULL) {
        list->head = NULL;
        list->tail = NULL;
    } else {
        list->tail = list->tail->prev;
        list->tail->next = NULL;
        cur->prev = NULL;
    }
    list->num_nodes--;
    return cur;
}










void print_list(linked_list *list, void (*print_function)(void *)) {
    putchar('[');
    node *cur = list->head;
    if (cur != NULL) {
        print_function(cur->data);
        cur = cur->next;
    }
    for ( ; cur != NULL; cur = cur->next) {
        printf(", ");
        print_function(cur->data);
    }
    puts("]");
}

#endif

















