#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "simple_shell.h"

/*
 *	Function printPrompt prints prompt parts
 */
int printPrompt() {
	char * buffer = malloc(PATH_LEN);	/* Dynamic memory allocation for buffer */
	char * path = getcwd(buffer, PATH_LEN);	/* Get path of directory */

	if(path != 0)
		fprintf(stdout, "2015040719@osh:%s> ", buffer);	/* Prompt print with my Student ID*/
	free(buffer);	/* free the buffer */

	return 0;
}
