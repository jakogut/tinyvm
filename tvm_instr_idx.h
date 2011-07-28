#ifndef INSTRUCTION_INDEX_H_
#define INSTRUCTION_INDEX_H_

/* End of the program */
#define END -0x1

/* Interrupt */
#define INT 0x0

/* Move a value to an address */
#define MOV 0x1

/* Stack instructions */
#define PUSH 0x2
#define POP 0x3

#define PUSHF 0x4
#define POPF 0x5

/* Increment/Decrement */
#define INC 0x6
#define DEC 0x7

/* Arithmetic */
#define ADD 0x8
#define SUB 0x9
#define MUL 0xA
#define DIV 0xB	/* Divide, storing the quotient in the first argument */
#define MOD 0xC	/* Divide, storing the remainder in the remainder register */
#define REM 0xD	/* Copy the remainder register to the address specified by the first argument */

/* Bitwise instructions */
#define NOT 0xE
#define XOR 0xF
#define OR  0x10
#define AND 0x11

/* Shifts */
#define SHL 0x12
#define SHR 0x13

/* Compare */
#define CMP 0x14

/* Conditionals */
#define JMP 0x15

#define JE  0x16
#define JNE 0x17

#define JG 0x18
#define JGE 0x19

#define JL 0x1A
#define JLE 0x1B

#endif
