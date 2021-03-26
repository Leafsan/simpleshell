#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simple_shell.h"

char * readLine(void) {
	int buffer_size = BUF_SIZE;	//set buffer size by BUF_SIZE at header file.
	char * input = malloc(buffer_size * sizeof(char));	//allocate memory for words.
	int i = 0;
	char c;

	if (input == NULL) {	//input error
		fprintf(stderr, "error: malloc failed.\n");
		exit(1);
	}

	while ((c = getchar()) != '\n') {
		if (i >= buffer_size) {
			buffer_size = 2 * buffer_size;
			input = realloc(input, buffer_size);	//memory reallocate for long words
		}

		input[i++] = c;
	}

	input[i] = '\0';	//end of line

	return input;
}
