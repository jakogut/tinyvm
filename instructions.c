#include "instructions.h"
#include "memory.h"

void populate_instruction_table(instruction_table t)
{
	t[MOV] = instr_mov;

	t[PUSH] = instr_push;
	t[POP] = instr_pop;

	t[INC] = instr_inc;
	t[DEC] = instr_dec;

	t[ADD] = instr_add;
	t[SUB] = instr_sub;
	t[MUL] = instr_mul;
	t[DIV] = instr_div;
	t[REM] = instr_rem;

	t[NOT] = instr_not;
	t[XOR] = instr_xor;
	t[OR] = instr_or;
	t[AND] = instr_and;

	t[CMP] = instr_cmp;

	t[JMP] = instr_jmp;

	t[JE] = instr_je;
	t[JNE] = instr_jne;

	t[JG] = instr_jg;
	t[JGE] = instr_jge;

	t[JL] = instr_jl;
	t[JLE] = instr_jle;
}

void instr_mov(program* p, memory* m, stack* s, int* instr_idx)
{
	*p->args[*instr_idx][0] = *p->args[*instr_idx][1];
}

void instr_push(program* p, memory* m, stack* s, int* instr_idx)
{
	stack_push(s, p->args[*instr_idx][0]);
}

void instr_pop(program* p, memory* m, stack* s, int* instr_idx)
{
	stack_pop(s, p->args[*instr_idx][0]);
}

void instr_inc(program* p, memory* m, stack* s, int* instr_idx)
{
	++(*p->args[*instr_idx][0]);
}

void instr_dec(program* p, memory* m, stack* s, int* instr_idx)
{
	--(*p->args[*instr_idx][0]);
}

void instr_add(program* p, memory* m, stack* s, int* instr_idx)
{
	*p->args[*instr_idx][0] += *p->args[*instr_idx][1];
}

void instr_sub(program* p, memory* m, stack* s, int* instr_idx)
{
	*p->args[*instr_idx][0] -= *p->args[*instr_idx][1];
}

void instr_mul(program* p, memory* m, stack* s, int* instr_idx)
{
	*p->args[*instr_idx][0] *= *p->args[*instr_idx][1];
}

void instr_div(program* p, memory* m, stack* s, int* instr_idx)
{
	int* arg0 = p->args[*instr_idx][0];
	int* arg1 = p->args[*instr_idx][1];

	m->remainder = *arg0 % *arg1;
	*arg0 /= *arg1;
}

void instr_rem(program* p, memory* m, stack* s, int* instr_idx)
{
        *p->args[*instr_idx][0] = m->remainder;
}

void instr_not(program* p, memory* m, stack* s, int* instr_idx)
{
	int* arg = p->args[*instr_idx][0];
	*arg = ~(*arg);
}

void instr_xor(program* p, memory* m, stack* s, int* instr_idx)
{
	*p->args[*instr_idx][0] ^= *p->args[*instr_idx][1];
}

void instr_or(program* p, memory* m, stack* s, int* instr_idx)
{
	*p->args[*instr_idx][0] |= *p->args[*instr_idx][1];
}

void instr_and(program* p, memory* m, stack* s, int* instr_idx)
{
	*p->args[*instr_idx][0] &= *p->args[*instr_idx][1];
}

void instr_cmp(program* p, memory* m, stack* s, int* instr_idx)
{
        int* arg0 = p->args[*instr_idx][0];
        int* arg1 = p->args[*instr_idx][1];

        m->equal = (*arg0 == *arg1);
        m->greater = (*arg0 > *arg1);
}

void instr_jmp(program* p, memory* m, stack* s, int* instr_idx)
{
	// We have to jump to the address prior to the instruction we *want* to execute
	*instr_idx = *p->args[*instr_idx][0] - 1;
}

void instr_je(program* p, memory* m, stack* s, int* instr_idx)
{
	if(m->equal) instr_jmp(p, m, s, instr_idx);
}

void instr_jne(program* p, memory* m, stack* s, int* instr_idx)
{
	if(!(m->equal)) instr_jmp(p, m, s, instr_idx);
}

void instr_jg(program* p, memory* m, stack* s, int* instr_idx)
{
        if(m->greater) instr_jmp(p, m, s, instr_idx);
}

void instr_jge(program* p, memory* m, stack* s, int* instr_idx)
{
        if(m->greater || m->equal) instr_jmp(p, m, s, instr_idx);
}

void instr_jl(program* p, memory* m, stack* s, int* instr_idx)
{
        if(!(m->greater) && !(m->equal)) instr_jmp(p, m, s, instr_idx);
}

void instr_jle(program* p, memory* m, stack* s, int* instr_idx)
{
        if(!(m->greater)) instr_jmp(p, m, s, instr_idx);
}

