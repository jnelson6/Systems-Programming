
/**
* Author: Julia Nelson
* Date: 06/04/2021
* Pledge: "I pledge my honor that I have abided by the Stevens Honor System."
* Description: Lab 3 - Doubly Linked List
**/
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

linked_list* create_linked_list() {
    return (linked_list *)calloc(1, sizeof(linked_list));
}

/**
 * Inserts a non-NULL node into a non-NULL linked list.
 * The node is inserted so that the data in the linked list is in
 * non-decreasing order.
 * A node with a value already in the list is inserted AFTER the node(s)
 * already in the list.
 */
void insert_in_order(linked_list *list, node *n, int (*cmp)(const void*, const void*)) {
    // TODO
    // check that node is not null
    // checks that linked-list is not null
    // check what head of list is 
            // if n >= head .. push onto back 
            // if n < head push onto front 

    // if the list is empty make the head and tail =  n, initialize next
    if (list->head == NULL) { 
        list->head = n;
        list->tail = n;
        list->num_nodes++;
                    // return EXIT_SUCCESS ????

    } else if ( cmp(list->head->data, n->data) > 0)  { //  If n < head insert at head
        // push n to head of list
        list->head->prev = n;
        n->next = list->head;
        list->head = n;
        list->num_nodes++;
    } else { // push n to tail of list
        list->tail->next = n;
        n->prev = list->tail;
        list->tail = n;
        list->num_nodes++;
    }
}



void print_list(linked_list *list, void (*print_function)(void*)) {
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
    printf("]\n{length: %lu, head->data: ", list->num_nodes);
    list->head != NULL ? print_function(list->head->data) :
                         (void)printf("NULL");
    printf(", tail->data: ");
    list->tail != NULL ? print_function(list->tail->data) :
                         (void)printf("NULL");
    printf("}\n\n");
}

/**
 * Frees a list starting from the tail.
 * This will check if your previous pointers have been set up correctly.
 */
void free_list(linked_list *list, void (*free_data)(void *)) {
    while (list->tail != NULL) {
        node *prev = list->tail->prev;
        free_node(list->tail, free_data);
        list->tail = prev;
    }
}

#endif
