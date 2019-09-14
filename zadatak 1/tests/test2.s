.section .labels_section, "a"
	label1:
		.skip 3
	label2:
		.align 8
	label3:
		.word -1, 0, -1
	label4:
		.byte 127, 0, 127, 0

.section .absolute_addresing, "awx"
	.equ equ_label, 4660

	movw r1, equ_label
	add *0x1111, 9999
	subb label1, 0x1234

.section .immed_addressing, "awx"
	.equ immed_label, 4660

	cmp r1, 0xcc
	push immed_label
	mulb r1, &label1

.section .reg_addressing, "axw"
	addb r1h, [r1]
	movb r1[3], r2l
	sub sp[immed_label], r14[0xffff]
	pop $label2

.section .mem_addressing, "wxa"
	int *0x5555
	add label1, *0x7654

	
		
