#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>	/* cannot use on Winows OS */
#include "simple_shell.h"

int main(void) {	
	//configuration	function or variables part.

	char * args; 			/* command line arguments */
	int should_run = 1; 	/* flag to determine when to exit program */

	//Run command loop
	while (should_run) {	/* actually i didn't used should_run */

		printPrompt();		/* Function prints prompt with directory and name */
		fflush(stdout);		/* flush all buffer. if you don't flush stdout buffer, prompt may not be printed. */
		
		args = readLine();	/* Read command line arguments */
		
		if(args[0]!=NULL)
			execute(args);

	}

	//Shutdown or Cleanup part.

	free(args);

	return EXIT_SUCCESS;
}
