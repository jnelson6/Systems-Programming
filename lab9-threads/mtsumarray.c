/******************************************************************************
* Name  : mtsumarray.c
* Author: Julia Nelson
* Date  : June 17, 2021
* Pledge: "I pledge my honor that I have abided by the Stevens Honor System."
* Description: Lab 9 - Threads
******************************************************************************/


/* Helpful notes: 
        - pthreads on build line in makefile
        - similar to sleepingthreads examples 

        - Asks you to take in num values.... need to Malloc the array that generates into that 
        - We want to make 2 threads.... 1 will add all values at even index and other at odd
        - Work with thread_args — passing one of those into thread index we will know starting index (0 or 1)
        - Loop until end of length
        - Increment by 2 at a time in for loop
        - sum_array should just work with odd or even 
*/
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

typedef struct arg_struct {
    int start_index;
    int length;
} thread_args;

pthread_mutex_t lock;
int *array, sum = 0;

/**
 * Generates a random integer in range [low, high].
 */
int random_int_in_range(int low, int high) {
    return low + rand() % (high - low + 1);
}

/**
 * Displays an array of integers on the screen.
 */
void display_array(int *array, const int length) {
    printf("[");
    if (length > 0) {
        printf("%d", *array);
    }
    for (int *ptr = array + 1, *end = array + length; ptr < end; ptr++) {
        printf(", %d", *ptr);
    }
    printf("]\n");
}

/**
 * The function for the thread.
 * TODO:
 *   Cast the void pointer to a thread_args pointer.
 *   Create a local int variable called partial_sum to accumulate the sum.
 *   Use the thread_args to loop over the values in the array. One thread should
 *   add all the values at even indexes, the other should add all the values at
 *   odd indexes. Proper use of thread_args will enable you to write one loop.
 *   Increment the global variable sum after aquiring the lock.
 *   You must have the following error messages:
 *     "Warning: Cannot lock mutex. %s.\n"
 *     "Warning: Cannot unlock mutex. %s.\n"
 *   %s is the strerror of the return value of the corresponding functions.
 *   Exit from the thread.
 *
 *  Starting indexes is 0 and 1. Just using “start_index” don’t say "if start_index = 0 or start_index =1 ""
 *  We will just take passed in arg that has it just start at the “start_index” 
 */
void *sum_array(void *ptr) {
    // Cast the void pointer to a thread_args pointer.   -----DO I NEED THE * BEFORE thread_args ??????????????
    thread_args *arg_ptr = (thread_args *)ptr;           
    // Create a local int variable called partial_sum to accumulate the sum.
    int partial_sum = 0;                
    /*  Use the thread_args to loop over the values in the array. 
    *   One thread should add all the values at even indexes.
    *   The other should add all the values at odd indexes. 
    *   Proper use of thread_args will enable you to write one loop.
    *
    *   Starts at start_index (1 or 0)
    *   Increments by 2 so the thread starting at idx 1 will add the odd and idx will add the even 
    */
    int idx;
    for (idx = arg_ptr->start_index; idx < arg_ptr->length; idx = idx + 2) {

        // starts summing values at each index 
        partial_sum = partial_sum + array[idx];     
    }

    int retval;

    // Lock
    // You must have the following error messages: "Warning: Cannot lock mutex. %s.\n"
    if ((retval = pthread_mutex_lock(&lock)) != 0) {
        fprintf(stderr, "Warning: Cannot lock mutex. %s.\n",
                strerror(retval));
    }

    // Increment the global variable sum after aquiring the lock
    sum = sum + partial_sum;

    // Unlock 
    // You must have the following error messages: "Warning: Cannot unlock mutex. %s.\n"
    if ((retval = pthread_mutex_unlock(&lock)) != 0) {
        fprintf(stderr, "Warning: Cannot unlock mutex. %s.\n",
                strerror(retval));
    }

    // Exit from the thread.
    pthread_exit(NULL);

}// end sum_array





int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <num values>\n", argv[0]);
        return EXIT_FAILURE;
    }
    int num_values = atoi(argv[1]);
    if (num_values <= 0) {
        fprintf(stderr, "Error: Invalid number of values '%s'.\n", argv[1]);
        return EXIT_FAILURE;
    }
    srand(time(NULL));
    if ((array = (int *)malloc(num_values * sizeof(int))) == NULL) {
        fprintf(stderr,
            "Error: Cannot allocate memory for array of size '%s'. %s.\n",
            argv[1], strerror(errno));
        return EXIT_FAILURE;
    }
    for (int i = 0; i < num_values; i++) {
        array[i] = random_int_in_range(0, 9);
    }
    printf("Random Array:\n");
    display_array(array, num_values);

    int retval;
    if ((retval = pthread_mutex_init(&lock, NULL)) != 0) {
        fprintf(stderr, "Error: Cannot create mutex. %s.\n", strerror(retval));
        free(array);
        return EXIT_FAILURE;
    }

    pthread_t threads[2];
    thread_args targs[2];

    // TODO:
    // In a loop, create 2 threads:
    //   One thread should add all the values at even indexes, the other should
    //   add all the values at odd indexes.

    //   Set targs[i].start_index and targs[i].length accordingly.
    //   Create the thread, passing in targs[i].
    //   If the thread fails to be created, display the following message and
    //   exit in failure:
    //     "Error: Cannot create thread %d. %s.\n", where
    //        %d is either 1 or 2 and
    //        %s is the strerror of the return value of the corresponding
    //           function.
    //   Note: Make sure you avoid a memory leak in this section of code!

    // create 2 threads:
    //      - Loop until end of length
    //      - Increment by 2 at a time in for loop
    // attempted to use num_values like class example or just  < 2  
    for ( int i = 0; i < 2; i++) { 
        
        // Set targs[i].length accordingly.
        targs[i].length = num_values;

        // Set targs[i].start_index accordingly.
        if ( i % 2 != 0 ) { // num_values like class example or just  < 2    ??
            targs[i].start_index = 1;
        } else {
            targs[i].start_index = 0;
        }

        // following process in sleepingthreads_v2.c
        if ((retval = pthread_create(&threads[i], NULL, sum_array, (void *)(&targs[i]))) != 0) {
            fprintf(stderr, "Error: Cannot create thread %d. %s.\n",i + 1, strerror(retval));           
            break;  // or return EXIT_FAILURE ?? 
        }

    }




    for (int i = 0; i < 2; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            fprintf(stderr, "Warning: Thread %d did not join properly.\n",
                    i + 1);
        }
    }

    if ((retval = pthread_mutex_destroy(&lock)) != 0) {
        fprintf(stderr, "Error: Cannot destroy mutex. %s.\n", strerror(retval));
        free(array);
        return EXIT_FAILURE;
    }

    printf("Sum with threads: %d\n", sum);
    int sum_check = 0;
    for (int i = 0; i < num_values; i++) {
        sum_check += array[i];
    }
    printf("Sum without threads: %d\n", sum_check);

    free(array);
    return EXIT_SUCCESS;
}

/*
user@box:~/shared1/lab9$ valgrind --tool=memcheck --leak-check=yes --track-origins=yes ./mtsumarray
==1567== Memcheck, a memory error detector
==1567== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==1567== Using Valgrind-3.15.0 and LibVEX; rerun with -h for copyright info
==1567== Command: ./mtsumarray
==1567== 
Usage: ./mtsumarray <num values>
==1567== 
==1567== HEAP SUMMARY:
==1567==     in use at exit: 0 bytes in 0 blocks
==1567==   total heap usage: 0 allocs, 0 frees, 0 bytes allocated
==1567== 
==1567== All heap blocks were freed -- no leaks are possible
==1567== 
==1567== For lists of detected and suppressed errors, rerun with: -s
==1567== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
*/
