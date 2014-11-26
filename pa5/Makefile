COMPILE = gcc
FLAGS = -Wall -g -lpthread

all: bookorder

bookorder: bookorder.c bookorder.h
	$(COMPILE) $(FLAGS) bookorder.c -o bookorder

clean:
	rm -f bookorder *.o
