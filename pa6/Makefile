COMPILE = gcc
FLAGS = -Wall -g 

all: memory

bookorder: memory.c memory.h
	$(COMPILE) $(FLAGS) memory.c -o memory

clean:
	rm -f memory *.o
