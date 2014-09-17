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
	
	void* malloc(size_T size);
	//changes size of previously malloc-ed memory
	void* realloc(void*ptr, size_T size); 
	//same as malloc but clears memory first
	void* calloc();
	void free(void*);
	
	// example
	char *s;
	s = (char *)malloc(strlen(argv[3] + 1);
	
- argv[0] tells you the file size
- Free everything you malloc, otherwise you create a *memory leak*

---

## 9/16/14

## Quicksort in C

- C provides a generic quicksort function, however you have to specify the following params:
	
		void qsort(void *base, \
			size_T num_elems, \
			size_T elem_size, \
			int (*compar)(const void *, const void *))
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
