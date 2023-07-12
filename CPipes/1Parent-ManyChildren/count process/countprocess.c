
// save as countprocess.c incide CPipesOneParent
// helpful for sortedpfind

#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>


int main(int argc, char *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <process name>\n", argv[0]);
		return EXIT_FAILURE;
	}
	//create pipes
	int ps_to_grep[2], grep_to_wc[2], wc_to_parent[2];
	pipe(ps_to_grep);
	pipe(grep_to_wc);
	pipe(wc_to_parent);
	// should check output of pipes in assignment but not done here
	// if they fail dont continue 
	// now need to get the fork

	pid_t pid[3];
	if ((pid[0] = fork()) == 0) {
		// ps
		// close off unneccessary pipes
		close(ps_to_grep[0]);
		// normally ps just writes to stout but use 
		dup2(ps_to_grep[1], STDOUT_FILENO);

		// close all unrelated file descriptors
		close(grep_to_wc[0]);
		close(grep_to_wc[1]);
		close(wc_to_parent[0]);
		close(wc_to_parent[1]);

		// use exec - execlp makes a list from in the system path
		// tells us where all executables are - path?
		execlp("ps", "ps", "-A", NULL);	// -A is identical to e and means all processes
		// if any of these fail we want to fail entire program

		// if program not ending...
			// close(ps_to_grep[1]); 
			// also try exit(EXIT_FAILURE)
	}

	if ((pid[1] = fork()) == 0) {
		//grep
		close(ps_to_grep[1]);
		dup2(ps_to_grep[0], STDIN_FILENO);
		close(grep_to_wc[0]);
		dup2(grep_to_wc[1], STDOUT_FILENO);

		// close all unrelated file descriptors
		close(wc_to_parent[0]);
		close(wc_to_parent[1]);

		// time to exec 
		// use execlp
		execlp("grep", "grep", "-i", argv[1], NULL);
	}

	if((pid[2] = fork()) == 0) {
		//wc
		close(grep_to_wc[1]);
		dup2(grep_to_wc[0], STDIN_FILENO);
		close(wc_to_parent[0]);
		dup2(wc_to_parent[1], STDOUT_FILENO);

		// close all unrelated file descriptors
		close(ps_to_grep[0]);
		close(ps_to_grep[1]);

		//exec
		execlp("wc", "wc", "-l", NULL);
	}

	close(wc_to_parent[1]);
	dup2(wc_to_parent[0], STDIN_FILENO);
	// close all unrelated file descriptors 
	close(ps_to_grep[0]);
	close(ps_to_grep[1]);
	close(grep_to_wc[0]);
	close(grep_to_wc[1]);

	char buffer[128];
	ssize_t count = read(STDIN_FILENO, buffer, sizeof(buffer) -1);

	// attempt to stop program from continuing execution
	buffer[0] = '\0';
	write(STDOUT_FILENO, buffer, count);
	printf("%ld\n", count);

	if (count == -1) {
		// means failed
		perror("read()");
		exit(EXIT_FAILURE);
	} else if (count != 0) {
	buffer[count] = '\0';
	print("Number of processes containing \"%s\": %d\n",
			argv[1], atoi(buffer));
	}
	close(wc_to_parent[0]);
	wait(NULL);
	wait(NULL);
	wait(NULL);

	return EXIT_SUCCESS;



}






// run with "make" then "./countprocess kworker"
// then type " ps -A | grep -i "kworker" | wc -l " to check if same



// man popen
// good for pipes for this lab - only reading or writing not both
// just want to start one process use popen then pclose
// look at screenshot 
// at count index if cant find \n put in \0 there 
//  at bottom need to use ! to negate result to give 0  
// run by screenshot





