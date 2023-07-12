/******************************************************************************
* Author: Julia Nelson
* Date: June 15, 2021
* Pledge: "I pledge my honor that I have abided by the Stevens Honor System."
* Description: Lab 7 - Pipes/popen/pclose
******************************************************************************/



    /*******************************************************************************************/
    /*******************************************************************************************/
    /***************** CORRECT ANSWER: SCREENSHOT ON JUNE 15, 2021 AT 2:30ish PM ****************/
    /*******************************************************************************************/
    /*******************************************************************************************/


/*
  A program that uses /proc/cpuinfo to display the model of the system's CPU.
  If you change 'cat' to some command not on the system, the program should silently fail and return 1.

    For example:
        on my computer I get the following output:
            $ ./cpumodel
            AMD Ryzen 9 3900X 12-Core Processor

  Submit only cpumodel.c.

*/
/*

should find name of cpu you have on processor 
basically 
cat /proc/cpuinfo

*/
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int BUFSIZE = 256;
int i;

bool starts_with(const char *str, const char *prefix) { // to be used inside loop for seeing if starts with string model name
    /* TODO:
       Return true if the string starts with prefix, false otherwise.
       Note that prefix might be longer than the string itself.
     */ 
    if (strncmp(prefix, str) > 0) { // prefix larger than str ------ CHANGED from strcmp to strncmp
            return false;
    } else {
        for (i = 0; i < strlen(prefix); i++) {       // does this have to be in an else{} ??
            if (str[i] == prefix[i]) {
                return true;
            } 
            else if (str[i] != prefix[i]) { // IS THIS FALSE NOT NECESSARY / GOING TO MESS THIS UP
                return false;
            }
        }
        return true; // else 
    }
}



int main() {
    /* TODO:
       Open "cat /proc/cpuinfo" for reading, redirecting errors to /dev/null.
       If it fails, print the string "Error: popen() failed. %s.\n", where
       %s is strerror(errno) and return EXIT_FAILURE.
     */
    char buf[BUFSIZE];
    FILE *fp = popen("cat /proc/cpuinfo", "r"); 
    // DO i need to redirect by doing "cat /proc/cpuinfo 2>/dev/null" ?????/ no clue where that should be going
    if (fp == NULL) { // or -1 ?
        fprintf(stderr, "Error: popen() failed. %s.\n", strerror(errno));
        return EXIT_FAILURE;
    } 


    /* TODO:
       Allocate an array of 256 characters on the stack.

       Use fgets to read line by line.
       If the line begins with "model name", print everything that comes after
       ": ".
       For example, with the line:
       model name      : AMD Ryzen 9 3900X 12-Core Processor
       print
       AMD Ryzen 9 3900X 12-Core Processor
       including the new line character.
       After you've printed it once, break the loop.
     */

    while (fgets(buf, BUFSIZE + 2, fp)) {   // similar to readints.c fgets()
        // replace the '\n' with '\0'.
        //  char *strchr(const char *s, int c); 
        // returns a pointer to the 1st occurrence of the character c in the string s 
        char *eoln = strchr(buf, '\n'); // "eoln" stands for "end of line"
        if (eoln == NULL) {
            // This should not happen.
            buf[BUFSIZE] = '\0';
        } else {
            * eoln = '\0';
        }

        while (starts_with(buf, "model name")) {
            int val;
            for(i = 0; i < strlen(buf); i++) { 
                if (buf[i] == ':' && buf[i+1] == ' ') {
                    val = i+2;
                    break;
                        
                    //}
                }
                
            }
            printf("%d\n", buf[val]);   // do i need to change the %s????
            printf("\n");
            break;

        }
    }
    /* TODO:
       Close the file descriptor and check the status.
       If closing the descriptor fails, print the string
       "Error: pclose() failed. %s.\n", where %s is strerror(errno) and return
       EXIT_FAILURE.
     */
    int status = pclose(fp);
    if (status == -1) {
        fprintf(stderr, "Error: pclose() failed. %s\n", strerror(errno));
        return EXIT_FAILURE;
    } 


    return !(WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS);
}


// to test 
// cat dyhsjdbhnj 2>/dev/nul