/**** example from class June 3, 2021      ***/
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
/* 
(man 2 stat)

	struct stat {
               dev_t     st_dev;         / * ID of device containing file * /
               ino_t     st_ino;         / * Inode number * /
               mode_t    st_mode;        / * File type and mode * /
               nlink_t   st_nlink;       / * Number of hard links * /
               uid_t     st_uid;         / * User ID of owner * /
               gid_t     st_gid;         / * Group ID of owner * /
               dev_t     st_rdev;        / * Device ID (if special file) * /
               off_t     st_size;        / * Total size, in bytes * /
               blksize_t st_blksize;     / * Block size for filesystem I/O * /
               blkcnt_t  st_blocks;      / * Number of 512B blocks allocated * /

	};
*/

int main(int argc, char *argv[]) {
	struct stat buf;
	char output[128], *ptr;

	for (int i = 1; i < argc; i++) {
		printf("%s: ", argv[i]);
		/* the lstat function is similar to stat, but when the names file is a 
		* symbolic link, lstat returns information about the symbolic link, 
		* not the file referenced by the symbolic link
		**/
		if (lstat(argv[i], &buf) < 0) {
			fprintf(stderr, "Error: Cannot stat '%s'.\n", argv[i]);
			continue;
		}
		if (S_ISREG(buf.st_mode)) {
			sprintf(output, "regular, %zu bytes", buf.st_size);
			ptr = output;
		} else if (S_ISDIR(buf.st_mode)) {
			ptr = "directory";
		} else if (S_ISCHR(buf.st_mode)) {
			ptr = "character special";
		} else if (S_ISBLK(buf.st_mode)) {
			ptr = "block special";
		} else if (S_ISFIFO(buf.st_mode)) {
			ptr = "fifo special";
		} else if (S_ISLNK(buf.st_mode)) {
			ptr = "symbolic link";
		} else if (S_ISSOCK(buf.st_mode)) {
			ptr = "socket";
		} else {
			ptr = "** unkown modes **";
		}
		printf("%s\n", ptr);

	}
	return EXIT_SUCCESS;
}