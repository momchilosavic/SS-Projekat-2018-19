	add r1, [r2]
	.word -1

	subb [r15], r2[15]
	.word -1

	mulw r15[15], sp[0xfefe]
	.word -1

label_1: .equ label_2, 1234

	div r1[0x1234], sp[label_2]
	.word -1

	mov r2[label_1], 0x9999
	.word -1

	xchg *0x1212, $label_1
	.word -1

	cmpw r1, &label_1
	.word -1

	or r1, r1
	.word -1

	shr r1, r1
	.word -1
