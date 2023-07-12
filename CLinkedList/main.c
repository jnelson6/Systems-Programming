#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "linked_list.h"

void print_str(void *str) {
    printf("\"%s\"", (char *)str);
}

void insert_word(linked_list *list, char *word, bool at_end) {
    if (at_end) {
        push_back(list, create_node((void *)strdup(word)));
        /** taking in pointer to char *word then calling strdup mallocs 
        memory and copies chars of string over and null terminates it for us. 
        casted to void star to put that data into a node. */
    } else {
        push_front(list, create_node((void *)strdup(word)));
    }
}

int main() {
    linked_list *list = create_linked_list();
    insert_word(list, "hello", true);
    insert_word(list, "CS392", true);
    insert_word(list, "Dr. B", false);
    insert_word(list, "Kevin", true);
    /**can add any names or phrases in */

    print_list(list, print_str);

    printf("Num nodes: %ld\n", count_nodes(list));

    /**NEED TO ADD STUFF TO THIS ONE*/
    for (int i = 0; i < 10; i++) {
        node *current = nth_from_end(list, i);
        if (current) {
            printf("%d-th node from end contains %s.\n", i, (char *)current->data);
        } else {
            printf("%d-th node from end is null.\n", i);
        }
    }
    /** while look calls pop_front that makes pointer to node but detaches 
    it from the list and then can print data and then free up that node */
    while (list->num_nodes > 0) {
        node *front = pop_back(list);
        printf("Popped value: %s\n", (char *)front->data);
        free_node(front, free);
    }
   
    free(list);
    return 0;
}
