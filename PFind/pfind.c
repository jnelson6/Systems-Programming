/******************************************************************************
* Author: Julia Nelson
* Date: June 12, 2021
* Pledge: "I pledge my honor that I have abided by the Stevens Honor System."
* Description: Permission Finder in C - hw3
* 			   (finds files with a specified set of permissions)
******************************************************************************/

#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pfind.h"

//maximum number of characters in path
#define C_STRING_SIZE 1024

/*
validate arguments
return 0
or 1 if help
or -1 if no arguments
or -2 if dir missing
or -3 if permission missing
-4 others
*/
int validateInput(int argc, char* argv[], int* dirPos, int* permissionPos) {

	/* Step 1: Validate input */

	int i; //loop control variable
	int dFlag = 0, pFlag = 0;

	*dirPos = -1;
	*permissionPos = -1;

	/*
	Check that all arguments passed to program are valid. Only -d, -h and -p flags
	should be passed, no arbitrary flags should be passed, and if -h is passed, then
	the usage statement should be printed with an exit status of EXIT_SUCCESS.
	*/

	if (argc == 1) { //no arguments
		// If no arguments are supplied, display the usage statement and exit the program with EXIT_FAILURE.		
		return -1; // may need exit(EXIT_FAILURE)
	}

	for (i = 1; i < argc; i++) {

		//check -h flag
		if (strcmp(argv[i], "-h") == 0) { //last for help			
			return 1;
		}

		//-d flag
		if (strcmp(argv[i], "-d") == 0) {
			dFlag = i;

			if (i < argc - 1 && argv[i + 1][0] != '-') {
				*dirPos = i + 1;
			}
			i++;
		}
		else if (strcmp(argv[i], "-p") == 0)
		{  //-p flag
			pFlag = i;

			if (i < argc - 1) {
				*permissionPos = i + 1;
			}
			i++;
		}
		else if (argv[i][0] == '-') {
			printf("Error: Unknown option '%s' received.\n", argv[i]);
			return -4;
		}

	}//end for

	if (dFlag == 0 || *dirPos == -1) {

		
		return -2;
	}

	if (pFlag == 0 || *permissionPos == -1) {

		
		return -3;
	}

	return 0;
}


/*
It will recursively search for files whose permissions match
the permissions string starting in the specified directory.
__________
You will be traversing the directory tree using the function readdir (see man 3 readdir)
- by first opening a directory (man 3 opendir),
- and for every file, checking the permissions on it using stat (man 2 stat).
		- The function stat allows you to check what type of file it is
		(regular file, directory, symlink, block special, etc), and
		handle each accordingly.


		LOOK AT statFULLPATH.c for helpful example
				// We want to be recursing into directory not printing the
				// [directory] when we see it. Will need to change/refactor this a little
				// and turn it into a functionable program.
				// under lstat need to check if permissions match what we need and then print them
				// printing all files of root
*/

/*
find files based on the permission
this function is the recursive function
that also check the subdirectory
*/
void find(const char* dirName, char* permission, int* count) {

	DIR* dir;
	struct dirent* entry;

	if (!(dir = opendir(dirName)))
		return;

	// process each entry
	while ((entry = readdir(dir)) != NULL) {

		//is directory
		if (entry->d_type == DT_DIR) {

			//path of subdirectory
			char path[C_STRING_SIZE];

			if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
				//ignore the . or ..
			}
			else {
				// help with snprintf found at https://joequery.me/code/snprintf-c/
				snprintf(path, sizeof(path), "%s/%s", dirName, entry->d_name);

				//call recursive to find in the sub directory
				find(path, permission, count);
			}
		}
		else { //is file

	  //path of subdirectory
			char path[C_STRING_SIZE];

			snprintf(path, sizeof(path), "%s/%s", dirName, entry->d_name);

			//check file permission against the input
			if (hasPermission(path, permission)) {
				printf("%s\n", path);

				*count += 1;
			}
		}
	}//end while  
}

/*
The usage is to look as follows:
Usage: ./pfind -d <directory> -p <permissions string> [-h]
*/

int main(int argc, char* argv[]) {

	int i; //loop control variable	

  //directory and permission in the argv
	int dirPos, permissionPos;

	/* Step 1: Validate input */
	int validate = validateInput(argc, argv, &dirPos, &permissionPos);
	if (validate == 1) { //help
		printf("Usage: ./pfind -d <directory> -p <permissions string> [-h]\n");
		return EXIT_SUCCESS;
	}
	else if (validate == -1)
	{ //no argument
		printf("Usage: %s <directory> -p <permissions string> [-h]\n", argv[0]);
		return EXIT_FAILURE;
	}
	else if (validate == -2) //-d missing
	{
		printf("Error: Required argument -d <directory> not found.\n");
		return EXIT_FAILURE;
	}
	else if (validate == -3) //-p missing
	{
		printf("Error: Required argument -p <permissions string> not found.\n");
		return EXIT_FAILURE;
	}
	else if (validate < 0)
	{
		return EXIT_FAILURE;
	}
	char* directory = argv[dirPos];
	char* permission = argv[permissionPos];
	int count = 0; //find found?

	//stat ?
	//get file statistics
	struct stat st;
	if (stat(directory, &st) != 0) {
		fprintf(stderr, "Error: Cannot stat '%s'.No such file or directory.\n", directory);
		return EXIT_FAILURE;
	}

	// TRY TO OPEN THE DIRECTORY TO SEE IF IT A DIRECTORY
	// If an invalid directory dir is passed to -d, then the following error message will be printed to standard error 
	// 	and the status EXIT_FAILURE will be returned: Error: Cannot stat 'dir'. No such file or directory.
	DIR* dir;
	if ((dir = opendir(directory)) == NULL) {
		fprintf(stderr, "Error: Cannot open directory '%s'. Permission denied.\n", directory);
		return EXIT_FAILURE;
	}

	char path[C_STRING_SIZE];
	if (realpath(directory, path) == NULL) {
		fprintf(stderr, "Error: Cannot get full path of file '%s'.\n", directory);
		return EXIT_FAILURE;
	}

	// close directory
	closedir(dir);

	//check permission length
	if (strlen(permission) != 9) {
		printf("Error: Permissions string '%s' is invalid\n", permission);
		return EXIT_FAILURE;
	}

	// check format rwxrwxrwx
	for (i = 0; i < 9; i++) {
		//the position 0, 3, 6 must be 'r' or '-'
		if (i == 0 || i == 3 || i == 6) {
			if (permission[i] != 'r' && permission[i] != '-') {
				break;
			}
		}

		//the position 1, 4, 7 must be 'w' or '-'
		if (i == 1 || i == 4 || i == 7) {
			if (permission[i] != 'w' && permission[i] != '-') {
				break;
			}
		}

		//the position 2, 5, 8 must be 'x' or '-'
		if (i == 2 || i == 5 || i == 8) {
			if (permission[i] != 'x' && permission[i] != '-') {
				break;
			}
		}
	}

	if (i < 9) { //invalid permission (break in the previous 'for' loop)
		printf("Error: Permissions string '%s' is invalid\n", permission);
		return EXIT_FAILURE;
	}	

	//recursive find
	find(path, permission, &count);

	if (count == 0)
	{
		printf("<no output>\n");
	}

	return EXIT_SUCCESS;
}


/*

Permissions strings are to be formatted similarly to how the command ls formats them.
There is, however, an important distinction.
	- The ls permissions string has 10 characters,
	- Ours will only have the rightmost 9.

In UNIX systems, the leftmost character specifies the type of file
	(d for directory, l for symlink, etc).
Our program will not consider this, and will only worry about the rightmost 9.
________________________

Those characters are in the following format:
	rwxrwxrwx.

They can be broken up as follows:
	|rwx 	|rwx 	|rwx 	|
	| USER  | GROUP | OTHER |

(USER is synonymous with OWNER in this context)

The presence of r represents read permissions.
The presence of w represents write permissions.
The presence of x represents execute permissions.
_______
If any of those characters are replaced by a dash (-), it represents a lack of
permissions of that kind.
For example,
	in the permissions string rw-r--r--,
	the user has permission to read and write, while anyone who is not the file's
	owner but is in the user's group only has permission to read,
	as is the case with anyone else on the system.


Keep in mind, A DIRECTORY IS JUST A SPECIAL KIND OF FILE!
So, you will be keeping directories in mind as well.
*/
/*
check if file has permission
return 1 (if true) or 0
*/
int hasPermission(const char* filename, char* permission) {

	struct stat st;

	//get file statistics
	if (stat(filename, &st) == 0) {

		mode_t perm = st.st_mode;
		// user 
		if (permission[0] == 'r' && !(perm & S_IRUSR)) { //USER read
			return 0;
		}
		if (permission[1] == 'w' && !(perm & S_IWUSR)) { //USER write
			return 0;
		}
		if (permission[2] == 'x' && !(perm & S_IXUSR)) { //USER execute
			return 0;
		}
		// group
		if (permission[3] == 'r' && !(perm & S_IRGRP)) { //GROUP read
			return 0;
		}
		if (permission[4] == 'w' && !(perm & S_IWGRP)) { //GROUP write
			return 0;
		}
		if (permission[5] == 'x' && !(perm & S_IXGRP)) { //GROUP execute
			return 0;
		}
		// other
		if (permission[6] == 'r' && !(perm & S_IROTH)) { //OTHER read
			return 0;
		}
		if (permission[7] == 'w' && !(perm & S_IWOTH)) { //OTHER write
			return 0;
		}
		if (permission[8] == 'x' && !(perm & S_IXOTH)) { //OTHER execute
			return 0;
		}

	}
	else {
		return 0;//could not get file statistics
	}

	return 1;
}



