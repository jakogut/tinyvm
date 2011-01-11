#ifndef INSTRUCTIONS_H_
#define INSTRUCTIONS_H_

#include "program.h"
#include "instruction_table.h"

void populate_instruction_table(instruction_table t);

inline void instr_mov(program* p, memory* m, stack* s, int* instr_idx);

inline void instr_push(program* p, memory* m, stack* s, int* instr_idx);
inline void instr_pop(program* p, memory* m, stack* s, int* instr_idx);

inline void instr_inc(program* p, memory* m, stack* s, int* instr_idx);
inline void instr_dec(program* p, memory* m, stack* s, int* instr_idx);

inline void instr_add(program* p, memory* m, stack* s, int* instr_idx);
inline void instr_sub(program* p, memory* m, stack* s, int* instr_idx);
inline void instr_mul(program* p, memory* m, stack* s, int* instr_idx);
inline void instr_div(program* p, memory* m, stack* s, int* instr_idx);
inline void instr_mod(program* p, memory* m, stack* s, int* instr_idx);
inline void instr_rem(program* p, memory* m, stack* s, int* instr_idx);

inline void instr_not(program* p, memory* m, stack* s, int* instr_idx);
inline void instr_xor(program* p, memory* m, stack* s, int* instr_idx);
inline void instr_or(program* p, memory* m, stack* s, int* instr_idx);
inline void instr_and(program* p, memory* m, stack* s, int* instr_idx);

inline void instr_cmp(program* p, memory* m, stack* s, int* instr_idx);

inline void instr_jmp(program* p, memory* m, stack* s, int* instr_idx);

inline void instr_je(program* p, memory* m, stack* s, int* instr_idx);
inline void instr_jne(program* p, memory* m, stack* s, int* instr_idx);

inline void instr_jg(program* p, memory* m, stack* s, int* instr_idx);
inline void instr_jge(program* p, memory* m, stack* s, int* instr_idx);

inline void instr_jl(program* p, memory* m, stack* s, int* instr_idx);
inline void instr_jle(program* p, memory* m, stack* s, int* instr_idx);

#endif
