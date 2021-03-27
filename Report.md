# 한양대학교 ERICA Operating Systems Project 보고서
기계공학과 2015040719 최우경


## 기본 설명
이전에 다른 강의에서 셸 프로그래밍을 해본 적이 있기에 해당 내용을 기반으로 다시 프로그래밍을 하였다. 알고리즘을 설명하기 전에 셸의 개념과 그 기능에 대해서 간략하게 설명할 것이다.

## 셸의 개념
### 셸 기능
셸은 크게 다음 세단계로 동작한다.

1. Initialize : 일반적으로 configuration을 읽어오고 지정된 환경을 불러들여 적용하는 단계이다.
2. Interpret : 실질적으로 입력된 명령문을 읽고 수행하게 된다.
3. Terminate : 종료 명령문을 받으면 종료한다.

위 기능들 중 Interpret가 실질적으로 Shell이 하는 기능을 담고 있다고 보면 된다.

### Interpret의 구성
Interpret는 다음과 같은 기능을 가지고 있다.
1. read : 명령문을 읽어들인다.
2. parse : 명령문을 parsing해서 단어로 분할한다.
3. execute : 명령문을 수행한다.

---

## Simple Shell

### 기능요약
내가 만든 셸은 다음 기능을 행한다.

* 프롬프트 출력: 현재 디렉토리를 읽어내고 명령문 수행이 끝나면 반복적으로 출력하게 된다.
* 명령문 수행: 셸의 기본적인 기능대로 명령문을 read, parse, execute한다. 개중에 Builtin 명령문은 exit와 작동을 시험하기 위해 만든 test 뿐이다.
* 파이프라인: 이전 명령문의 출력을 파이프라인을 통해 그 다음 명령문으로 전달한다.
* 리다이렉션: 파일 디스크립트를 통해 파일을 입력하거나 출력한다.
* 백그라운드: 프로세스를 백그라운드 상에서 수행할 수 있도록 한다.

### 알고리즘 요약
<img src="https://raw.githubusercontent.com/Leafsan/simpleshell/master/reports/%EC%95%8C%EA%B3%A0%EB%A6%AC%EC%A6%98%20%EC%B0%A8%ED%8A%B8%ED%94%8C%EB%A1%9C%EC%9A%B0.png">
위는 내가 만든 셸 알고리즘의 차트플로우를 간략하게 나타낸 것이다. 완전하지는 않지만 해당 알고리즘으로 충분하다고 가정하고 개발을 시작했다.
기본적으로 위의 흐름대로 동작하지만 자세한 설명은 뒤에 설명한다.

#### 알고리즘 흐름
1. 초기화
  셸에서 필요한 내용을 초기화한다. 그 중에는 명령문을 입력받았을 때 저장하는 버퍼도 포함하고 있다.
2. 명령문 입력 및 저장
  프롬프트를 출력하고난 후, 사용자가 입력한 명령문을 버퍼에 저장하다. 이후 버퍼는 파싱을 위해 전달된다.
3. 명령문 파싱
  저장한 명령문을 우선 "|"를 기준으로 파싱한다. |로 파싱된 명령문 개수를 세서 이후 전달될 명령문 개수를 확인한다. 또한, "&" 문자가 끝에 존재하는지 확인하고 존재하는 경우에는 해당 명령문을 백그라운드 프로세스로 실행하라는 플래그를 설정한다.
  이후, 파싱되어 "|"를 기준으로 구분된 명령문들은 문자열 포인터로 저장되어 구분 후 실행된다.
4. 명령문 구분
  파싱된 명령문을 받은 실행자는 해당 명령문이 built-in 명령문인지 확인하고 built-in일 경우에는 실행한다. built-in 명령문에 대해서 좀더 첨언하자면 해당 명령문들의 경우에는 리다이렉션이나 백그라운드 등의 동작을 하지 못하게 되어있고 fork()도 되지 않는다. exit 명령문만 존재한다는 점을 생각해서 해당 명령문만 존재한다는 가정하에 만들어버린 점이 조금 아쉽다. 그러나 여전히 소스코드 내에 추가적인 명령문을 built-in할 수 있는 구조로 만들어두긴 했다.
  만약, 위 내용에서 built-in 명령문이 아닐 경우에는 해당 명령문은 execvp()로 넘어가게 된다. 이 단계에서는 명령문이 저장된 문자열 포인터에서 문자열을 읽어내고 해당 문자열을 우선 ">"나 "<"로 구분한다. 해당 문자가 존재할 경우, 리다이렉션을 지정하고 그렇지 않을 경우에는 공백 문자를 기준으로 다시 문자열을 잘라내서 execvp()로 전달하게 된다.
5. 명령문 실행
  built-in의 경우에는 fork()없이 실행되고 그 외의 경우에는 fork()와 함께 자식 프로세스 상에서 실행되게 된다. 모든 명령문의 처리가 끝나면 다시 입력을 받는 대기 상태가 된다. 단, exit 명령문의 경우에는 셸을 종료시키게 된다.

---
## 소스파일
### 기본 파일 설명
* Makefile: make를 행함.
* Executer.c: 파싱된 명령문을 실행하고, built-in 명령문들이 포함되어있다.
* Parser.c: 저장된 명령문을 파싱함.
* PromptPrinter.c: 지속적으로 터미널에 프롬프트를 출력함.
* WordReader.c: 입력된 명령문을 읽고 버퍼에 저장함.
* simpleshell.c: main 함수가 존재한다. 내부적인 기능은 존재하지 않으며, 다른 파일에서 함수를 불러와서 기능대로 동작시킨다.
* simpleshell.h: 다른 C 파일들의 내용물을 받아서 전달한다.

### 소스코드
* Makefile
```
# Makefile by 2015040719 Choi Wookyung

CC = gcc
STRIP = strip

SRC1 = simple_shell
SRC2 = PromptPrinter
SRC3 = WordReader
SRC4 = Parser
SRC5 = Executer

all: simpleShell

simpleShell : $(SRC1).o $(SRC2).o $(SRC3).o $(SRC4).o $(SRC5).o
	$(CC) -o simpleShell $(SRC1).o $(SRC2).o $(SRC3).o $(SRC4).o $(SRC5).o
	$(STRIP) simpleShell

$(SRC1).o: $(SRC1).c
	$(CC) -c $(SRC1).c

$(SRC2).o: $(SRC2).c
	$(CC) -c $(SRC2).c

$(SRC3).o: $(SRC3).c
	$(CC) -c $(SRC3).c

$(SRC4).o: $(SRC4).c
	$(CC) -c $(SRC4).c

$(SRC5).o: $(SRC5).c
	$(CC) -c $(SRC5).c

clean:
	rm -f *.o simpleShell
```
* Executer.c
```
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
```
* Parser.c
```
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
```
* PromptPrinter.c
```
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
```
* WordReader.c
```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simple_shell.h"

/*
 *	Function readLine get input chracters as strings
 */
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
```
* simpleshell.c
```
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
		fflush(stdout);
		
		args = readLine();	/* Read command line arguments */
		
		if(args[0]!=NULL)
			execute(args);

	}

	//Shutdown or Cleanup part.

	free(args);

	return EXIT_SUCCESS;
}
```
* simpleshell.h
```
/*
 *	Constants list
 */
#define PATH_LEN		150		//Length of directory path buffer
#define BUF_SIZE		1024	//size of buffer
#define TOK_BUFSIZE		64		//size of command token
#define FILE_NAME_LEN	32		//Length of file name

/*
 *	Funtions list that uses often
 */
int printPrompt();
char * readLine();
char * trim();
char ** tokenizeLine();
int execute();
```
