/***
* cat.c - our own cat functions
* using system commands
* 
* can use (man 2 open) for help
***/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#define BUFSIZE 128 // Note including '\0'.


void display_usage(char *progname) {
	printf("Usage: %s <filename>\n", progname);
}

int main(int argc, char *argv[]) {
	if (argc != 2) {
		display_usage(argv[0]);
		return EXIT_FAILURE;
	}

	int src_fd;
	if ((src_fd = open(argv[1], O_RDONLY) ) == -1) {
		fprintf(stderr, "Error: Cannot open source file '%s'. %s.\n", 
			argv[1], strerror(errno));
	}
	char *buf;
	if ((buf = (char *)malloc((BUFSIZE +1) * sizeof(char))) == NULL) {
		close(src_fd);
		return EXIT_FAILURE;
	}
	int bytes_read;
	/* Read up to BUFSIZE bytes each time the loop iterates. */
	while ((bytes_read = read(src_fd, buf, BUFSIZE)) > 0) {
		if (bytes_read == -1 ){
			fprintf(stderr, "Error: Failed to read from file '%s'. %s.\n",
					argv[1],strerror(errno));
			return EXIT_FAILURE
		} else{
			int bytes_written;
			if ((bytes_written = write(STDOUT_FILENO, buf, bytes_read)) != bytes_read) {
				fprintf(stderr, "Error: Failed to write to STDOUT. %s.\n",
					strerror(errno));
			return EXIT_FAILURE
			}
		}
	}
	free(buf);
	close(src_fd);

	return EXIT_SUCCESS;
}



