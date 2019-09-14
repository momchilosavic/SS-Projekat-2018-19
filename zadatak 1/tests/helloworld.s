	.data

string: 
	.asciiz "Zdravo svete!\n"
string_end:
	.equ duzina, 15

	.text
	.extern printf
	.extern exit
	.global _start
_start:
	push duzina
	push &string
	push 0
	call printf
	add sp, 12

	push 0
	call exit

	.end

