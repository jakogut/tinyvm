#ifndef INSTRUCTION_INDEX_H_
#define INSTRUCTION_INDEX_H_

#define NUM_INSTRUCTIONS 23

// End of the program
#define END -1

// Move a value to an address
#define MOV 0

// Stack instructions
#define PUSH 1
#define POP 2

// Increment/Decrement
#define INC 3
#define DEC 4

// Arithmetic
#define ADD 5
#define SUB 6
#define MUL 7
#define DIV 8	// Divide, storing the quotient in the first argument
#define MOD 9	// Divide, storing the remainder in the remainder register
#define REM 10	// Copy the remainder register to the address specified by the first argument

// Bitwise instructions
#define NOT 11
#define XOR 12
#define OR  13
#define AND 14

// Compare
#define CMP 15

// Conditionals
#define JMP 16

#define JE  17
#define JNE 18

#define JG 19
#define JGE 20

#define JL 21
#define JLE 22

#endif
