#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simple_shell.h"

char ** tokenizeLine(char *, char *);

char ** tokenizeLine(char * args, char * symbol) {
	int bufsize = TOK_BUFSIZE, position = 0;
	char ** tokens = malloc(bufsize * sizeof(char*));
	char * token;

	if (!tokens) {
		fprintf(stderr, "ERROR: allocation error\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(args, symbol);
	while (token != NULL) {
		tokens[position] = token;
		position++;

		if (position >= bufsize) {
			bufsize += TOK_BUFSIZE;
			tokens = realloc(tokens, bufsize * sizeof(char*));

			if(!tokens) {
				fprintf(stderr, "ERROR: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}

		token = strtok(NULL, symbol);		
	}
	tokens[position] = NULL;
	
	return tokens;
}

/*void elimChr(char* str, char ch) {

	for (; *str!='\0'; str++) {
		if (*str == ch) {
			strcpy(str, str + 1);
			str--;
		}
	}
}*/
