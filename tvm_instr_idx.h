#ifndef INSTRUCTION_INDEX_H_
#define INSTRUCTION_INDEX_H_

// End of the program
#define END -0x1

// Comment
#define COM 0x0

// Interrupt
#define INT 0x1

// Move a value to an address
#define MOV 0x2

// Stack instructions
#define PUSH 0x3
#define POP 0x4

// Increment/Decrement
#define INC 0x5
#define DEC 0x6

// Arithmetic
#define ADD 0x7
#define SUB 0x8
#define MUL 0x9
#define DIV 0xA	// Divide, storing the quotient in the first argument
#define MOD 0xB	// Divide, storing the remainder in the remainder register
#define REM 0xC	// Copy the remainder register to the address specified by the first argument

// Bitwise instructions
#define NOT 0xD
#define XOR 0xE
#define OR  0xF
#define AND 0x10

// Shifts
#define SHL 0x11
#define SHR 0x12

// Compare
#define CMP 0x13

// Conditionals
#define JMP 0x14

#define JE  0x15
#define JNE 0x16

#define JG 0x17
#define JGE 0x18

#define JL 0x19
#define JLE 0x1A

#endif
