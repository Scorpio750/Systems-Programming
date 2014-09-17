all: tokenizer tokenizer.c

tokenizer.o: tokenizer.c
	gcc -ansi -pedantic -Wall -c -g tokenizer.c

tokenizer: tokenizer.o
	gcc -o tokenizer tokenizer.o

clean:
	rm -f tokenizer
	rm -f *.o
