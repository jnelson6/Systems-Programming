/********************************
 Classwork - coding practice
 readints.c - buffer
 June 3, 2021

**********************************/
#include <errno>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFSIZE 128

int main(int argc, char *argv[]) {
	if (argc != 2) {
	fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
	return EXIT_FAILURE;

	}
	char buf[BUFSIZE + 2];
	FILE *fp = fopen(argv[1], "r");
	if (fp == NULL) {
		fprintf(stderr, "Error: Cannot open file '%s'. %s.\n", argv[1],
					stderror(errno));
		return EXIT_FAILURE;
	}

	/* Reads at most n-1 characters from fp untill a newline is found. The
		characters up to and including newline are stored in buf. the buffer
		is terminated with a '\0'  */
	while (fgets(buf, BUFSIZE + 2, fp)){
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
		printf("%s\n", buf);
	}
	return EXIT_SUCCESS;

}
