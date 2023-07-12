/******************************************************************************
* Name  : main.c
* Author: Julia Nelson
* Date  : June 24, 2021
* Pledge: "I pledge my honor that I have abided by the Stevens Honor System."
* Description: Lab 12 - Dynamically Linked Libraries
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#define MAX_ARR_LEN 10

int main() {
    void *handle;
    int (*sum_array)(int*, const int);

    /* TODO: Dynamically open the shared library libsum.so. If it fails,
     * print the system-generated error message, and return from the program
     * with the failure exit code. */
    // similar to dl_demo.c
    if ((handle = dlopen("./libsum.so", RTLD_LAZY)) == NULL) {
        fprintf(stderr, "Error: %s.\n", dlerror());
        return EXIT_FAILURE;
    }


    /* TODO: Get a pointer to the sum_array() function we just loaded. If it
     * fails, print the system-generated error message, and return from the
     * program with the failure exit code.
     * Heavy casting is needed to avoid errors when compiling with -pedantic. */
    // similar to lines 15-19 dl_demo.c but with int casting 
    *(int **)&sum_array = dlsym(handle, "sum_array");
    if (sum_array == NULL) {
        fprintf(stderr, "Error: %s.\n", dlerror());
        return EXIT_FAILURE;
    }


    int array[MAX_ARR_LEN];
    for (int i = 0; i < MAX_ARR_LEN; i++) {
        *(array + i) = i + 1;
    }
    /* Expected output:
       sum{1..1}: 1
       sum{1..2}: 3
       sum{1..3}: 6
       sum{1..4}: 10
       sum{1..5}: 15
       sum{1..6}: 21
       sum{1..7}: 28
       sum{1..8}: 36
       sum{1..9}: 45
       sum{1..10}: 55
    */
    for (int i = 0; i < MAX_ARR_LEN; i++) {
                                              /* TODO: Call the function here. */ 
        printf("sum{%d..%d}: %d\n", *array, *(array + i), sum_array(array, i+1)); 
                                              // i+1 to get following element printed
    }

    /* TODO: Close the shared library.  If it fails, print the system-generated
     * error message, and return from the program with the failure exit code. */
    // similar to lines 25-28 dl_demo.c
    if (dlclose(handle) < 0) {
        fprintf(stderr, "Error: %s.\n", dlerror());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
