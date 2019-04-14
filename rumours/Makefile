CC = clang
CFLAGS = -c -g -Wall -Wextra
LFLAGS = -Wall -Wextra


all: rumours.o
	${CC} ${LFLAGS}  rumours.o -o rumours
rumours.o: rumours.c rumours.h
	${CC} ${CFLAGS} rumours.c -o rumours.o
clean:
	rm *.o rumours
