#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>	//isspace()
#include "simple_shell.h"

char ** tokenizeLine(char *, char *);
char * trimLeft(char*);
char * trimRight(char*);
char* trim(char*);

/*
 *  Function "tokenizeLine" receives two inputs.
 * 	First input args is strings.
 *	Second input symbol is character.
 *	It divides args by symbol inputs
 */	

char ** tokenizeLine(char * args, char * symbol) {
	int bufsize = TOK_BUFSIZE, position = 0;
	char ** tokens = malloc(bufsize * sizeof(char*));	//memory allocate for tokens. tokens saves strings that divided by symbol
	char * token;	//divided symbol

	if (!tokens) {
		fprintf(stderr, "ERROR: allocation error\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(args, symbol);	//divide first time
	while (token != NULL) {
		tokens[position] = token;
		position++;

		if (position >= bufsize) {	//if there is lack of memory, reallocte memory for tokens
			bufsize += TOK_BUFSIZE;
			tokens = realloc(tokens, bufsize * sizeof(char*));

			if(!tokens) {
				fprintf(stderr, "ERROR: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}

		token = strtok(NULL, symbol);	//divide
	}
	tokens[position] = NULL;	//last pointer is NULL
	
	return tokens;
}

/*
 *	Function trimLeft erases blanks in front of strings 
 */
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
/*
 *	Function trimRight erases blanks behind strings
 */
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
/*
 *	Function trim erases blanks both side of strings by performing trimLeft and trimRight once
 */
char* trim (char *str) {
	return trimRight(trimLeft(str));
}
