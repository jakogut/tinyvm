extern printf

	SECTION .data
	str:	db "%i", 10, 0

	SECTION .text

	global main
main:
	; Push the base pointer onto the stack, and move the stack's address to EBP
	push ebp
	mov ebp, esp

	mov ebx, 1000000000
	xor edx, edx
	mov eax, 3
loop0:	add edx, eax
	add eax, 3
	cmp eax, ebx
	jl loop0
	mov eax, 5
loop1:	add edx, eax
	add eax, 15
	cmp eax, ebx
	jl loop1
	mov eax, 10
loop2:	add edx, eax
	add eax, 15
	cmp eax, ebx
	jl loop2

	; Push our arguments onto the stack
	push edx
	push dword str
	call printf

	; Remove that shit from the stack
	add esp, byte 8

	; Pop the caller's base pointer back to EBP
	pop ebp
	mov eax, 0
	ret


