#include <stdio.h>
#include "sorts.h"



static void swap(int *array, const int a, const int b) {
    int temp = *(array + a);
    *(array + a) = *(array + b);
    *(array + b) = temp;
}


static void swap2(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}


void bubble_sort(int *array, const int length) {
    int len = length;
    while (len > 0) {
        int n = 0;
        for (int i = 1; i < len; ++i) {
            if (*(array + i - 1) > *(array + i)) {
                swap(array, i - 1, i);
                n = i;
            }
        }
        len = n;
    }
}





void insertion_sort(int *array, const int length) {
    for (int i = 1; i < length; ++i) {
        int k, current = *(array + i);
        for (k = i - 1; k >= 0 && *(array + k) > current; --k) {
            *(array + k + 1) = *(array + k);
        }
        *(array + k + 1) = current;
    }
}

void display_array(int *array, const int length) {
    putchar('[');
    if (length > 0) {
        printf("%d", *array);
    }
    for (int i = 1; i < length; i++) {
        printf(", %d", *(array + i));
    }
    puts("]");
}
