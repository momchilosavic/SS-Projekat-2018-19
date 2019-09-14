#include <stdio.h>
#include "instruction.h"
#include "word.h"
#include "helper.h"

OpCode decodeOpCode(char* word){
	int i;
	trim(word, NULL);
	for(i = 0; word[i]; i++)
  		word[i] = tolower(word[i]);
	
	if(strcmp(word, "halt") == 0)
		return HALT;	
	if(strcmp(word, "xchg") == 0)
		return XCHG;	
	if(strcmp(word, "int") == 0)
		return INT;	
	if(strcmp(word, "mov") == 0)
		return MOV;	
	if(strcmp(word, "add") == 0)
		return ADD;	
	if(strcmp(word, "sub") == 0)
		return SUB;	
	if(strcmp(word, "mul") == 0)
		return MUL;	
	if(strcmp(word, "div") == 0)
		return DIV;	
	if(strcmp(word, "cmp") == 0)
		return CMP;	
	if(strcmp(word, "not") == 0)
		return NOT;	
	if(strcmp(word, "and") == 0)
		return AND;	
	if(strcmp(word, "or") == 0)
		return OR;	
	if(strcmp(word, "xor") == 0)
		return XOR;	
	if(strcmp(word, "test") == 0)
		return TEST;	
	if(strcmp(word, "shl") == 0)
		return SHL;		
	if(strcmp(word, "shr") == 0)
		return SHR;	
	if(strcmp(word, "push") == 0)
		return PUSH;	
	if(strcmp(word, "pop") == 0)
		return POP;	
	if(strcmp(word, "jmp") == 0)
		return JMP;	
	if(strcmp(word, "jeq") == 0)
		return JEQ;	
	if(strcmp(word, "jne") == 0)
		return JNE;	
	if(strcmp(word, "jgt") == 0)
		return JGT;	
	if(strcmp(word, "call") == 0)
		return CALL;	
	if(strcmp(word, "ret") == 0)
		return RET;	
	if(strcmp(word, "iret") == 0)
		return IRET;

	if(strcmp(word, "xchgb") == 0)
		return XCHGB;	
	if(strcmp(word, "intb") == 0)
		return INTB;	
	if(strcmp(word, "movb") == 0)
		return MOVB;	
	if(strcmp(word, "addb") == 0)
		return ADDB;	
	if(strcmp(word, "subb") == 0)
		return SUBB;	
	if(strcmp(word, "mulb") == 0)
		return MULB;	
	if(strcmp(word, "divb") == 0)
		return DIVB;	
	if(strcmp(word, "cmpb") == 0)
		return CMPB;	
	if(strcmp(word, "notb") == 0)
		return NOTB;	
	if(strcmp(word, "andb") == 0)
		return ANDB;	
	if(strcmp(word, "orb") == 0)
		return ORB;	
	if(strcmp(word, "xorb") == 0)
		return XORB;	
	if(strcmp(word, "testb") == 0)
		return TESTB;	
	if(strcmp(word, "shlb") == 0)
		return SHLB;		
	if(strcmp(word, "shrb") == 0)
		return SHRB;	
	if(strcmp(word, "pushb") == 0)
		return PUSHB;	
	if(strcmp(word, "popb") == 0)
		return POPB;	
	if(strcmp(word, "jmpb") == 0)
		return JMPB;	
	if(strcmp(word, "jeqb") == 0)
		return JEQB;	
	if(strcmp(word, "jneb") == 0)
		return JNEB;	
	if(strcmp(word, "jgtb") == 0)
		return JGTB;	
	if(strcmp(word, "callb") == 0)
		return CALLB;

	if(strcmp(word, "xchgw") == 0)
		return XCHGW;	
	if(strcmp(word, "intw") == 0)
		return INTW;	
	if(strcmp(word, "movw") == 0)
		return MOVW;	
	if(strcmp(word, "addw") == 0)
		return ADDW;	
	if(strcmp(word, "subw") == 0)
		return SUBW;	
	if(strcmp(word, "mulw") == 0)
		return MULW;	
	if(strcmp(word, "divw") == 0)
		return DIVW;	
	if(strcmp(word, "cmpw") == 0)
		return CMPW;	
	if(strcmp(word, "notw") == 0)
		return NOTW;	
	if(strcmp(word, "andw") == 0)
		return ANDW;	
	if(strcmp(word, "orw") == 0)
		return ORW;	
	if(strcmp(word, "xorw") == 0)
		return XORW;	
	if(strcmp(word, "testw") == 0)
		return TESTW;	
	if(strcmp(word, "shlw") == 0)
		return SHLW;		
	if(strcmp(word, "shrw") == 0)
		return SHRW;	
	if(strcmp(word, "pushw") == 0)
		return PUSHW;	
	if(strcmp(word, "popw") == 0)
		return POPW;	
	if(strcmp(word, "jmpw") == 0)
		return JMPW;	
	if(strcmp(word, "jeqw") == 0)
		return JEQW;	
	if(strcmp(word, "jnew") == 0)
		return JNEW;	
	if(strcmp(word, "jgtw") == 0)
		return JGTW;	
	if(strcmp(word, "callw") == 0)
		return CALLW;
	return -1;
}

AddrMode decodeAddrMode(char* word){
	if(match(word, "^[[:space:]]*&[[:alpha:]][_[:alnum:]]*[[:space:]]*$") == 1 || 
		match(word, DECIMAL_8_POSITIVE) == 1 || 
		match(word, DECIMAL_8_NEGATIVE) == 1 || 
		match(word, DECIMAL_16_POSITIVE) == 1 || 
		match(word, DECIMAL_16_NEGATIVE) == 1 || 
		match(word, HEXADECIMAL_8_POSITIVE) == 1 || 
		match(word, HEXADECIMAL_8_NEGATIVE) == 1 || 
		match(word, HEXADECIMAL_16_POSITIVE) == 1 || 
		match(word, HEXADECIMAL_16_NEGATIVE) == 1)
		return IMMED;
	if(match(word, "^[[:space:]]*r([0-9]|1[0-5])(h|l)?[[:space:]]*$") ||
		match(word, "^[[:space:]]*(sp|pc|psw)(h|l)?[[:space:]]*$"))
		return REGDIR;
	if(match(word, "^[[:space:]]*\\[r[0-9]\\][[:space:]]*$") || 
		match(word, "^[[:space:]]*\\[r1[0-5]\\][[:space:]]*$") ||
		match(word, "^[[:space:]]*\\[(sp|pc|psw)\\][[:space:]]*$"))
		return REGIND;
	if(match(word, "^[[:space:]]*((r([0-9]|1[0-5]))|sp|pc)\\[0*([0-9]|[1-9][0-9]|1[0-1][0-9]|12[0-7])\\][[:space:]]*$") || 
		match(word, "^[[:space:]]*((r([0-9]|1[0-5]))|sp|pc)\\[-0*([1-9]|[1-9][0-9]|1[0-1][0-9]|12[0-8])\\][[:space:]]*$") ||
		match(word, "^[[:space:]]*((r([0-9]|1[0-5]))|sp|pc)\\[(0x|0X)0*(([0-9]|[a-f]|[A-F])|([1-7]([0-9]|[a-f]|[A-F])))\\][[:space:]]*$") ||
		match(word, "^[[:space:]]*((r([0-9]|1[0-5]))|sp|pc)\\[(0x|0X)0*(([8-9]|[a-f]|[A-F])([0-9]|[a-f]|[A-F]))\\][[:space:]]*$"))
		return REGIND8S;
	if(match(word, "^[[:space:]]*((r([0-9]|1[0-5]))|sp|pc)\\[0*(12[8-9]|1[3-9][0-9]|[2-9][0-9]{2}|[1-9][0-9]{3}|[1-2][0-9]{4}|3[0-1][0-9]{3}|32[0-6][0-9]{2}|327[0-5][0-9]|3276[0-7])\\][[:space:]]*$") == 1 || 
		match(word, "^[[:space:]]*((r([0-9]|1[0-5]))|(sp)|(pc))\\[-0*(129|1[3-9][0-9]|[2-9][0-9]{2}|[1-9][0-9]{3}|[1-2][0-9]{4}|3[0-1][0-9]{3}|32[0-6][0-9]{2}|327[0-5][0-9]|3276[0-8])\\][[:space:]]*$") == 1 || 
		match(word, "^[[:space:]]*((r([0-9]|1[0-5]))|(sp)|(pc))\\[(0x|0X)0*((([1-9]|[a-f]|[A-F])([0-9]|[a-f]|[A-F]){2})|([1-7]([0-9]|[a-f]|[A-F]){3}))\\][[:space:]]*$") == 1 || 
		match(word, "^[[:space:]]*((r([0-9]|1[0-5]))|(sp)|(pc))\\[(0x|0X)0*([8-9]|[a-f]|[A-F])([0-9]|[a-f]|[A-F]){3}\\][[:space:]]*$") == 1 ||
		match(word, "^[[:space:]]*((r([0-9]|1[0-5]))|(sp)|(pc))\\[[_[:alpha:]][_[:alnum:]]*\\][[:space:]]*$") == 1)
		return REGIND16S;
	if(match(word, "^[[:space:]]*\\*0*([1-9]|[1-9][0-9]|1[0-1][0-9]|12[0-7])[[:space:]]*$") == 1 ||
		match(word, "^[[:space:]]*\\*-0*([1-9]|[1-9][0-9]|1[0-1][0-9]|12[0-8])[[:space:]]*$") == 1 ||
		match(word, "^[[:space:]]*\\*0*(12[8-9]|1[3-9][0-9]|[2-9][0-9]{2}|[1-9][0-9]{3}|[1-2][0-9]{4}|3[0-1][0-9]{3}|32[0-6][0-9]{2}|327[0-5][0-9]|3276[0-7])[[:space:]]*$") == 1 ||
		match(word, "^[[:space:]]*\\*-0*(129|1[3-9][0-9]|[2-9][0-9]{2}|[1-9][0-9]{3}|[1-2][0-9]{4}|3[0-1][0-9]{3}|32[0-6][0-9]{2}|327[0-5][0-9]|3276[0-8])[[:space:]]*$") == 1 ||
		match(word, "^[[:space:]]*\\*(0x|0X)0*(([1-9]|[a-f]|[A-F])|([1-7]([0-9]|[a-f]|[A-F])))[[:space:]]*$") == 1 ||
		match(word, "^[[:space:]]*\\*(0x|0X)0*([8-9]|[a-f]|[A-F])([0-9]|[a-f]|[A-F])[[:space:]]*$") == 1 ||
		match(word, "^[[:space:]]*\\*(0x|0X)0*((([1-9]|[a-f]|[A-F])([0-9]|[a-f]|[A-F]){2})|([1-7]([0-9]|[a-f]|[A-F]){3}))[[:space:]]*$") == 1 ||
		match(word, "^[[:space:]]*\\*(0x|0X)0*([8-9]|[a-f]|[A-F])([0-9]|[a-f]|[A-F]){3}[[:space:]]*$") == 1)
		return MEMDIR;
	if(match(word, "^[[:space:]]*\\$[[:alpha:]][_[:alnum:]]*[[:space:]]*$") == 1)
		return PCREL;
	if(match(word, "^[[:space:]]*[_[:alpha:]][_[:alnum:]]*[[:space:]]*$") == 1)
		return ABS;
	return -1;
}

Register decodeRegister(char* word){
	if(strcmp(word, "r0") == 0 || strcmp(word, "r0h") == 0 || strcmp(word, "r0l") == 0)
		return R0;
	if(strcmp(word, "r1") == 0 || strcmp(word, "r1h") == 0 || strcmp(word, "r1l") == 0)
		return R1;
	if(strcmp(word, "r2") == 0 || strcmp(word, "r2h") == 0 || strcmp(word, "r2l") == 0)
		return R2;
	if(strcmp(word, "r3") == 0 || strcmp(word, "r3h") == 0 || strcmp(word, "r3l") == 0)
		return R3;
	if(strcmp(word, "r4") == 0 || strcmp(word, "r4h") == 0 || strcmp(word, "r4l") == 0)
		return R4;
	if(strcmp(word, "r5") == 0 || strcmp(word, "r5h") == 0 || strcmp(word, "r5l") == 0)
		return R5;
	if(strcmp(word, "r6") == 0 || strcmp(word, "r6h") == 0 || strcmp(word, "r6l") == 0 || strcmp(word, "sp") == 0)
		return R6;
	if(strcmp(word, "r7") == 0 || strcmp(word, "r7h") == 0 || strcmp(word, "r7l") == 0 || strcmp(word, "pc") == 0)
		return R7;
}

