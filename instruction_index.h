#ifndef INSTRUCTION_INDEX_H_
#define INSTRUCTION_INDEX_H_

#define NUM_INSTRUCTIONS 25

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

// Shifts
#define SHL 15
#define SHR 16

// Compare
#define CMP 17

// Conditionals
#define JMP 18

#define JE  19
#define JNE 20

#define JG 21
#define JGE 22

#define JL 23
#define JLE 24

#endif
