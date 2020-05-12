# CPU-Project

GitHub repository for summer term design project.

# General Details

**Deadline** -> 14th June
### Deliverables:

**Report (50%)**

 - Formal documentation of all the technical and non-technical work.
 - By this time, all design decisions have been made and documentation of system performance is possible.
 - Should have a clear plan for any work outstanding before you can complete the demonstration. 
 

**Video Demonstration (50%)**
Explains final design and includes a demonstration.

**Code and source files for plagiarism checking**

# CPU Requirements

## Basics

 - Describe the CPU using block schematics and Verilog Modules and simulate using Quartus Prime. 
 - Need to Implement benchmark algorithms in the machine code for ISA. 
 - CPU word length = 16 bits
 - Memory minimum requirements -> 2K words of instruction and data each(32K bits)
 - Instruction and data memory units can be separated or combined. 

## Task 1 - Calculate Fibonacci Numbers using Recursion
This benchmark requires a **stack** to keep track of all the nested intermediate data values in a *recursive* implementation of the `fib() ` function. 
The **stack** is a *history of all the functions* the CPU is currently executing, so that when it finishes a function it can return to where it left off, including all the local variables that were being used.
Can be implemented with custom hardware or with normal data memory, designing appropriate instructions in your ISA. 
``` c++
int fib(const int n){ 
	int y; 
	if (n <= 1){ y = 1;} 
	else { 
		y = fib(n-1); 
		y = y + fib(n-2); 
		return y; 
	}
}
```

## Task 2 - Calculate Pseudo-Random Integers with a Linear Congruential Generator (LCG)
The **LCG** computes the sequence: $x_{n+1} = (ax_n +b) \;{\rm mod}\; 2^N$ which generates random numbers in the range $[0 \to (2^N-1)]$ for suitably chosen values of $a$ , $b$ and $N$. Typically, $N =$ `computer word length` so that the modulus operation is simply truncating the binary representation as word length is a power of 2. 

This operation requires **multiplication**. 

 - Many ways to implement in either hardware or software
 - But Verilog IP and Verilog multiply operators are **NOT** allowed.
 ``` c++
 int lcong(const unsigned int a; const unsigned int b; const int n; const unsigned int s) {
	unsigned int y = s; 
	unsigned int sum = 0; 
	for (int i = n ; i > 0; i--){ 
		y = y*a + b // calculate the new pseudo-random number 
		sum = sum + y // add it to the total } return sum; 
		}
}
  ```

Typical values in this code would have $a=25385, b =3, n=8$.
You might try to investigate which values of a, b lead to optimal generators, achieving the longest possible sequence before it repeats. Parameter $s$ is a seed – it defines *a starting point for the sequence.*

## Traverse a Linked List to Find an Item
This example steps through **items in a linked list** and **search for a given value.** Since each item in the list is reached by a *pointer* in the previous item, the CPU must use an efficient form of *indirect addressing* to traverse the data.
``` c++ 
typedef struct item{ 
	int value; 
	struct item *next; 
	} 
item_t; item_t* find(const int x, item_t* head){ 
	while (head->value != x){ 
		head = head->next; 
		if (head == NULL){break;}; 
		return head; 
		}
```
You will need to create a **linked list** in your RAM initialisation data to test this algorithm. A typical problem here would consist of a list of *length 10 nodes.*

## Evaluation
### Correctness
- Writing the benchmark algorithms in assembler  and running them on trial data. 
- Compare to results calculated with a paper analysis

### Speed
- Evaluated counting the *number of CPU cycles* required for the benchmarks and how the figure changes based on **problem size** and **implementation design**. 
- Use Quartus to find **maximum clock speed of design.** `Need to get detailed instructions`, hence calculating ***minimum execution time*** in microseconds. 
- When trading off performance of each circuit you should minimise the **geometric mean time** $(T_1,T_2,T_3)^{(1/3)}$ where $T_1,T_2,T_3$ are the times of each algorithm. 

### Power Consumption
- Relates approximately to *number of logic gates* and the *clock speed.* 
- Find the **number of logic gates** to estimate the overall power consumption. 
