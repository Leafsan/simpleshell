#define PATH_LEN		150		//Length of directory path buffer
#define BUF_SIZE		1024	//size of buffer
#define TOK_BUFSIZE		64		//size of command token
#define FILE_NAME_LEN	32		//Length of file name

int printPrompt();
char * readLine();
char * trim();
char ** tokenizeLine();
int execute();
