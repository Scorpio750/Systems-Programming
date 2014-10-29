COMPILE = gcc
FLAGS = -Wall -g

all: index

index: indexer.c
	$(COMPILE) $(FLAGS) indexer.c -o index

clean:
	rm -f index *.o