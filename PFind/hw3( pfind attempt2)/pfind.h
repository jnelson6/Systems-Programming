#ifndef PFIND_H //for preventing include many times
#define PFIND_H

#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
pfind.h
*/

/*
validate arguments
return EXIT_FAILURE
or EXIT_SUCCESS
*/
int validateInput(int argc, char* argv[], int* dirPos, int* permissionPos);

/*
find files based on the permission
this function is the recursive function
that also check the subdirectory
*/
void find(const char* dirName, char* permission, int* count);

/*
check if file has permission
*/
int hasPermission(const char* filename, char* permission);

#endif