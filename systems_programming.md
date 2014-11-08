# Systems Programming
# Brian Russell
## Fall 2014

---
---

## Table of Contents

1. [C Programming](#anchor1)
	1. [Structs](#anchor1.1)
	2. [Arrays](#anchor1.2)
	3. [Pointers](#anchor1.3)
	4. [Pointers to Structs](#anchor1.4)
	5. [Strings](#anchor1.5)
	6. [Quicksort in C](#anchor1.6)
	7. [Memory Manipulation](#anchor1.7)
	8. [Constants](#anchor1.8)
	9. [Libraries](#anchor1.9)
	10. [File Systems](#anchor1.10)
	11. [System Calls](#anchor1.11)
2. [Process Handling](#anchor2)
	1. [Signal Handling](#anchor2.1)
	2. [Forking Around](#anchor2.2)
	3. [Zombie Processes](#anchor2.3)
	4. [Orphan Processes](#anchor2.4)
	5. [Multi-Threading](#anchor2.5)
	6. [Mutexes](#anchor2.6)
	7. [Semaphores](#anchor2.7)

---
---

## 9/4/14

# [C Programming](id:anchor1)

## [Structs](id:anchor1.1)

- C has no *classes*, instead it has something called a **struct**

		struct Point {
			int x;
			int y;
		} A, B, C;
		
	- No visibility controls (no abstraction)
	- All structs are *public*
- no **new** operator

		struct {
			float a, b, c;
			} D = {3, 4, 5}, E;
		
		struct F = {7};
		
- F will be *partially defined*, **a** will be defined, but the other two values will garbage bits
- Can have structs within structs

		struct Rectangle {
			struct Point topLeft,
			struct Point bottomRight,
		} A = {{3,4},{7,9}};
		
## [Arrays](id:anchor1.2)

	int array[10];
	char stringArray[] = "Hello"; // = {'H', 'e', 'l', 'l', 'o', 'Ø'}
	int square[10][10] = {{0, 1, 2, 3, 4, 5},
						   {6, 7, 8, 9, 10}};
	char s[20] = "Hello";

- String arrays are *null-terminating*; they have an extra null byte at the end of all 0's
- square has an *incomplete initializer*
	
		int main() {
			int array[10], i;
		
			for (i=0; i <= 10; i++) {
				array[i] = 0;
			}
			return 0;
		}

- Because C does not check array bounds, when the for loop goes out of bounds, it will set the next item at the next memory address and potentially bad things will happen.

## [Pointers](id:anchor1.3)

	int x;
	int *px;
	int *p1, *p2, y;
	
	x = 7;
	px = &x;
	p1 = px;
	*px = 6; // x = 6
	*p1 = 6;
	p1 = 0;
	
---
	
	char **p2;
	char array[10] = "Hello";
	char *p1;
	char other[5];
	
	p1 = array;
	p1 = &array[0];
	p1[0] = 'y';
	p1[3] = 'z';
	p2 = &p1;
	*p2 = other;
	
	// p2 -> p1 -> 'H' -> 'e' -> 'l' -> 'l' -> 'o' -> '\Ø'
	
	//types
	array : char[10]
	array[3] : char
	p1 : char*
	*p1 : char
	c : char
	$c : char *
	&&c : BAD!
	*c : BAD!
---

	arglist hello welcome bienvenue
	char **argv
	
	// argv -> "arglist" -> "hello" -> welcome" -> "happy"
	
---

## 9/9/14

## [Pointers to Structs](id:anchor1.4)

	struct Point *p;
	struct Point point;
	
	p = &point;
	p -> x
	
### Unions

	Union F {
		int ipart;
		char cpart(sizeof(int));
	} u = 3;
	
	u.ipart = 37;
	
- The initializer of the union is defined as the frst member of the union
- The size of the the union is the size of the largest member of the union

### Enumeration Types

	enum boolean {False, True};
	enum Trafficlight{red, yellow, green};
	enum Trafficlight a, b, c;
	
	a = red;
	b = green;
	
	enum Months {
		jan = 1; feb, mar, apr, may, jun
		};
		
	enum boolean TF;
	int x;
	
	TF = true;
	x = TF;
	TF = x;
	TF = 5; // C lets you do this
	
### Typedefs

	typedef char *charptr;
	charptr s;
	s = "hello";
	
	typedef int length;
	length a, b;
	length *ptr;
	typedef char *string;
	string p;
	
### Functions

	int gcd(int a, int b) {
		if (b == 0)
			return a;
		else
			return gcd(b, a%b);
		}
	
- Functions in C are *pass-by-value*: they duplicate a copy of the value being sent in the receiving function.
- In order to modify the value of a variable, you must pass in a pointer to the address of that variable

		void F(int *p) {
			*p = 3;
		}
		
		x = 7;
		F(&x);
		
- Do not return a pointer to the local variable, return the local variable

### Arrow Notation

	p->x
	(*px).x
	
The above two statements are equivalent.


### Pointer Arithmetic

	int x;
	int *p;
	
	p = &x;
	
	p = p + 1;
	p = p + sizeof(x);
	
The last two statements are equivalent

#### List of Pointer Operators

	++p
	--p
	p++
	p--
	*p
	p1 = p2
	p1 == p2
	p1 != p2
	p += i
	p -= i
	p + i
	i + p
	p1 - p2
	p[i]
	
- C does not care if you put in negative subscripts

		p = &x;
		p = p + 1;
		p[-1] = 7; // x = 7
		
### Pointer to Function

	int x;
	int f(int);
	int (*pg)(int);
	int *f2(int);
	
	pg = f; // pg now points to f
	x = (*pg)(3); // assigns to x the return value of the dereferenced function
	
---

## 9/11/14

## [Strings](id:anchor1.5)

	# include <stdio.h>
	
	int printf(const char *EMT,...);
	
	"This is a char %c \n";


$$\begin{matrix} \%c \;\text{char} & \%u \;\text{unsigned integer} & \%f \;\text{float} & \%\% \;\text{percent} \\\ \%s \;\text{string} & \%o & \%g \\\ \%d \;\text{decimal} & & \%e \end{matrix}$$


### List of String Commands

	#include <string.h>
	
	strcpy(char *DST, const char *src);
	// copies first n characters from the first string into the second string 
	strncpy
	// concatenates strings
	// concatenates first n characters from first string with second string. Returns string. 	
	strcat 	
	strncat
	// same thing as in java 
	strcmp 
	
## Standard Library

	#include <stdlib.h>
	
	void* malloc(size_t size);
	//changes size of previously malloc-ed memory
	void* realloc(void*ptr, size_T size); 
	//same as malloc but clears memory first
	void* calloc(size_t count, size_t size);
	void free(void*);
	
	// example
	char *s;
	s = (char *)malloc(strlen(argv[3] + 1);
	
- argv[0] tells you the file size
- Free everything you malloc, otherwise you create a *memory leak*

---

## 9/16/14

## [Quicksort in C](id:anchor1.6)

- C provides a generic quicksort function, however you have to specify the following params:
	
		void qsort(void *base, \
			size_T num_elems, \
			size_T elem_size, \
			int (*compar)(const void *, const void *)
- In this case, the final param is a comparison function that you have to send it. The ugly syntax is necessary to cast the comparison function into a pointer to that function
	
- C has a comma operator which allows you to string a series of expressions together
	- The value of that expression is the rightmost expression

Example function:
			
	int main() {
		float array[200];
	
		qsort( array, \
			count, \
			sizeof(float), \
			(int (*)(const void *, const void *) )numcmp );
		}

### Header Files

- In a C program there are two kinds of files, source files, and header files
	- sources files are the *.c files
	- header files are included as *.h files
- **static** functions are used to prevent the modified function's access from other programs
	- **static** can be used *inside* a function to modify variables. This means that the scope of that variable is limited to the function's it's within.
	
			static void foo() {
				static int is_initialized = 0;
				
				if (!is_initialized) {
					is_initialized = 1;
					}
					//do stuff
				}

	- When foo is called again, is_initialized will be the same variable in the same space in memory; the statement execution will be moved to the top of the memory instructions by the compiler. The actual *function* does not execute this statement.

---
## 9/18/14

## [Memory Manipulation](id:anchor1.7)

	void * memcpy(void * dst, const void * src, size_t len);
	int memcmp(const void * s1, const void * s2, size_t len);
		/* Return values
		<0 if s1 < s2
		 0 if s1 == s2
		>0 s1 > s2
		
	void * memset(void * s, int c, size_t len)
		/ * sets 
		
### [Constants](id:anchor1.8)

- **const** values *must* have an initializer

		const int pi = 3.1415
		const int * ptr = &pi;
		int x = 35;
		
		ptr = &x; // NO
		*ptr = 22/7 // YES
		
		const int * const ptr = &x;
		/* cannot change the dereferenced value OR the pointer */

## Modularization

- Advantages to modularization:
	- Localization of specialization
	- Makes changes much easier because of localization
	- Manageability
	- Allows for encapsulation
	
## Files

	#include <stdio.h>
	
	FILE * fopen(const char * path, const char * mode);
	flags:
	
	-r
	-r+
	-w
	-w+
	-a
	-a+
	
	int fread(void * ptr, \
			 	size_t size, \
			 	size_t num_elems, \
			 	file * stream);
			 	
---
## 9/24/14

### Makefiles

	#CC		= cc
	CC		= gcc
	ROOT	= /grad/users/morbius/cs214
	#ROOT 	= /u
	COMPILE	= $(CC)
	
	all: buncha shit
	
	calloc5: calloc5.c
		$(COMPILE) -g -o calloc5 calloc5.c
	
	malloc5: malloc5.c
		$(COMPILE) -g -o malloc5 malloc5.c
	
	factorial: main.o factorial.o
		$(COMPILE) -g -o factorial main
	
	main.o: main.c factorial.h
		$(COMPILE) -c -g main.c
		
	factorial.o: factorial.h factorial.c
		$(COMPILE) -c -g factorial.c
	
	color: color.c
		$(COMPILE) -g -o color color.c

- Dependencies in a Makefile are *unordered*
- Your shell looks for the following files when you execute *make*:
	
		makefile
		Makefile
		s.makefile
		s.Makefile

- You can give *make* the following flags:

		make 	-i keep going even when a command fails
				-f FILENAME
				-n list all commands it will execute w/o running them

## [File Systems and their related shit](id:anchor1.9)


- inode (index node) contains meta-information about the file
	- Managed by the OS
	- Includes information such as date created, date modified, user, etc.

#### Directory library Commands

	#include <dirent.h>
	dir * opendir(const char *);
	struct dirent * readdir(dir*);
	void seekdir(dir *, long offset);
	long telldir(dir *);
	int closedir(dir *);


##### Example file

	#include <dirent.h>
	#include <errno.h>
	#include <stdio.h>
	#include <string.h>
	
	/* print files listed in a directory. */
	
	int main(int argc, char ** argv) {
		DIR * dir;
		struct dirent *entry;
		extern int errno;
		
		if (argc < 2) {
			printf("Specify dirname");
			}
		else {
			printf("begin dirinfo list");
			while ( (entry = readdir(dir)) != 0 ) {
					printf("d_ino %d, d_off %d 
				
---
## 9/30/14

## [Libraries](id:anchor1.10)

- Can be either *static* or *dynamic*
	- When you link from a *static* library, your program will pull out a separate copy of the object file information
	- When you link from a *dynamic* library, you get the object file information only when you start the process and store it in your process address space
	- *Static* libraries take up a lot of space
	
			STATIC
				gcc -c means.c
				ar -rv libmeans.a means.c
				
				---
			
			DYNAMIC
				gcc -c -FPIC meand.c
				gcc -shared - o libmeand.so meand.c

## [System Calls](id:anchor1.11)

- All operating systems provide means for direct requests from the system
	- In *nix systems, this is also called the **kernel**
	
			#include <errno.h>
			
			 char * strerror(int errnum);
			 void perror(const char * message);
	- In C, all I/O is done through file descriptors. [0] is stdin [1] is stdout, and [2] is stderr
	- Sockets are also parsed through file descriptors
	
---

## 10/2/14

## [Debugging](id:gdb)

- The -g flag enables it to be debugged
	- Used to be highly machine-specific/compiler-specific

### gdb Commands

	print <varname>	// prints value of specified varname
	n 				// evaluates and prints the next line in the program
	backtrace		// self-explanatory

### Core dumps are baaaaad

- gives you a core file; an image of a dead process
- Will give you a look at the stack frame, memory, etc.
	
---

## 10/7/14

# [Process Handling](id:anchor2)

## [Signals](id:anchor2.1)


- Signals are inter-process communication in *NIX systems

		SIGABRT	/* Abort (ANSI). */
		SIGKILL	/* Kill, unblockable (POSIX) */
		SIGUSR1	/* User-defined signal 1 (POSIX) */
		
		int SIGPROCMASK(INT, CONSTSIGSET_T * SET, SIGSET_T * OLDSET);
	
- After a signal is generated, it is now pending
- A signal is *delivered* when

### Signal Handlers 

- Signals are managed by a **signal handler**
	- A signal handler *returns*, but does not get called
	- Signal handlers are not and should not be large.
- Signal handlers are managed **asynchrously**
- The signal that causes the signal handler is *blocked* to prevent infinite recursion
- You can have multiple signal handlers for the same signal, but not at the same time

		struct sigaction {
			void (*sa_handler)(int);
			void (*sa_sigaction)(int, siginfo_t*, void*);
			SIGSET_T sigmask;
			int sa-flags; 
		};

Dangerous vvvv

	SA_NODEFER: Do not block signal in its own signal handler
	
---
## 10/9/14

### Signal Handler Uses

- Can send multiple signals to the same signal handler
- Can send timers to the signal handler, and SIGALARM will communicate with the program when the timer runs out
- Signal handlers can control changes in the program's internal *state*
- Efficient, proactive communication with other machines

		setitimer(ITIMER_REAL, ITIMER_VIRTUAL, ITIMER_PROF
		
---
## 10/14/14

## [Valgrind](id:valgrind)

- When you free a ptr that isn't malloc'ed, BAD THINGS WILL HAPPEN
- Malloc returns a pointer behind which is a speicla data structure
- When you free a ptr, it looks behind it to see what information to delink

---
## 10/16/14

## [Forking Around](id:anchor2.2)

	int main() {
		pid_t pid;
		pid = fork();
		switch(pid);
		{
			case -1: //ERROR
			case 0: //child
			default: //parent
	}

- Once you fork, the parent and child are running *asynchronously*
- Separate process addresses
- All file descriptors are duplicated
	- Position is also the same
- Once a fork() is executed, the parent has to *wait()* for the child to return

		pid_t wait(int *);
		pid_t waitpid(pid_t, 
						int *, 
						int);

- Will get *errno* back if it fails
- Returns twice

### Exit

- Exits the process
- Closes file descriptors
- If the process has children, they get returned to the __init__ function
- If a child calls exit, the parent gets sent a __sigchild__ 


### Exec

	int execl("pathname", "program name", 0)
		execv()
		execle()
		execve()
		execlp()
		execvp()
		
- Can execute different programs in process

		//execl() envarg test program
		int main(int argc, char ** argv) {
			printf("Process %s PID %d invoking envarg\n", argv[0], getpid() );
			execl( "/grad/users/morbius/cs214/envarg", "envarg", 0);
		}
		

- int main() can actually take a 3rd argument, char ** envp
	- Retrieves environment variables
- Exec does not return values unless there is an error, in which case it returns -1
- It will return to the shell, not the parent process

---

## 10/21/14

## [ZOMG ZOMBIES](id:anchor2.3)

- A zombie process is a transient state
	- Has gone from running to not running

## [OMG ORPHANS](id:anchor2.4)

- Orphans are different from zombies
	- An *orphan* is usually a child process still running whose parent process has died without waiting on it
	- In such a case a speciail *init* process will *re-parent* the child process
	
## Command Line Commands

	// create and maintain library archives
	ar <key> libfile obj1.o obj2.o ... 
		flags <-r -t -d -v- c>
		-r 
		-t // table of contents of static shared library
		-d // delete files from static shared library
		-v // verbose
		-c // suppress messages

	// display a calender 
	cal <<month> year>
	
	// concatenates and prints files
	cat <file 1> <file 2>
	// destroys/creates file 1 and puts contents of file 2 into 1
	cat file 1 file 2 > file 1
	
	//change permissions but not ownership
	chmod <mode> <file>
		rwx	rwx	rwx
		7	7	7
		rwx	r-x	r-x
		7	5	5
		rw-	r--	---
		6	4	0

If permissions are prefixed by a d when using *ls*, that means the file is a directory

	
	chown <owner> <file> ...
	chgrp <group> <file> ...

	// hard-link files together 
	ln [-s] file target-file
	ln [-s]
	
Hard links cannot link across file systems, but soft links can
### [ASK RUSSELL]

	find <path> <expression list>
			|usually .	|
						|-name "*.h"
						|-type df // depth-first traversal
						|-print
						|-depth
						|-exec cmd args {} \;
	
	grep <flags> <expression> <file>...
			|-i case insensitive
			|-n print line number of file
			|-l print file name
			|-v prints all lines that don't contain the regex
	
### Regular Expressions:
		$
		*
		[]
		^
		|
		.

### Other Command-Line Arguments
	
	kill -signo pid...
	
	ls <flags>
		| -a 
		| -l
		| -F will put a / after directories
	
	// looks at a file without opening it a page at a time
	pg <file>...
	more <file>...
	
	// see all processes running
	ps
		-e
		-f // verbose
		-u morbius // see for that user
		
	rm <flags> <filename>
		|-i requests confirmation
		|- f force command, overrides -i
	
	sort 
		|-u with -c check for strict ordering
		|-r reverse
		|-f ignore lower-case
		
---

## 10/23/14

## [Multi-Threading](id:anchor2.5)

- multiple threads of execution
- **Control Flow** is separate
- Threads exist in a shared address space
- Every *thread* has its own **register set**, **program counter** and **call stack**
- Every thread also has its own **thread id** or *tid*
- ***Multi-threading* is *ideal* for networking (client-server) architecture**
- No thread can ever change the registers or stack of another 

### Benefits

- If some threads get stopped or blocked, other threads can continue executing
- Threads are cheaper to create than processes
	- between 10-100 times faster
- Switching b/w threads is cheaper and faster than switching b/w processes
- Can take advantage of multi-core processors
	- Can get threads to run on different processors in true *parallel* processing

## Thread Scheduling

- Different levels: **user** and **kernel**-level threads
	- Kernel-level threads handled by kernel and not the processor itself
- Certain overhead in managing/scheduling different threads
- How do I write a multi-client server? **[Ask Russell]**
- User-level threads (scheduling and ish) all run as library code inside your process

### Disadvantages

- When one user-level thread gets blocked, all of them get blocked
	- A blocking system call will stop all user-level threads
		- **Signals** send to processes, not to threads
- *However*, if one kernel-level thread gets blocked, the other kernel-level threads will continue executing
- Kernel-level threads can handle hardware commands better
	- Run a little bit slower than user-level
- You make kernel-level threads *by default*

### Syntax

	#include <pthread.h>
		
		// fields
		pthread_t		tid;
		pthread_atr_t	attr;
		struct gcd *	gcdptr;
		unsigned int *	rptr;
		unsigned int	error;
		
		// creates thread
		pthread_create
		pthread_join - blocking call: stop calling process until thread to itself is done
		pthread_detach - prevents join on the specified thread
		pthread_yield
		pthread_threadexit

### The Dynamic Memory Covenant

- The parent thread allocates the parameter struct, the child thread extracts data from the struct and frees it
- For return values, the child allocates and the joining thread does the free()

---
## [Mutexes](id:anchor2.6)

- Mutexes are used to lock shared data
- Default state is unlocked
- The first thread that calls pthread_mutex_lock locks it and returns immediately
	- Subsequent threads will have to wait until the mutex becomes unlocked
- A data structure of non-trivial complexity (linked list, hash table) is **inconsistent** when it is being modified
	- In such a case, a mutex should **lock** the relevant execution control flows regarding the specified data from being modified by other threads with *pthread_mutex_lock*
	
## Deadlock

- A **deadlock** occurs when you have one or more threads waiting for omsone else to unlock some shared resource. There are 4 conditions for a deadlock, also known as **Coffman conditions**

	1. Mutual Exclusion
	2. Circular Wait
	3. Hold and Wait
	4. No Preemption
- You can avoid deadlocks by always locking shared resources in the same order
	
---
## 11/3/14

## [Semaphores](id:anchor2.7)

- Asynchronization mechanism
- 3 components:
	1. Flag
	2. Queue of suspended threads
	3. Nonnegative Counter (instd. of an owner like a mutex)
	
```
#include <semaphore.h>

int sem_init(sem_t *, int, unsigned int);
int sem_destroy(sem_t *);
int sem_wait(sem_t *);
int sem_post(sem_t *);
```

- Middle argument is a flag signifying whether the semaphore will be in shared memory
- Third argument is a counter
- If counter is 1
- If suspended thread in queue, dequeue + make it runnable
- Anyone can post to a semaphore (no ownership)
- Options for *sem_wait*:
	- If counter $$$
\geq$$$ 1, let one thread through, decrement counter by 1
	- If counter = 0, suspend calling thread and put into queue
- You increment the counter every time you post



**Asynch-Signal-Safe**

- Something you can build inside a signal handler

### Things you cannot do in a sighandler

- Create or exit a hread
	- You don't know which thread you're exiting
- Lock/unlock or init/destroy a mutex
- Same thing for a condition variable
- Invoke pthread
- Invoke most semaphore fns.

### Things you can do in a sighandler

- Post to a semaphore
- Sem_post if asynch-sig-safe
- Anything labeled asynch-sig-safe

### Differences b/w Semaphores and Mutexes

- Mutexes can only be locked/unlocked by their owner
	- Anyone can post to a semaphore
- Mutexes only have two states: locked and unlocked
	- Semaphore can take on $$$\mid \mathbb N \mid$$$ states
		- With the right counter value it can have multiple concurrent accessors