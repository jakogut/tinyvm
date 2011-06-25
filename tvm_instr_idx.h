#ifndef INSTRUCTION_INDEX_H_
#define INSTRUCTION_INDEX_H_

#define NUM_INSTRUCTIONS 25

// End of the program
#define END -1
#define COM 0

// Move a value to an address
#define MOV 1

// Stack instructions
#define PUSH 2
#define POP 3

// Increment/Decrement
#define INC 4
#define DEC 5

// Arithmetic
#define ADD 6
#define SUB 7
#define MUL 8
#define DIV 9	// Divide, storing the quotient in the first argument
#define MOD 10	// Divide, storing the remainder in the remainder register
#define REM 11	// Copy the remainder register to the address specified by the first argument

// Bitwise instructions
#define NOT 12
#define XOR 13
#define OR  14
#define AND 15

// Shifts
#define SHL 16
#define SHR 17

// Compare
#define CMP 18

// Conditionals
#define JMP 19

#define JE  20
#define JNE 21

#define JG 22
#define JGE 23

#define JL 24
#define JLE 25

#endif
