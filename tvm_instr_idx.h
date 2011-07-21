#ifndef INSTRUCTION_INDEX_H_
#define INSTRUCTION_INDEX_H_

// End of the program
#define END -0x1

// Interrupt
#define INT 0x0

// Move a value to an address
#define MOV 0x1

// Stack instructions
#define PUSH 0x2
#define POP 0x3

// Increment/Decrement
#define INC 0x4
#define DEC 0x5

// Arithmetic
#define ADD 0x6
#define SUB 0x7
#define MUL 0x8
#define DIV 0x9	// Divide, storing the quotient in the first argument
#define MOD 0xA	// Divide, storing the remainder in the remainder register
#define REM 0xB	// Copy the remainder register to the address specified by the first argument

// Bitwise instructions
#define NOT 0xC
#define XOR 0xD
#define OR  0xE
#define AND 0xF

// Shifts
#define SHL 0x10
#define SHR 0x11

// Compare
#define CMP 0x12

// Conditionals
#define JMP 0x13

#define JE  0x14
#define JNE 0x15

#define JG 0x16
#define JGE 0x17

#define JL 0x18
#define JLE 0x19

#endif
