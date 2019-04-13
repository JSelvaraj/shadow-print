CC = gcc -std=c99
CFLAGS = -c -g -Wall -Wextra
LFLAGS = -Wall -Wextra -lpthread


all: task2.o
	${CC} ${LFLAGS}  task2.o -o task2
task2.o: task2.c
	${CC} ${CFLAGS} task2.c -o task2.o
clean:
	rm *.o task2
