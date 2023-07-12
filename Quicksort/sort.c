/*******************************************************************************
 * Name        : sort.c
 * Author      : Julia Nelson
 * Date        : June 5, 2021
 * Description : Uses quicksort to sort a file of either ints, doubles, or
 *               strings.   (the source file that contains the main function) 
 * Pledge      : "I pledge my honor that I have abided by the Stevens Honor System."
 ******************************************************************************/
#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "quicksort.h"

#define MAX_STRLEN     64 // Not including '\0'
#define MAX_ELEMENTS 1024

typedef enum {
    STRING,
    INT,
    DOUBLE
} elem_t;


// DO I NEED THESE STILL
int string_flag = 0;
int int_flag = 0;
int double_flag = 0;
int flag_count = 0;

int line_count = 0;



void usage_message(){
	printf("Usage: ./sort [-i|-d] filename");		// may need to add \n to end before quotes
	printf("   -i: Specifies the file contains ints.");
	printf("   -d: Specifies the file contains doubles.");
	printf("   filename: The file to sort.");
	printf("   No flags defaults to sorting strings.");
}






/**       
 * Reads data from filename into an already allocated 2D array of chars.
 * Exits the entire program if the file cannot be opened.
 */
size_t read_data(char *filename, char **data) {
    // Open the file.
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: Cannot open '%s'. %s.\n", filename,
                strerror(errno));
        free(data);
        exit(EXIT_FAILURE);
    }
    // Read in the data.
    size_t index = 0;
    char str[MAX_STRLEN + 2];
    char *eoln;
    while (fgets(str, MAX_STRLEN + 2, fp) != NULL) {
        eoln = strchr(str, '\n');
        if (eoln == NULL) {
            str[MAX_STRLEN] = '\0';
        } else {
            *eoln = '\0';
        }
        // Ignore blank lines.
        if (strlen(str) != 0) {
            data[index] = (char *)malloc((MAX_STRLEN + 1) * sizeof(char));
            strcpy(data[index++], str);

            line_count++;
        }
    }
    // Close the file before returning from the function.
    fclose(fp);

    return index;
}







/**
 * Basic structure of sort.c:
 *
 * Parses args with getopt.

 * Opens input file for reading.

 * Allocates space in a char** for at least MAX_ELEMENTS strings to be stored,
 * where MAX_ELEMENTS is 1024.

 * Reads in the file
 * - For each line, allocates space in each index of the char** to store the
 *   line.

 * Closes the file, after reading in all the lines.

 * Calls quicksort based on type (int, double, string) supplied on the command
 * line.

 * Frees all data.

 * Ensures there are no memory leaks with valgrind. 
 */

int main(int argc, char **argv) {
	int opt;

	// DO I NEED THESE
	//optind = 1;
	opterr = 0;

  	// Case 1: No input arguments 
	if(argc == 1) {
		usage_message();
    	return EXIT_FAILURE;
	}
	
	// Parse flags
	while ((opt = getopt(argc, argv, ":id")) != -1) {
		switch (opt) {
      		case 'i':
				int_flag = 1;
				flag_count++;
				if (argc < 3 ){
                	printf("Error: No input file specified.");
                	return EXIT_FAILURE;
            	}
            	// not sure if needed here 
  				// Case 3: Invalid filename ????????
    			if (!fopen(argv[2], "r")) {
        			fprintf(stderr, "Error: Cannot open '%s'. %s.\n", argv[2], strerror(errno));
        			return EXIT_FAILURE;
        		}
        		// CASE 5: Multiple Filenames
    			if ((flag_count == 1) && (argc > 3)) {
    				printf("Error: Too many files specified.");
        			return EXIT_FAILURE;
    			} 
        		///////fclose(argv[2]); ?
        		break;		


      		case 'd':
				double_flag = 1;
				flag_count++;
				if (argc < 3 ){
                	printf("Error: No input file specified.");
                	return EXIT_FAILURE;
            	}
            	// not sure if needed here 
  				// Case 3: Invalid filename ????????
    			if (!fopen(argv[2], "r")) {
        			fprintf(stderr, "Error: Cannot open '%s'. %s.\n", argv[2], strerror(errno));
        			return EXIT_FAILURE;
        		}
        		// CASE 5: Multiple Filenames
    			if ((flag_count == 1) && (argc > 3)) {
    				printf("Error: Too many files specified.");
        			return EXIT_FAILURE;
    			} 
        		///////fclose(argv[2]); ?
        		break;

    		// Case 2: Invalid Flag   
      		case '?':
				printf("Error: Unkown option '%s' recieved.\n", optarg );
				usage_message();
        		return EXIT_FAILURE;
        		break;

        		// MAY NOT BE NEEDED ANYMORE 
        	// Case 4: No filename  &  Case 7: & valid flags with unkown flags
        	case ':':
				printf("Error: No input file specified.");
				usage_message();
        		return EXIT_FAILURE;
     	}
  	}

    // Case 6: Multiple Valid Flags 
    if ((int_flag != 0) && (double_flag != 0)) {						
		printf("Error: Too many flags specified.\n");
		return EXIT_FAILURE;
	} 
  	// Case 5: Multiple Filenames when no flag given 
  	if(flag_count == 0) {
    	if (argc > 2) {
    		printf("Error: Too many files specified.");
        	return EXIT_FAILURE;
        } 
  		// Case 3: Invalid filename ????????
        if (!fopen(argv[1], "r")) {
        	fprintf(stderr, "Error: Cannot open '%s'. %s.\n", argv[1], strerror(errno));
        	return EXIT_FAILURE;
        }
        //fclose(argv[1]); ?
  	}




		char **data = (char **)malloc(MAX_ELEMENTS);  // * sizeof(char) ??????
		

		/* INT */
  		if ((flag_count == 1) && (int_flag == 1)) { // or optarg or argv[1] 
  			char *filename = argv[2];
  			read_data(filename, data);



			int *t_int = malloc(MAX_ELEMENTS * sizeof(int));

			for(int i = 0; i < line_count; i++) {
				t_int[i] = atoi(data[i]);

				
			}
  			quicksort(t_int, line_count, 4, int_cmp);
  			for(int i = 0; i < line_count; i++) {
				printf("%d\n", *(t_int+i) );
			}

  		}

		/* DOUBLE */
  		if ((flag_count == 1) && (double_flag == 1)) { // or optarg or argv[1] 
  			char *filename = argv[2];
  			read_data(filename, data);


  			double *t_dub = malloc(MAX_ELEMENTS * sizeof(double));

			for(int i = 0; i < line_count; i++) {
				//*(converted_DOUBLES + i) = atoi(data+i);
				t_dub[i] = (double) atof(data[i]);
			}
  			quicksort(t_dub, line_count, 8, dbl_cmp);
  			for(int i = 0; i < line_count; i++) {

				printf("%f\n", *(t_dub+i) );
			}
  		} 
  		/* printf("%lf\n", (t_dub + i) );
      |             ~~^     ~~~~~~~~~~~
      |               |            |
      |               double       double *


      lf tdub +i

      maybe take out added stars

  		*/

		/* STRING */
  		else if(flag_count == 0) {
  			char *filename = argv[1];
  			read_data(filename, data);
  			quicksort(data, line_count, 1, str_cmp);
  			for(int i = 0; i < line_count; i++) {
				printf("%s\n", *(data + i) );
			}
  		}

  		free(data);
  		return EXIT_SUCCESS;
  	}



/*

  			double converted_DOUBLES[line_count];
  			else if(double_flag == 1){

  				/ * FOUND IN TEXTBOOK: atoi: convert string s to integer using atof * /
 			
  				quicksort(data, idx, 8, dbl_cmp(const void*, const void*));

  				for(int i = 0; i < idx; i++) {
            		printf("%s\n", *(data + i));
        		}
  			} else {
  				quicksort(data, idx, 1, str_cmp(const void*, const void*));

  				for(int i = 0; i < idx; i++){
            		printf("%s\n",*(data + i));
        		}

  			}
	}

    return EXIT_SUCCESS;
}


  			*/






/*
	if (optind < argc) {
		printf("Non-option args: ");
    	while (optind < argc){
      		printf("%s ", argv[optind++]);
    		printf("\n");
    	}

/ * atoi: convert string s to integer using atof * /
 int atoi(char s[])
 {
 double atof(char s[]);
 return (int) atof(s);
 }

*/

