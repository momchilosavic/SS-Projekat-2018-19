#ifndef _INSTRUCTION_H_
#define _INSTRUCTION_H_

typedef enum Registers{
	R0 	= 	0x0,			// 0	-	0000
	R1 	= 	0x1, 	// 1	-	0001 "r1h", "r1l",
	R2 	= 	0x2, 	// 2	-	0010 "r2h", "r2l",
	R3 	= 	0x3, 	// 3	-	0011 "r3h", "r3l",
	R4 	= 	0x4, 	// 4	-	0100 "r4h", "r4l",
	R5 	= 	0x5, 	// 5	-	0101 "r5h", "r5l",
	R6 	= 	0x6, 	// 6	-	0110 "r6h", "r6l", "sp",
	R7 	= 	0x7, 	// 7	-	0111 "r7h", "r7l", "pc",
	PSW 	= 	0xF,		// 15	-	1111
	data_in = 0xFF00,
	data_out = 0xFF02,
	timer_cfg = 0xFF10
} Register;

typedef enum OpCodes{
	HALT 	= 	0x01,	// 1	-	0 0001		//	stop execution
	XCHG 	=	0x02,	// 2	-	0 0010		//	xchg dst, src	-	temp <= dst; dst <= src; src <= temp;
	INT 	=	0x03,	// 3	-	0 0011		//	int dst			-	push psw; pc <= mem16[(dst mod 8) * 2];
	MOV 	= 	0x04,	// 4	-	0 0100		//	mov dst, src	-	dst <= src									|	Z, N
	ADD 	= 	0x05,	// 5	-	0 0101		//	add dst, src	-	dst <= dst + src							|	Z, N, C, O
 	SUB 	= 	0x06,	// 6	-	0 0110		//	sub dst, src	-	dst <= dst - src							|	Z, N, C, O
	MUL 	= 	0x07,	// 7	-	0 0111		//	mul dst, src	-	dst <= dst * src							|	Z, N
	DIV 	= 	0x08,	// 8	-	0 1000		//	div dst, src	-	dst <= dst / src							|	Z, N
	CMP 	= 	0x09,	// 9	-	0 1001		//	cmp dst, src	-	temp <= dst - src							|	Z, N, C, O
	NOT 	= 	0x0A,	// 10	-	0 1010		//	not dst			-	dst <= ~dst									|	Z, N
	AND 	= 	0x0B,	// 11	-	0 1011		//	and dst, src	-	dst <= dst & src							|	Z, N
	OR		=	0x0C,	// 12	-	0 1100		//	or dst, src		-	dst <= dst | src							| 	Z, N
	XOR 	= 	0x0D,	// 13	-	0 1101		// 	xor dst, src	-	dst <= dst ^ src							| 	Z, N
	TEST 	= 	0x0E,	// 14	-	0 1110		//	test dst, src	-	temp <= dst & src							|	Z, N
	SHL 	= 	0x0F,	// 15	-	0 1111		// 	shl dst, src	-	dst <= dst << src							|	Z, N, C
	SHR 	= 	0x10,	// 16	-	1 0000		//	shr	dst, src	-	dst <= dst >> src							|	Z, N, C
	PUSH 	= 	0x11,	// 17	-	1 0001		//	push src		-	sp <= sp - 2; mem16[sp] <= src				
	POP 	= 	0x12,	// 18	-	1 0010		//	pop src			-	sp <= mem16[sp]; sp <= sp + 2
	JMP 	= 	0x13,	// 19	-	1 0011		//	jmp dst			-	pc <= dst
	JEQ 	= 	0x14,	// 20	-	1 0100		//	jeq dst			-	if(flags.Z == 1) then pc <= dst end_if
	JNE 	= 	0x15,	// 21	-	1 0101		//	jne dst			-	if(flags.Z == 0) then pc <= dst end_if
	JGT 	= 	0x16,	// 22	-	1 0110		//	jgt dst			-	if(flags.N == 0) then pc <= dst end_if
	CALL 	= 	0x17,	// 23	-	1 0111		//	call dst		-	push psw; pc <= dst
	RET 	= 	0x18,	// 24	-	1 1000		//	ret				-	pop pc;
	IRET 	= 	0x19,	// 25	-	1 1001		//	iret			-	pop psw; pop pc								|	PSW ALL FLAGS

	XCHGB	=	0x020,
	INTB	=	0x030,
	MOVB	=	0x040,
	ADDB	=	0x050,
	SUBB	=	0x060,
	MULB	=	0x070,
	DIVB	=	0x080,
	CMPB	=	0x090,
	NOTB	=	0x0A0,
	ANDB	=	0x0B0,
	ORB		=	0x0C0,
	XORB	=	0x0D0,
	TESTB	=	0x0E0,
	SHLB	=	0x0F0,
	SHRB	=	0x100,
	PUSHB	=	0x110,
	POPB	=	0x120,
	JMPB	=	0x130,
	JEQB	=	0x140,
	JNEB	=	0x150,
	JGTB	=	0x160,
	CALLB	=	0x170,

	XCHGW	=	0x0200,
	INTW	=	0x0300,
	MOVW	=	0x0400,
	ADDW	=	0x0500,
	SUBW	=	0x0600,
	MULW	=	0x0700,
	DIVW	=	0x0800,
	CMPW	=	0x0900,
	NOTW	=	0x0A00,
	ANDW	=	0x0B00,
	ORW		=	0x0C00,
	XORW	=	0x0D00,
	TESTW	=	0x0E00,
	SHLW	=	0x0F00,
	SHRW	=	0x1000,
	PUSHW	=	0x1100,
	POPW	=	0x1200,
	JMPW	=	0x1300,
	JEQW	=	0x1400,
	JNEW	=	0x1500,
	JGTW	=	0x1600,
	CALLW	=	0x1700,
} OpCode;

typedef enum AddrModes{
	IMMED 		= 	0x00,	// 1	-	000		// USE FIRST OR SECOND Im/Di/Ad	<=> 3rd / 3rd and 4th Byte used
	REGDIR 		= 	0x01,	// 1	-	001		// SKIP FIRST Im/Di/Ad <=> 3rd Byte = 0
	REGIND 		= 	0x02,	// 2	-	010		// SKIP FIRST Im/Di/Ad <=> 3rd Byte = 0
	REGIND8S 	= 	0x03,	// 3	-	011		// USE FIRST Im/Di/Ad <=> 3rd Byte used
	REGIND16S 	= 	0x04,	// 4	-	100		// USE FIRST AND SECOND Im/Di/Ad <=> 3rd and 4th Byte used
	MEMDIR 		= 	0x05,	// 5	-	101		// USE FIRST AND SECOND Im/Di/Ad <=> 3rd and 4th Byte used
	PCREL		=	0x40,						// 3rd and 4th Byte used
	ABS			=	0x50,						// 3rd and 4th Byte used

} AddrMode;

typedef struct{
	OpCode operation;
	char* op1;
	char* op2;
	char useHigherBits;
	char useLowerBits;
	char size;
} Instruction; // OBRISATI ALOKACIJU PROSTORA U KONTEKTU AKO SE CHAR* IZMENI U INT

OpCode decodeOpCode(char* word);
AddrMode decodeAddrMode(char* word);
Register decodeRegister(char* word);

#endif
