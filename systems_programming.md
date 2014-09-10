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
	length* ptr;
	typedef char* string;
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
	int f(int)
	int (*pg)(int);
	int *f2(int);
	
	pg = f;
	x = (*pg)(3);
	x = pg(3);