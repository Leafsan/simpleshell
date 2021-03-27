#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "simple_shell.h"

/* builtin function part */
int cmd_test(char **args);
int cmd_exit(char **args);

/* builtin function command list */
char *builtin_str[] = {
	"test",
	"exit"
};

int (*builtin_func[]) (char **) = {
	&cmd_test,
	&cmd_exit
};

int num_builtins() {	//Count how many builtin command are there.
	return sizeof(builtin_str) / sizeof(char *);
}

/* builtin function  */
int cmd_test(char ** args) {
	printf("%s\n",args[1]);
	printf("test\n");
}

int cmd_exit(char ** args) {
	exit(0);
}

/* execution part */
int execute(char*);
int launchBuiltin(char*, int, int, int, int);
int executeCommand(char*, int, int, int, int);


/*
 *	Function "execute" first divide input strings into commands string list.
 *	After that it executes each commands one by one.
 */
int execute(char * args) {
	char ** cmd_list;
	int i, n = 0, isFirst= 1, input = 0, isBackground = 0;

	if (strchr(args, '&')) {				//Check background process character
		isBackground = 1;
	}

	cmd_list = tokenizeLine(args, "|&");	//Divide command by "|" and "&"

	while(cmd_list[n] != NULL){				//Count command numbers.
		n++;
	}

	for (i = 0; i < n - 1 ; i++) {			//Execute commands from first to before the last(pipe)
		input = launchBuiltin(cmd_list[i], input, isFirst, 0, isBackground);
		isFirst = 0;	
	}

	input = launchBuiltin(cmd_list[i], input, isFirst, 1, isBackground);	//Execute last commands

	return 0;
}
/*
 *	Function "launchBuiltin" analyzes input commands strings are builtin commands or not.
 *	If the commands are builtin, it performs builtin commands.
 *	If not it sends original command strings to "executeCommand" function to execute them.
 */
int launchBuiltin(char * cmd, int input, int first, int last, int background) {
	int i;
	char ** argv;	//arguments array
	char * new_cmd;

	new_cmd = strdup(cmd);

	argv = tokenizeLine(cmd, " \t\r\n\a");	//Divide commands by blank characters to make arguments array

	if (argv[0] == NULL)
		return 1;

	for (i = 0; i < num_builtins(); i++) {
		if (strcmp(argv[0], builtin_str[i]) == 0) {
			return (*builtin_func[i]) (argv);
		}
	}

	return executeCommand(new_cmd, input, first, last, background);	//Send commands that are not builtin command to executeCommand
}
/*
 *	Function "executeCommand" performs commands that are not builtin commands
 *  
 *	- List of what it does - 
 * 1)It makes pipe for communication between parents and childs
 *	And it uses "fork()"to make parent and child.
 * 2)It checks redirection characters and uses file descriptor for input/output.
 * 3)It checks "background" variable is either "1" or "0" and performs the process as background or not.
 */
int executeCommand(char * arg, int input, int first, int last, int background) {
	int pipefd[2], fd_in, fd_out, status;
	char * ret;		//actual file name for redirection
	char ** argv;	//arguments array
	char ** redirection;	//Array for divide strings into arguments and file name.
	pid_t pid, wpid;

	if (pipe(pipefd) == -1) {	//make pipe
		perror("ERROR: pipe error");
		return 1;
	}

	pid = fork();				//fork

	if(pid < 0) {				//Failed to fork
		fprintf(stderr, "ERROR: fork error");
	}
	else if (pid == 0) {		//Child process
		if (first == 1 && last == 0 && input == 0)			// first commands that with no input
			dup2(pipefd[1], 1);
		else if (first == 0 && last == 0 && input != 0) {	// not first commands and last commands with input
			dup2(input, 0);
			dup2(pipefd[1], 1);
		}
		else
			dup2(input, 0);

		
		if (strstr(arg, "<")) {		//redirection_in case
			redirection = tokenizeLine(arg, "<");

			ret = trim(redirection[1]);

			if ((fd_in = open(ret, O_RDONLY, 0)) < 0) {	//open file
				fprintf(stderr, "ERROR: Failed to open %s for reading\n", redirection[1]);
				return (EXIT_FAILURE);
			}
			dup2 (fd_in, 0);
			close (fd_in);
		}
		else if (strstr(arg, ">")) {	//redirection_out case
			redirection = tokenizeLine(arg, ">");

			ret = trim(redirection[1]);
			
			if ((fd_out = creat(ret, 0644)) < 0) {	//write file
				fprintf(stderr, "ERROR: Failed to open %s for writing\n", redirection[1]);
				return (EXIT_FAILURE);
			}
			dup2 (fd_out, 1);
			close (fd_out);
		}

		argv = tokenizeLine(arg, " \t\r\n\a");	//make arguments
		if (execvp(argv[0], argv) < 0 )	//execute commands
			fprintf(stderr, "ERROR: Command %s not found\n", argv[0]);
		exit(0);

	}
	else {	//Parent process
		if (background == 0)	//check the process is background or not
		wpid = waitpid(pid, 0, 0);
	}

	if (last == 1)		//if it is last command, close pipe
		close(pipefd[0]);

	if (input != 0)		//if there is inputs, close input.
		close(input);

	close(pipefd[1]);	//close rest pipe
	
	return (pipefd[0]);	//return output
}
