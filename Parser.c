#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "simple_shell.h"

char ** tokenizeLine(char *, char *);
char * trimLeft(char*);
char * trimRight(char*);
char* trim(char*);

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
char* trimLeft(char* str) {
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
char* trimRight(char* str) {
	char t[64];
	char * end;

	strcpy(t, str);
	end = t + strlen(t) -1;
	while (end != t && isspace(*end))
		end--;
	*(end+1) = '\0';
	str = t;

	return str;
}
char* trim (char *str) {
	return trimRight(trimLeft(str));
}
