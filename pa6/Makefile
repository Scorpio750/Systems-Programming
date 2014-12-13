COMPILE = gcc
FLAGS = -Wall -g 

all: tc1 tc2 tc3 tc4 tc5 tc6 tc7

tc1: tc1.c memory.c memory.h
	$(COMPILE) $(FLAGS) tc1.c -o tc1

tc2: tc2.c memory.c memory.h
	$(COMPILE) $(FLAGS) tc2.c -o tc2

tc3: tc3.c memory.c memory.h
	$(COMPILE) $(FLAGS) tc3.c -o tc3

tc4: tc4.c memory.c memory.h
	$(COMPILE) $(FLAGS) tc4.c -o tc4

tc5: tc5.c memory.c memory.h
	$(COMPILE) $(FLAGS) tc5.c -o tc5

tc6: tc6.c memory.c memory.h
	$(COMPILE) $(FLAGS) tc6.c -o tc6

tc7: tc7.c memory.c memory.h
	$(COMPILE) $(FLAGS) tc7.c -o tc7

clean:
	rm -f tc1 tc2 tc3 tc4 tc5 tc6 tc7 *.o
