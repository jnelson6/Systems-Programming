/******************************************************************************
* Name  : main.c
* Author: Julia Nelson
* Date  : June 25, 2021
* Pledge: "I pledge my honor that I have abided by the Stevens Honor System."
* Description: Minishell  
 *  This shell must implement color printing, cd and exit commands as built-ins,
 *  signal handling, and fork/exec for all other command
******************************************************************************/
 

#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <limits.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <pwd.h>

#define BRIGHTBLUE "\x1b[34;1m"
#define DEFAULT "\x1b[0m"

static pid_t child_pid = 0;

//Function to process the cd command
static int cmd_cd(char * args[], const int nargs){
	char * ptr = NULL;

	if(nargs == 2){	//if we have a directory as argument
		ptr = args[1];

	}else if(nargs == 1){	//if no directory is given
		ptr = "~";
	}else{	//invalid syntax
		printf("Usage: cd [directory | ~]\n");
		ptr = NULL;
		return 0;
	}

	if(strcmp(ptr, "~") == 0){
		struct passwd * pw = getpwuid(getuid());
		if(pw == NULL){
			printf("Error: Cannot get passwd entry. %s.\n", strerror(errno));
			return -1;
		}
		ptr = pw->pw_dir;
	}

	//if we have a directory and chdir fails
	if((ptr != NULL) && chdir(ptr) == -1){
		printf("Error: Cannot change directory to '%s'. %s.\n", ptr, strerror(errno));
		return -1;	//return error
	}
	return 0;
}


static void child_sig_handler(int sig){
	//child just exits on any signal received
  exit(EXIT_SUCCESS);
}

static int cmd_external(char * args[], const int nargs){
	int rv, status;
	struct sigaction sa;

	const pid_t pid = fork();

	switch(pid){
		case -1:
			printf("Error: fork() failed. %s.\n", strerror(errno));
			break;
		case 0:	//child
			//change the signal handler of child
			sa.sa_flags = 0;
			sigemptyset(&sa.sa_mask);
			sa.sa_handler = child_sig_handler;
			if( (sigaction(SIGCHLD, &sa, NULL) == -1) ||

		      (sigaction(SIGTERM, &sa, NULL) == -1) ||

		      (sigaction(SIGINT,  &sa, NULL) == -1)  || //Ctrl-C
		      (sigaction(SIGTSTP, &sa, NULL) == -1)  || //Ctrl-Z

		      (sigaction(SIGTTOU, &sa, NULL) == -1)  ||
		      (sigaction(SIGTTIN, &sa, NULL) == -1)){
				printf("Error: Cannot register signal handler. %s.\n", strerror(errno));
				return EXIT_FAILURE;
			}

			if(args[0][0] == '/'){
				execv(args[0], args);	//if we have a path as first argument
			}else{
				execvp(args[0], args);	//if we have file name
			}
			printf("Error: execv() failed. %s.\n", strerror(errno));
			exit(EXIT_FAILURE);
			break;

		default:
			child_pid = pid;

			//change child process group
		  if(setpgid(pid, pid) == -1){
		    printf("Error: setpgid() failed. %s.\n", strerror(errno));
		    return -1;
		  }

		  //set child to run in foreground
		  if(tcsetpgrp(0, pid) == -1){
		    printf("Error: tcsetpgrp() failed. %s.\n", strerror(errno));
		    return -1;
		  }

			if(waitpid(pid, &status, WUNTRACED | WCONTINUED) == -1){
				printf("Error: wait() failed. %s.\n", strerror(errno));
				return -1;
			}
			break;
	}
	return 0;
}

static void sig_handler(int sig){
	switch(sig){
    case SIGTERM:
    case SIGINT:  //Ctrl-C
      if(child_pid > 0){
        if(kill(child_pid, sig) == -1){
          perror("kill");
        }
      }
			printf("\n");
      break;

    case SIGTSTP: //Ctrl-Z
      if(child_pid > 0){
        if(tcsetpgrp(0, getpid()) == -1){
          perror("tcsetpgrp");
        }
        if(kill(child_pid, sig) == -1){
          perror("kill");
        }
        child_pid = 0;
      }
      break;

    case SIGCHLD:
      if(tcsetpgrp(0, getpid()) == -1){
        perror("tcsetpgrp");
      }
      child_pid = 0;
      break;

		case SIGTTOU: //Ctrl-Z in child
      if(tcsetpgrp(0, getpid()) == -1){
        perror("tcsetpgrp");
      }
      child_pid = 0;
      break;

    default:
      break;
  }
}

int main(int argc, char *argv[]) {

	char buf[PATH_MAX];
	char *args[100];
	struct sigaction sa;

	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = sig_handler;
	if( (sigaction(SIGCHLD, &sa, NULL) == -1) ||

      (sigaction(SIGTERM, &sa, NULL) == -1) ||

      (sigaction(SIGINT,  &sa, NULL) == -1)  || //Ctrl-C
      (sigaction(SIGTSTP, &sa, NULL) == -1)  || //Ctrl-Z

      (sigaction(SIGTTOU, &sa, NULL) == -1)  ||
      (sigaction(SIGTTIN, &sa, NULL) == -1)){
	  printf("Error: Cannot register signal handler. %s.\n", strerror(errno));
		return EXIT_FAILURE;
	}

	while(1){

		int nargs = 0;

		//show prompt
		if(getcwd(buf, sizeof(buf)) == NULL){
			printf("Error: Cannot get current working directory. %s.\n", strerror(errno));
			break;
		}
		printf("[%s%s%s]$ ", BRIGHTBLUE, buf, DEFAULT);

		//get user input
		if(fgets(buf, sizeof(buf), stdin) == NULL){
			if(!feof(stdin)){
				printf("Error: Failed to read from stdin. %s.\n", strerror(errno));
			}
			break;
		}

		//tokenize the command
		args[nargs] = strtok(buf, " \t\r\n");
		while(args[nargs] != NULL){
			args[++nargs] = strtok(NULL, " \t\r\n");
		}
		args[nargs] = NULL;

		if(nargs == 0){
			continue;
		}

		if(strcmp(args[0], "cd") == 0){
			cmd_cd(args, nargs);
		}else if(strcmp(args[0], "exit") == 0){
			break;
		}else{	//external command
			cmd_external(args, nargs);
		}

	}

	

	return EXIT_SUCCESS;
}


/*
Signals:

		Your minishell needs to capture the SIGINT signal. Upon doing so, it should return the user to the prompt.

		Interrupt signals generated in the terminal are delivered to the active process group (which includes both parent & child processes)
		 The child will receive the SIGINT and deal with it accordingly.
		 ( snooze.c)

		 Make sure you use only async-safe functions in your signal handler.


			One suggestion would be to use a single volatile sig_atomic_t variable interrupted that is set to true inside the signal handler.

			 Then, inside the program’s main loop that displays the prompt, reads the input, and executes the command,

			 	don’t do anything if that iteration of the loop was interrupted by the signal.

			 	If read fails, you need to make sure it wasn’t simply interrupted before erroring out of the minishell.

			 	Finally, set interrupted back to false before the next iteration of the main loop.

*/
