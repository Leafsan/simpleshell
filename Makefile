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
