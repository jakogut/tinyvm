#ifndef INSTRUCTION_INDEX_H_
#define INSTRUCTION_INDEX_H_

// End of the program
#define END -1

// Comment
#define COM 0

// Interrupt
#define INT 1

// Move a value to an address
#define MOV 2

// Stack instructions
#define PUSH 3
#define POP 4

// Increment/Decrement
#define INC 5
#define DEC 6

// Arithmetic
#define ADD 7
#define SUB 8
#define MUL 9
#define DIV 10	// Divide, storing the quotient in the first argument
#define MOD 11	// Divide, storing the remainder in the remainder register
#define REM 12	// Copy the remainder register to the address specified by the first argument

// Bitwise instructions
#define NOT 13
#define XOR 14
#define OR  15
#define AND 16

// Shifts
#define SHL 17
#define SHR 18

// Compare
#define CMP 19

// Conditionals
#define JMP 20

#define JE  21
#define JNE 22

#define JG 23
#define JGE 24

#define JL 25
#define JLE 26

#endif
