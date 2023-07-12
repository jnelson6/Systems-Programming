/********************************
 Classwork - coding practice
 readints.c - buffer
 June 3, 2021

**********************************/
#include <ctype.h>
#include <errno>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFSIZE 128
/****
	Determines whether or not the input string representd a valid integer.
	A valid integer has an optional minus sign, followed by a series of digits
	[0-9].
******/
bool is_integer(char *input) {
	int start = 0, len = strlen(input);

	if (len >= 1 && input[0] == '-') {
		if (len < 2) {
			return false;
		}
		start = 1;
	}
	for (int i = start; i < len; i++) {
		if (!isdigit(input[i])) {
			return false;
		}
	}
	return true;
}


/**
* Takes as input a string an in-out parameter value.
* If the string can be parsed, the integer value is assigned to the value
* parameter and true is returned. 
* Otherwise, false is returned an the best attempt to modify the value 
* parameter is made
**/
bool get_integer(char *input, int *value) {
	long long long_long_i;
	if(sscanf(input, "%lld", &long_long_i) != 1) {
		return false;
	}
	*value = (int)long_long_i;
	if (long_long_i != (long long)*value) {
		fprintf(stderr, "Warning: Integer overflow with '%s'.\n", input);
		return false;
	}
	return true;
}




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
		if (is_integer(buf)) {
			int val;
			if (get_integer(buf, &val)) {
				printf("%d\n", val);
			}
		}
	}
	fclose(fp);
	return EXIT_SUCCESS;

}
