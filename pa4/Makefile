COMPILE = gcc
FLAGS = -Wall -g

all: search index

search: search.c search.h
	$(COMPILE) $(FLAGS) search.c -o search

index: indexer.c
	$(COMPILE) $(FLAGS) indexer.c -o index

clean:
	rm -f search index *.o
