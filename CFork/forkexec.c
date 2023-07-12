// save as forkexec

//need pfind in the folder
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno>
#include <unitstd.h>
#include <sys/wait.h>
int main(int argc, char *argv[]) {
	// get the process ID of this (the parent) process
	printf("Parent PID: %ld\n", (long)getpid() );
	pid_t pid;
	if ((pid = fork()) < 0) {
		fprintf(stderr, "Error: for failed. %s\n", strerror(errno));
		return EXIT_FAILURE;
	} else if (pid > 0) {
		// were in the parent
		// pid is the process id of the child
		printf("Child PID: %ld\n", (long)pid);
		int status;
		do{
		// COPY IN THE FORK ID HIGHLIGHTED STUFF FROM SCREENSHOT AT 4:07 on June 9th 
		// wait for the child to complete and get the status of how it terminated
		pid_t w = waitpid(pid, &status, WUNTRACED | WCONTINUED);





		}
	} else {
		// we're in the child 
		if (execv("     pfind", argv) == -1) {
			fprintf(stderr, "Error: execvp() failed. %s.\n", strerror(errno));
			return EXIT_FAILURE;
		}
		//?
		printf("HI.\n");

	}
	return EXIT_SUCCESS;
}