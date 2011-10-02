CC=gcc -Wall -g

all: cs350sh

cs350sh: main.o
	$(CC) -o cs350sh main.o
	
main.o: main.c
	$(CC) -c main.c
	
clean:
	rm -f *.txt cs350sh
