#ifndef INSTRUCTION_INDEX_H_
#define INSTRUCTION_INDEX_H_

#define NUM_INSTRUCTIONS 22

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
#define DIV 8
#define REM 9

// Bitwise instructions
#define NOT 10
#define XOR 11
#define OR  12
#define AND 13

#define CMP 14
#define JMP 15

#define JE  16
#define JNE 17

#define JG 18
#define JGE 19

#define JL 20
#define JLE 21

#endif
