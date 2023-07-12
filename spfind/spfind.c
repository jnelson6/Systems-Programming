/******************************************************************************
* Name  : spfind.c
* Author: Julia Nelson
* Date  : June 17, 2021
* Pledge: "I pledge my honor that I have abided by the Stevens Honor System."
* Description: Sorted Permission Finder in C - hw4
******************************************************************************/
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

/*
1. Submit spfind.zip.
2. Use the pfind binary.
3. Compare your output against this spfind binary.
Advice:
    1) Make sure you test on the root of the entire file system, /, a single slash. 
        This will generate lots of files that match your permissions string. 
        If it hangs, something is wrong.

    2) Use the "one parent, many children" model. 
        We did an example of that together in class, and it's in Canvas.
-------------------------
CANNOT use 
    * Do Not use 'popen()'
    * Do Not use 'fread()/fwrite()'
    * Do Not use Any other functions that use FILE streams.
CAN use 
    ***  will be working with pipes (which are raw file descriptors),
    *** using 'fork()' and 'exec*()', 
    *** using 'dup2()' to duplicate file descriptors, 
    *** and making sure to close all unused file descriptors. 
    ***  may also use read() and write().
_________________________
"Permission Find" and sorting its output. 
Linux utility - program is called 'sort'. 

*** Check out 'man 1 sort'. ******
*/

int main(int argc, char *argv[]) {

    //int pipefd[2]; /****DO I NEED THIS - man page ****/
    //char buf;     /****DO I NEED THIS - from man page ****/

    /*
    if (argc != 5) { // or if arc = 1 or 6
        printf("Usage: ./spfind -d <directory> -p <permissions string> [-h]\n"); // or do stderr?
        return EXIT_FAILURE; // or success?
    } 
    */
    if (argc == 1) { // or if arc = 1 or 6
        printf("Usage: ./spfind -d <directory> -p <permissions string> [-h]\n"); // or do stderr?
        return EXIT_FAILURE; // or success?
    } 
    if (argc == 6) { // or if arc = 1 or 6?
        printf("Usage: ./spfind -d <directory> -p <permissions string> [-h]\n"); // or do stderr?
        return EXIT_FAILURE;
    } 
    
    /*
    created 2 child processes 
        1 for pfind (executable provided)
        1 for sort 
    */
    // create 2 pipes
    int pfind_to_sort[2], sort_to_parent[2];
    pid_t pid[2];

    //pipe(pfind_to_sort);
    if( pipe(pfind_to_sort) == -1) {
        fprintf(stderr, "Error: pipe() failed.\n");                         // should these perros be stderr????
        return EXIT_FAILURE;                // exit(EXIT_FAILURE);
    }

    //pipe(sort_to_parent);
    if( pipe(sort_to_parent) == -1) {
        fprintf(stderr, "Error: pipe() failed.\n");                   // should these perros be stderr????
        return EXIT_FAILURE;                // exit(EXIT_FAILURE);
    }

    /*
    connect the standar output of pfind to the standard input of sort 
        * Using Pipe

    then connect a 2nd Pipe pipe to the standard output of sort 
        - where your parent process will read from the read-end of 
        the 'sort' pipe until it reaches end-of-file (read() returns 0), 
        print out all the text received, AND report how many lines were 
        returned.
    */

    // [0] = IN     [1] = OUT
    // pfind_to_sort
    pid[0] = fork();                                                    // wasn't working with fork inside the if statement for some reason?? 
    if (pid[0] == 0 ) {  /* in child pfind*/
        
        close(pfind_to_sort[0]);
        close(sort_to_parent[0]);
        close(sort_to_parent[1]);

        //dup2(pfind_to_sort[1], STDOUT_FILENO); // CAN I ADD AN ERROR CHECK HERE?? dup2 == -1
        if ( dup2(pfind_to_sort[1], STDOUT_FILENO) == -1) {
            return EXIT_FAILURE;
        }

        if (execv("./pfind", argv) == -1) {                             // yes or no './' ??? also ~ file path  should work 
            fprintf(stderr, "Error: pfind failed.\n"); 
            return EXIT_FAILURE;
        }
    }
    else if (pid[0] == -1 ) {     
            fprintf(stderr, "Error: fork() failed.\n");    //fork check
            return EXIT_FAILURE; 
    }


    // sort_to_parent
    pid[1] = fork();                               // wasn't working with fork inside the if statement for some reason?? 
    if (pid[1] == 0 ) {  /* in sort child  */
        close(pfind_to_sort[1]);
        if ( dup2(pfind_to_sort[0], STDIN_FILENO) == -1) {                  // == -1 ? then do somethign  --- FAILURE
            return EXIT_FAILURE;
        }

        close(sort_to_parent[0]);
        if ( dup2(sort_to_parent[1], STDOUT_FILENO) == -1 ) {               // == -1 ? then do somethign --- FAILURE
            return EXIT_FAILURE;
        }

        if (execlp("sort", "sort", NULL) == -1) {                           // "-A" ? ....... yes or no './'  or anything else ??
            fprintf(stderr, "Error: sort failed.\n"); 
            return EXIT_FAILURE;
        }
    } else if (pid[1] == -1 ) {     
        fprintf(stderr, "Error: fork() failed.\n");
        return EXIT_FAILURE; 
    }

    close(sort_to_parent[1]);
    if ( dup2(sort_to_parent[0], STDIN_FILENO) == -1) { // == -1 error check for dup2
        return EXIT_FAILURE;

    }
    // close(pfind_to_sort[1]);
    close(pfind_to_sort[0]);
    close(pfind_to_sort[1]);
    close(sort_to_parent[0]);

    /*
    You should check the return code of all the system and function calls 
    you use in your program. 
    - If any of them fails, print an error message and return EXIT_FAILURE. 

    Must verify you check return codes and take the proper action in code 

    We will check for specific output for failures that occur when exec-ing ‘pfind’ and ‘sort’. 
    If an error occurs, we expect to see

                Error: pfind failed.
                        or
                Error: sort failed.

    written to standard error.
    */

    //printf("=========== match start =============\n");        // print to help debug 

    int line_count = 0;
    char buffer[256]; // char buffer[128]; in countprocc ran into error
    while (1) {
        ssize_t count = read(STDIN_FILENO, buffer, sizeof(buffer));         
        if (count == 0) {
            if (line_count == 0) {
                printf("Total matches: %d\n", line_count);      // printing like test case
            } else {
                line_count -= 2;
                printf("Total matches: %d\n", line_count);
            }
            break;
        } else if (count == -1) {
            if (errno == EINTR) {   // er check
                continue;
            } else {
                fprintf(stderr, "Error: read() failed.\n"); 
                exit(EXIT_FAILURE);
            }
        } else {
            for (int i = 0; i < sizeof(buffer); i++) {

                if ( buffer[i] == '\n') {
                    line_count++;
                }
            }            
            write(STDOUT_FILENO, buffer, count);
        }
    }
    //printf("=========== end =============\n");

    return EXIT_SUCCESS;
}







