/*******************************************************************************
 * Name        : lab2.c
 * Author      : Julia Nelson
 * Date        : June 2, 2021
 * Description : Practice with pointers and strings.
 * Pledge : "I pledge my honor that I have abided by the Stevens Honor System."
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>

#define BUFLEN 128

/* Function prototypes */
size_t my_strlen(char *src);
char *my_strcpy(char *dst, char *src);

/**
 * Accepts user input from standard in, copies it to a new string, and prints
 * out the copied string and string length. You are NOT ALLOWED to use any 
 * functions from <string.h>.
 */
int main() {
    char src[BUFLEN];
    src[0] = '\0';
    char *copy;
    /*size_t length = 0;                does not work in main - put in my_strlen instead*/

    printf("Enter a string: ");

    if (scanf("%[^\n]", src) < 0) {
        fprintf(stderr, "Error: Failed to get a string from standard in.\n");
        return EXIT_FAILURE;
    }

    /* TODO: Implement the following features before the return. */

    /* 1. Using malloc, allocate enough space in 'copy' to fit 'src'.
          (man 3 malloc) */
            /***********                              
                (from man 3 malloc)
                    The  malloc()  function allocates size bytes and returns a pointer to  
                    the allocated memory.  The memory is not initialized.  If size is 0, 
                    then malloc() returns either NULL, or a unique pointer value that can 
                    later be successfully passed to free().
            ************/
    copy = (char *)malloc((my_strlen(src) + 1)); 
    if (copy == NULL) { 
        /*length = 0;
        return length;*/                    /** first attempt - used length but is unessecary duplicate usage */
        return 1;
    }          
        



    /* TODO: These function calls should not crash your program if you handled
             NULLs correctly. Do not delete them! */
    my_strlen(NULL);
    if (my_strcpy(copy, NULL)) {
        fprintf(stderr, "Error: This line should not have executed!\n");
    }

    /* 2. Call my_strcpy to make a copy of src, stored in copy. */
    my_strcpy(copy, src);

    /*
     * 3. Print out the following, matching the format verbatim:
     * Duplicated string: some_string
     * Length: some_length
     */
    printf("Duplicated string: %s\n", copy);
    printf("Length: %ld\n", my_strlen(copy));

    /* 
     * 4. Deallocate (free) any memory allocated with malloc/calloc/realloc/etc.
     */
    free(copy);

    return EXIT_SUCCESS;
}


/**
 * This function calculates the length of a character array passed in,
 * returning the length as a size_t. Calculation must be done via pointer
 * arithmetic. No credit will be given for calling strlen() in <string.h>.
 *
 * src: character array, possibly NULL (which will be considered length 0)
 *
 * return: length of src as a size_t
 */
size_t my_strlen(char *src) {
    /* TODO: Implement me! */
    size_t length = 0;          /* had inside main function but did not work*/
    /*int i = 0;*/
    if (src == NULL) {
        return 0;
    }
    while (*src != '\0') {      /* Possibly NULL not '\0' */
        src++;
        length++;
    }
    return length;
} 
/*
    **********
    initial attempts: 

    size_t length = 0;
    int i = 0;
    char *current = src;
    if (current == NULL){
        length = 0;
    }
    for (int i = 0; current != '\0'; current++ ){  
        length++;
    }
    return length;
    *******

    while (current != NULL) {
        length++;
        current = src++;
    } 
    *******

    size_t len = 0;
    if (src[0] == '\0'){
        len = 0;
    }
    for (int i = 0; src[0] != '\0'; i++ ){
        len++;
    }
    return len;

    */



/**
 * This function copies a character array src into dest. The caller must ensure
 * that the length of dst is at least length of src plus one. This function then
 * returns a pointer to dst. Computation must be done via pointer arithmetic.
 * No credit will be given for calling memcpy() in <string.h>.
 *
 * dst: target character array
 * src: source character array, possibly NULL
 *
 * return: pointer to dst
 *         If src is NULL, return NULL.
 */
char *my_strcpy(char *dst, char *src) {
    /* TODO: Implement me! 
        - if source is null then return null for the copy
    */
    /*  char *copy;  */
    if (src == NULL) {                 /* copy is null */
        return NULL;
    }
    while (*src) {                  /* changed from non-working (*src != NULL)  */
        *dst = *src;
        src++;
        dst++;
    }
    /* FIXED original mistake: using copy rather than dst 
    and was only getting back the first couple chars 

    for (int i = 0; i <= (my_strlen(src) + 1); i++) {  
        *dst = *src;
        dst++;
        src++;
    }  
    */
    return dst;
}




