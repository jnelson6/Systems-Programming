/***********************************
* statfullpath.c
* done together in class
* supposed to help with upcoming lab or assignment

***********************************/

#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("Usage: %s <directory> \n", argv[0]);
		return EXIT_FAILURE
	}


	char path[PATH_MAX];
	if (realpath(argv[1], path) == NULL) {
		fprintd(stderr, "Error: Cannot get full path of file '%s'. %s.\n",
				argv[1], sterror(errno));
		return EXIT_FAILURE
	}

	DIR *dir;
	if ((dir = opendir(path)) == NULL) {
		fprintf(stderr, "Error: Cannot open directory. '%s'. %s.\n",
				path, strerror(errno));
		return EXIT_FAILURE;
	}

	struct dirent *entry;
	struct stat sb;
	char full_filename[PATH_MAX + 2];
	size_t pathlen = 0;
	// Set the initial character to the NULL byte.
	// If path is roo '/', you can now take the strlen of a properly 
	// terminated empty string.
	full_filename[0] = '\0';
	if (strcmp(path, "/")) {
		// if the path is not the root = '/', then....

		// If there is no NULL byte among the first n bytes of the path,
		// the full_filename will not be terminated. So, copy up to and
		// including PATH_MAX characters
		strncpy(full_filename, path, PATH_MAX);
		full_filename[PATH_MAX] = '\0';

	}
	// Add + 1 for the trailing '/' that we're going to add.
	pathlen = strlen(full_filename) + 1;
	full_filename[pathlen - 1] = '/';// USEFUL FOR UPCOMING ASSIGNMENT => full_filename[pathlen] = '\0';
	printf("%s\n", full_filename);			// for when you have to recursively go down directories so all we 
											// have to do is concatinate at end of string 
	// test by typing make and then examples like
	//  ./statfullpath .
	//  ./statfullpath /
	//  ./statfullpath 


	while ((entry = readdir(dir)) != NULL) { 
		if(strcmp(entry->d_name, ".") == 0 ||
			strcmp(entry->d_name, "..") == 0 ) {
			continue;
		}
		strncpy(full_filename + pathlen, entry->d_name, PATH_MAX - pathlen);
		if (lstat(full_filename, &sb) < 0) {
			fprintf(stderr, "Error: Cannot stat file '%s'. %s.\n",
					full_filename, strerror(errno));
			continue;
		}
		if (entry->d_type == CT_DIR) {
			printf("%s [directory]\n", full_filename);
		} else {
			printf("%s\n", full_filename);
		}			
	}

	closedir(dir)
	return EXIT_SUCCESS;
}

// in upcoming assignment we are recursing into directory not printing the
// [directory] when we see it. Will need to change/refactor this a little
// and turn it into a functionable program.
// under lstat need to check if permissions match what we need and then print them
// printing all files of root
// code will be posted



// going to make head file
// read top few lines of file with read and write 









