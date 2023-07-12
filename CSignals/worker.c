// csignals - worker
// worker.c
// want the process to say they are a worker and keep printing it and we try to send s
//igint and sigterm to see what happpens 

#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


/* Volatile variables are always read from memory and not from a register
	Compiler optimizations are disabled for volatile variables.
	The type sig_atomic_t is an integer that can be accessed as an atomic entity
	even in the presence of asynchronous interrupts. 
	(simply - reading int from memory)
*/
volatile sig_atomic_t signal_val = 0;

/* Signal handler -- keep it simple */
void catch_signal(int sig) {
	signal_val = sig;
}

int main() {
	struct sigaction action;

	memset(&action, 0, sizeof(struct sigaction));
	action.sa_handler = catch_signal; //need to write function later
	action.sa_flags = SA_RESTART; // restart syscalls if possible -- do not do in MINISHELL assignment

	if (sigaction(SIGINT, &action, NULL) == -1) {
		perror("sigaction");
		return EXIT_FAILURE;

	}
	if (sigaction(SIGTERM, &action, NULL) == -1) { // can be caught 
		perror("sigterm");
		return EXIT_FAILURE;

	}

	while (true) {
		if (!signal_val) {
			printf("I am busy working. Do not disturb me!\n");
		} else if (signal_val == SIGINT) {
			printf("ARGH! I told you not to disturb me!\n");
			signal_val = 0;
		} else if (signal_val == SIGTERM) {
			printf("YOU TRIED TO TERMINATE ME?\n");
			signal_val = 0;
		}

		sleep(1); 
	}
	return EXIT_SUCCESS;

}


// KILL for MINISHELL - do not let keep running in list 
// need to kill use:
// ps -aux | grep worker 	-to get info on what we should kill under user & pick the actual program 
// or 
// kill -9 2167
// 		sigterm # and # of process
// can use "top" to see processes
// man  top for help
// to print process id with top --- 





