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