COMPILER = gcc
CCFLAGS = -g -Wall
LIBFLAGS = ar -cvq

all: sl

sorted-list.o: sorted-list.c
	${COMPILER} ${CCFLAGS} -o sorted-list.o -c sorted-list.c

libsl.a: sorted-list.o
	${LIBFLAGS} -libsl.a sorted-list.c

main.o: main.c sorted-list.o
	${COMPILER} ${CCFLAGS} -o main.o -c main.c

sl: main.o
	${COMPILER} -o sl main.o sorted-list.o

clean:
	rm -f *.o
