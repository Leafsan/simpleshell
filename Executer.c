#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <ctype.h>
#include "simple_shell.h"

/* builtin function part */
int cmd_test(char **args);
int cmd_exit(char **args);

char *builtin_str[] = {
	"test",
	"exit"
};

int (*builtin_func[]) (char **) = {
	&cmd_test,
	&cmd_exit
};

int num_builtins() {
	return sizeof(builtin_str) / sizeof(char *);
}

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
char* trim(char*);

int execute(char * args) {
	char ** cmd_list;
	int i, n = 0, isFirst= 1, input = 0, isBackground = 0;

	if (strchr(args, '&')) {
		isBackground = 1;
	}

	cmd_list = tokenizeLine(args, "|&");

	while(cmd_list[n] != NULL){
		n++;
	}

	for (i = 0; i < n - 1 ; i++) {
		input = launchBuiltin(cmd_list[i], input, isFirst, 0, isBackground);
		isFirst = 0;	
	}

	input = launchBuiltin(cmd_list[i], input, isFirst, 1, isBackground);

	return 0;
}
int launchBuiltin(char * cmd, int input, int first, int last, int background) {
	int i;
	char ** argv;
	char * new_cmd;

	new_cmd = strdup(cmd);

	argv = tokenizeLine(cmd, " \t\r\n\a");

	if (argv[0] == NULL)
		return 1;

	for (i = 0; i < num_builtins(); i++) {
		if (strcmp(argv[0], builtin_str[i]) == 0) {
			return (*builtin_func[i]) (argv);
		}
	}

	return executeCommand(new_cmd, input, first, last, background);
}
int executeCommand(char * arg, int input, int first, int last, int background) {
	int pipefd[2], fd_in, fd_out, status;
	char * ret;
	char ** argv;
	char ** redirection;
	pid_t pid, wpid;

	if (pipe(pipefd) == -1) {
		perror("ERROR: pipe error");
		return 1;
	}

	pid = fork();

	if(pid < 0) {
		fprintf(stderr, "ERROR: fork error");
	}
	else if (pid == 0) {
		if (first == 1 && last == 0 && input == 0)
			dup2(pipefd[1], 1);
		else if (first == 0 && last == 0 && input != 0) {
			dup2(input, 0);
			dup2(pipefd[1], 1);
		}
		else
			dup2(input, 0);

		
		if (strstr(arg, "<")) {
			redirection = tokenizeLine(arg, "<");

			ret = strdup(trim(redirection[1]));

			if ((fd_in = open(ret, O_RDONLY, 0)) < 0) {
				fprintf(stderr, "ERROR: Failed to open %s for reading\n", redirection[1]);
				return (EXIT_FAILURE);
			}
			dup2 (fd_in, 0);
			close (fd_in);
		}
		else if (strstr(arg, ">")) {
			redirection = tokenizeLine(arg, ">");

			ret = strdup(trim(redirection[1]));
			
			if ((fd_out = creat(ret, 0644)) < 0) {
				fprintf(stderr, "ERROR: Failed to open %s for writing\n", redirection[1]);
				return (EXIT_FAILURE);
			}
			dup2 (fd_out, 1);
			close (fd_out);
		}

		argv = tokenizeLine(arg, " \t\r\n\a");
		if (execvp(argv[0], argv) < 0 )
			fprintf(stderr, "ERROR: Command %s not found\n", argv[0]);
		exit(0);

	}
	else {
		if (background == 0)
		wpid = waitpid(pid, 0, 0);
	}

	if (last == 1)
		close(pipefd[0]);

	if (input != 0)
		close(input);

	close(pipefd[1]);
	
	return (pipefd[0]);
}
char* trim(char* str) {
	char* begin;
	begin = str;

	while (*begin != '\0') {
		if (isspace(*begin))
			begin++;
		else {
			str = begin;
			break;
		}
	}

	return str;
}
