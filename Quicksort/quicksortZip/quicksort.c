/*******************************************************************************
 * Name        : quicksort.c
 * Author      : Julia Nelson
 * Date        : June 5, 2021
 * Description : Quicksort implementation.(the source file w/ implementation of functions contained within the header)
 * Pledge      : "I pledge my honor that I have abided by the Stevens Honor System."
 ******************************************************************************/
#include <stdio.h>
#include <string.h>
#include "quicksort.h"

/* Static (private to this file) function prototypes. */
static void swap(void *a, void *b, size_t size);
static int lomuto(void *array, int left, int right, size_t elem_sz,
                  int (*comp) (const void*, const void*));
static void quicksort_helper(void *array, int left, int right, size_t elem_sz,
                             int (*comp) (const void*, const void*));

/**
 * Compares two integers passed in as void pointers and returns an integer
 * representing their ordering.
 * First casts the void pointers to int pointers.
 * Returns:
 * -- 0 if the integers are equal
 * -- a positive number if the first integer is greater
 * -- a negative if the second integer is greater
 */
int int_cmp(const void *a, const void *b) {
    // TODO
    return *(const int *)a - *(const int *)b;
}

/**
 * Compares two doubles passed in as void pointers and returns an integer
 * representing their ordering.
 * First casts the void pointers to double pointers.
 * Returns:
 * -- 0 if the doubles are equal
 * -- 1 if the first double is greater
 * -- -1 if the second double is greater
 */
int dbl_cmp(const void *a, const void *b) {
    // TODO
    int tmp;
    if (*(const double *)a > *(const double *)b) {
      tmp = 1;
    }
    if (*(const double *)a < *(const double *)b) {
      tmp = -1;
    }
    if ((*(const double *)a - *(const double *)b) == 0) {
      tmp = 0;
    }
    return tmp;
}

/**
 * Compares two char arrays passed in as void pointers and returns an integer
 * representing their ordering.
 * First casts the void pointers to char* pointers (i.e. char**).
 * Returns the result of calling strcmp on them.
 */
int str_cmp(const void *a, const void *b) { 
    return strcmp(*(const char **)a, *(const char **)b);
}

/**
 * Swaps the values in two pointers.
 *
 * Casts the void pointers to character types and works with them as char
 * pointers for the remainder of the function.
 * Swaps one byte at a time, until all 'size' bytes have been swapped.
 * For example, if ints are passed in, size will be 4. Therefore, this function
 * swaps 4 bytes in a and b character pointers.
 */
static void swap(void *a, void *b, size_t size) {
  // TODO
    char *a_ptr = (char *)a;
    char *b_ptr = (char *)b;
    int i = 0;
    char tmp;
    while(i < size) {   /// SHOULD I ADD IN +i to the pointers
        tmp = *a_ptr;
        *a_ptr = *b_ptr;
        *b_ptr = tmp;
        ++*a_ptr;
        ++*b_ptr;
        i++;
    }
    return;
}

/**
 * Partitions array around a pivot, utilizing the swap function.
 * Each time the function runs, the pivot is placed into the correct index of
 * the array in sorted order. All elements less than the pivot should
 * be to its left, and all elements greater than or equal to the pivot should be
 * to its right.
 * The function pointer is dereferenced when it is used.
 * Indexing into void *array does not work. All work must be performed with
 * pointer arithmetic.
 */
static int lomuto(void *array, int left, int right, size_t elem_sz,
                  int (*comp) (const void*, const void*)) {
    // TODO
    char* ar = (char *)array;                             // this right may be left***********************
    char* pivot = ar + (left * elem_sz);    //sets the pivot p to be the left index of the inputed array
    size_t s = left;        // set initial output to be the left index 
    // increments the index counter var to help track which index were on for lomuto partition
    for(int i = left + 1; i <= right; i++){   /*   inx = left +1 maybe*/
        
        // if the value of element in the current index is < the pivot value (in left index of input array)
        if ( comp( ar + (i * elem_sz), pivot) < 0) {
            s++;                            // shift the new position s up one index
            swap( ar+ (s * elem_sz),  ar + (i * elem_sz), elem_sz);   // then swap the values in the new position s and the current index 
                                            // we are working on of the array 
        }
    }
    /// Possibly change to left not right
    swap( ar + (left * elem_sz),  ar + (s * elem_sz), elem_sz);  // finally swap the values in index left and s 
    return s; 
}

/**
 * Sorts with lomuto partitioning, with recursive calls on each side of the
 * pivot.
 * This is the function that does the work, since it takes in both left and
 * right index values.
 */
static void quicksort_helper(void *array, int left, int right, size_t elem_sz, int (*comp) (const void*, const void*)) {
    // TODO
    while (left < right) {

    int s = lomuto(array, left, right, elem_sz, comp);
    quicksort_helper(array, left, s - 1, elem_sz, comp);
    quicksort_helper(array, s + 1, right, elem_sz, comp);
    }

}

/**
 * Quicksort function exposed to the user.
 * Calls quicksort_helper with left = 0 and right = len - 1.
 */
void quicksort(void *array, size_t len, size_t elem_sz, int (*comp) (const void*, const void*)) {
    // TODO
    quicksort_helper(array, 0, len - 1, elem_sz, comp);
}


