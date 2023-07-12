/******************************************************************************
* Name 	: spfind.c
* Author: Julia Nelson
* Date	: June 17, 2021
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

"Permission Find" and sorting its output. 
Linux utility - program is called 'sort'. 

*** Check out 'man 1 sort'. ******


use exec(), in order to NOT create a whole new 'sort' utility. 

To utilize this tool, 
	use combination of 
		- fork(), 
		- exec*() (choose your favorite flavor), &
		- pipe().
____________1 _________
created 2 child processes 
	1 for pfind (executable provided)
	1 for sort 

	connect the standar output of pfind to the standard input of sort 
		* Using 1 Pipe

	then connect a 2nd Pipe pipe to the standard output of sort 
		- where your parent process will read from the read-end of 
		the 'sort' pipe until it reaches end-of-file (read() returns 0), 
		print out all the text received, AND report how many lines were 
		returned.



****** ** * Do Not use 'popen()'
****** ** * Do Not use 'fread()/fwrite()'
****** ** * Do Not use Any other functions that use FILE streams.


****** *** You CAN use 
	     *** You will be working with pipes (which are raw file descriptors),
 		    *** using 'fork()' and 'exec*()', 
 			*** using 'dup2()' to duplicate file descriptors, 
 		 	*** and making sure you close all unused file descriptors. 
 		 	*** You may also use read() and write().


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































