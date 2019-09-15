#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "context.h"
#include "word.h"
#include "helper.h"


void initializeContext(Context** context){
	if(*context == NULL){
		*context = (Context*)malloc(sizeof(Context));

		(*context)->lineCount = 0;
		(*context)->wordCount = 0;
		(*context)->totalWordCount = 0;
		(*context)->order = 0;

		(*context)->symbolTable = malloc(sizeof(SymbolTable));
		(*context)->symbolTable->head = NULL;
		(*context)->symbolTable->tail = NULL;
		(*context)->currentSymbol = malloc(sizeof(Symbol));
		(*context)->currentSymbol->name = malloc(sizeof(char)*128);

		(*context)->sectionList = malloc(sizeof(SectionList));
		(*context)->sectionList->head = NULL;
		(*context)->sectionList->tail = NULL;
		(*context)->currentSection = NULL;

		(*context)->instruction = malloc(sizeof(Instruction));
		(*context)->instruction->op1 = malloc(sizeof(char)*128);
		(*context)->instruction->op2 = malloc(sizeof(char)*128);

		updateSection(".bss", context);
		strcpy((*context)->currentSymbol->name, ".bss");
		updateSymbolTable(context);
		updateSection(".data", context);
		strcpy((*context)->currentSymbol->name, ".data");
		updateSymbolTable(context);
		updateSection(".text", context);
		strcpy((*context)->currentSymbol->name, ".text");
		updateSymbolTable(context);

		// DRUGI PROLAZ
		(*context)->relocationTable = malloc(sizeof(RelocationTable));
		(*context)->relocationTable->head = NULL;
		(*context)->relocationTable->tail = NULL;
	}
}

void deleteContext(Context** context){	
	
	removeAllSymbols(&((*context)->symbolTable->head), &((*context)->symbolTable->tail));
	removeAllSections(&((*context)->sectionList->head), &((*context)->sectionList->tail));
	
	free((*context)->symbolTable);
	(*context)->symbolTable = NULL;
	free((*context)->sectionList);
	(*context)->sectionList = NULL;

	free((*context)->currentSymbol->name);
	(*context)->currentSymbol->name = NULL;
	free((*context)->currentSymbol);
	(*context)->currentSymbol = NULL;
	
	free((*context)->instruction->op1);
	(*context)->instruction->op1 = NULL;
	free((*context)->instruction->op2);
	(*context)->instruction->op2 = NULL;
	free((*context)->instruction);
	(*context)->instruction = NULL;

	// DRUGI PROLAZ
	removeAllRelocation(&((*context)->relocationTable->head));
	free((*context)->relocationTable);
	(*context)->relocationTable = NULL;
	//

	free(*context);
	*context = NULL;
}

int isInstruction(char* line, Context** context, int pass){
	OpCode opr;
	char size;
	AddrMode addr1;
	AddrMode addr2;
	char* op1 = malloc(sizeof(char)*128);
	char* op2 = malloc(sizeof(char)*128);

	char* word = (char*)strtok(line, " \t\n");
	word = trim(word, NULL);
	(*context)->wordCount++;
	(*context)->totalWordCount++;

	opr = decodeOpCode(word);
	// NEPOSTOJECA INSTRUKCIJA
	if(word == NULL || opr == -1){
		free(op1);
		free(op2);		
		return 0;
	}
	// INSTRUKCIJE BEZ OPERANADA
	if(opr == HALT || opr == RET || opr == IRET){
		word = strtok(NULL, " \t\n");
		if(word == NULL){
			free(op1);
			free(op2);
			(*context)->instruction->operation = opr;
			strcpy((*context)->instruction->op1, "\0");
			strcpy((*context)->instruction->op2, "\0");
			(*context)->instruction->size = 0;
			return 1;
		}
		// NEISPRAVNA SINTAKSA
		free(op1);
		free(op2);
		return 0;
	}	

	// INSTRUKCIJE SA JEDNIM OPERANDOM
	if(opr == INT || opr == NOT || opr == PUSH || opr == POP || opr == JMP || opr == JEQ || opr == JNE || opr == JGT || opr == CALL || opr == INTB || opr == NOTB || opr == PUSHB || opr == POPB || opr == JMPB || opr == JEQB || opr == JNEB || opr == JGTB || opr == CALLB || opr == INTW || opr == NOTW || opr == PUSHW || opr == POPW || opr == JMPW || opr == JEQW || opr == JNEW || opr == JGTW || opr == CALLW)	{			
		word = (char*)strtok(NULL, ",");
		// NEISPRAVNA SINTAKSA
		if(word == NULL || (addr1 = decodeAddrMode(word)) == -1 || (addr1 == IMMED && opr != PUSH && opr != PUSHB && opr != PUSHW)){
			free(op1);
			free(op2);
			return 0;
		}
		//
		if(pass == 2 && addr1 == ABS && (opr != PUSH && opr != PUSHB && opr != PUSHW)){
			Symbol* symb = findSymbol(&((*context)->symbolTable->head), word);
			if(symb == NULL || (symb->section == NULL && symb->isLocal == 0 && symb->offset == -1) || symb->section != NULL);
			else
				return 0;
		}
		//

		(*context)->wordCount++;
		(*context)->totalWordCount++;
		word = trim(word, NULL);
		strcpy(op1, word);

		word = (char*)strtok(NULL, " \t\n");
		if(word == NULL){
			if(opr == INTB || opr == NOTB || opr == PUSHB || opr == POPB || opr == JMPB || opr == JEQB || opr == JNEB || opr == JGTB || opr == CALLB)
				size = 0;
			else
				size = 1;

			(*context)->instruction->operation = opr;
			strcpy((*context)->instruction->op1, op1);
			strcpy((*context)->instruction->op2, "\0");
			(*context)->instruction->size = size;

			free(op1);
			free(op2);
			return 1;
			}
		// NEISPRAVNA SINTAKSA
		free(op1);
		free(op2);
		return 0;
	}

	// INSTRUKCIJE SA DVA OPERANDA
	if(opr == XCHG || opr == MOV || opr == ADD || opr == SUB || opr == MUL || opr == DIV || opr == CMP || opr == AND || opr == OR || opr == XOR || opr == TEST || opr == SHL || opr == SHR || opr == XCHGB || opr == MOVB || opr == ADDB || opr == SUBB || opr == MULB || opr == DIVB || opr == CMPB || opr == ANDB || opr == ORB || opr == XORB || opr == TESTB || opr == SHLB || opr == SHRB || opr == XCHGW || opr == MOVW || opr == ADDW || opr == SUBW || opr == MULW || opr == DIVW || opr == CMPW || opr == ANDW || opr == ORW || opr == XORW || opr == TESTW || opr == SHLW || opr == SHRW){
		word = (char*)strtok(NULL, ",");
		word = trim(word, NULL);
		(*context)->wordCount++;
		(*context)->totalWordCount++;
		// NEISPRAVNA SINTAKSA
		if(word == NULL || (addr1=decodeAddrMode(word)) == -1 || decodeAddrMode(word) == IMMED){
			free(op1);
			free(op2);
			return 0;
		}
		//
		if(pass == 2 && addr1 == ABS){
			Symbol* symb = findSymbol(&((*context)->symbolTable->head), word);
			if(symb == NULL || (symb->section == NULL && symb->isLocal == 0 && symb->offset == -1) || symb->section != NULL);
			else
				return 0;
		}
		//

		strcpy(op1, word);
		word = (char*)strtok(NULL, ",");
		word = trim(word, NULL);
		(*context)->wordCount++;
		(*context)->totalWordCount++;
		// NEISPRAVNA SINTAKSA
		if(word == NULL || decodeAddrMode(word) == -1){
			free(op1);
			free(op2);
			return 0;
		}

		strcpy(op2, word);
		word = (char*)strtok(NULL, " \t\n");
		if(word == NULL){
			if(opr == XCHGB || opr == MOVB || opr == ADDB || opr == SUBB || opr == MULB || opr == DIVB || opr == CMPB || opr == ANDB || opr == ORB || opr == XORB || opr == TESTB || opr == SHLB || opr == SHRB)
				size = 0;
			else
				size = 1;
			(*context)->instruction->operation = opr;
			strcpy((*context)->instruction->op1, op1);
			strcpy((*context)->instruction->op2, op2);
			(*context)->instruction->size = size;
			free(op1);
			free(op2);
			return 1;
		}
		// NEISPRAVNA SINTAKSA
		free(op1);
		free(op2);
		return 0;
	}
	// NEISPRAVNA SINTAKSA
	free(op1);
	free(op2);
	return 0;
}


int isLabel(char* line, Context** context, int pass){
	char* word;
	if(match(line, "^[[:space:]]*[_[:alpha:]][_[:alnum:]]*[[:space:]]*:[[:space:]]*") == 1){
		word = (char*)strtok(line, ":");
		// NIJE U PITANJU LABELA
		if(word == NULL)
			return 0;
		(*context)->wordCount++;
		(*context)->totalWordCount++;
		strcpy((*context)->currentSymbol->name, trim(word, "\t\n\v\f\r :"));
		word = (char*)strtok(NULL, ":");
		if(word == NULL || trim(word, NULL) == NULL | strlen(trim(word, NULL)) == 0){
			// VISESTRUKA DEFINICIJA SIMBOLA
			if(pass == 1 && updateSymbolTable(context) == 0)
				return -1;
			return 1;
		}
	
		// VISESTRUKA DEFINICIJA SIMBOLA
		if(pass == 1 && updateSymbolTable(context) == 0)
			return -1;
		
		// DIREKTIVA U NASTAVKU
		if(isDirective(word, context, pass))
			return 1;

		// SINTAKSNA GRESKA
		if(isInstruction(word, context, pass) == 0)
			return -1;

		if(pass == 1)
			updateOffset(context);

		return 2;
	}
	return 0;
}


int isSection(char* line, Context** context, int pass){
	char* word;
	if(match(line, "^[[:space:]]*(.text|.bss|.data)[[:space:]]*$") == 1){
		updateSection(trim(line, "\t\n\v\f\r "), context);
		strcpy((*context)->currentSymbol->name, trim(line, "\t\n\v\f\r "));
		(*context)->wordCount++;
		(*context)->totalWordCount++;
		if(pass == 1)
			updateSymbolTable(context);
		return 1;
	}
	if(match(line, "^[[:space:]]*.section[[:space:]]*[_.[:alpha:]][_[:alnum:]]*([[:space:]]*,[[:space:]]*\"(a|w|x|aw|ax|wx|wa|xa|xw|wa|awx|axw|wxa|xaw|xwa|xaw)\")?[[:space:]]*$") == 1){
		line = trim(line, " \t\n");
		word = (char*)strtok(line, " \t");
		word = (char*)strtok(NULL, " \t,");
		updateSection(trim(word, "\t\n\v\f\r ,"), context);
		(*context)->wordCount++;
		(*context)->totalWordCount++;
		strcpy((*context)->currentSymbol->name, trim(word, "\t\n\v\f\r "));
		if(pass == 1)
			updateSymbolTable(context);
		return 1;
	}
		return 0;
}

// PROVERA DIREKTIVA
Directive decodeDirective(char* line){
	if(match(line, "^[[:space:]]*.global[[:space:]]+[_[:alpha:]][_[:alnum:]]*[[:space:]]*$"))
		return GLOBAL;
	if(match(line, "^[[:space:]]*.extern[[:space:]]+[_[:alpha:]][_[:alnum:]]*[[:space:]]*$"))
		return EXTERN;
	if(match(line, "^[[:space:]]*.end[[:space:]]*$"))
		return END;
	if(match(line, "^[[:space:]]*.align[[:space:]]+0*[[:digit:]]+[[:space:]]*$"))
		return ALIGN;
	if(match(line, "^[[:space:]]*.skip[[:space:]]+((0*[[:digit:]]+)|(0+))[[:space:]]*$"))
		return SKIP;
	if(match(line, "^[[:space:]]*.byte[[:space:]]+((0*([0-9]|[1-9][0-9]|1[0-1][0-9]|12[0-7]))|(-0*([1-9]|[1-9][0-9]|1[0-1][0-9]|12[0-8])))([[:space:]]*,[[:space:]]*(0*([0-9]|[1-9][0-9]|1[0-1][0-9]|12[0-7]))|(-0*([1-9]|[1-9][0-9]|1[0-1][0-9]|12[0-8])))*[[:space:]]*$"))
		return BYTE;
	if(match(line, "^[[:space:]]*.word[[:space:]]+((0*([0-9]|[1-9][0-9]{1,3}|3[0-1][0-9]{3}|32[0-6][0-9]{2}|327[0-5][0-9]|3276[0-7]))|(-0*([1-9]|[1-9][0-9]{1,3}|3[0-1][0-9]{3}|32[0-6][0-9]{2}|327[0-5][0-9]|3276[0-8])))([[:space:]]*,[[:space:]]*((0*([0-9]|[1-9][0-9]{1,3}|3[0-1][0-9]{3}|32[0-6][0-9]{2}|327[0-5][0-9]|3276[0-7]))|(-0*([1-9]|[1-9][0-9]{1,3}|3[0-1][0-9]{3}|32[0-6][0-9]{2}|327[0-5][0-9]|3276[0-8]))))*[[:space:]]*$"))
		return WORD;
	if(match(line, "^[[:space:]]*.equ[[:space:]]+[_[:alpha:]][_[:alnum:]]*[[:space:]]*,[[:space:]]*((0*([0-9]|[1-9][0-9]{1,3}|3[0-1][0-9]{3}|32[0-6][0-9]{2}|327[0-5][0-9]|3276[0-7]))|(-0*([1-9]|[1-9][0-9]{1,3}|3[0-1][0-9]{3}|32[0-6][0-9]{2}|327[0-5][0-9]|3276[0-8])))[[:space:]]*$"))
		return EQU;
	if(match(line, "^[[:space:]]*.ascii[[:space:]]+\"[[:print:]]+\"[[:space:]]*$"))
		return ASCII;
	if(match(line, "^[[:space:]]*.asciiz[[:space:]]+\"[[:print:]]*\"[[:space:]]*$"))
		return ASCIIZ;

	return -1;
}

int isDirective(char* line, Context** context, int pass){
	Directive dir = decodeDirective(line);
	char* word;

	if(dir == -1)
		return 0;

	// TEKST NAKON .end DIREKTIVE SE NE TUMACI
	if(dir == END)
		return -1;	

	// EXTERN
	if(dir == EXTERN){
		// PASS 1
		if(pass == 1){
			word = strtok(line, " \t");
			word = strtok(NULL, " \t");
			word = trim(word, NULL);
			Symbol* sym = findSymbol(&((*context)->symbolTable->head), word);
			if(sym != NULL){
				printf("\nERROR: MULTIPLE DEFINITION OF SYMBOL \"%s\"", word);
				return -1;
			}
			else
				sym = addSymbol(&((*context)->symbolTable->head), &((*context)->symbolTable->tail), word, NULL, -1, 0, (*context)->order++);
		}
		return 1;
	}
	
	// GLOBAL
	if(dir == GLOBAL){	
		// PASS 2
		if(pass == 2){
			word = strtok(line, " \t");
			word = strtok(NULL, " \t");
			word = trim(word, NULL);
			Symbol* sym = findSymbol(&((*context)->symbolTable->head), word);
			if(sym == NULL){
				printf("\n\tERROR: SYMBOL \"%s\" NOT DEFINED", word);
				return -1;
			}
			sym->isLocal = 0;
		}
		return 1;
	}

	// EQU
	if(dir == EQU){
		// PASS 1
		if(pass == 1){
			line = trim(line, NULL);
			word = strtok(line, " \t");
			word = strtok(NULL, ",");
			word = trim(word, NULL);
			strcpy((*context)->currentSymbol->name, word);
			word = strtok(NULL, ",");
			word = trim(word, NULL);
			if(findSymbol(&((*context)->symbolTable->head), (*context)->currentSymbol->name) != NULL){
				printf("\n\tERROR: MULTIPLE DEFINITION OF SYMBOL %s \n\t\tLINE %d\n", (*context)->currentSymbol->name, (*context)->lineCount);
				return -1;
			}
			addSymbol(&((*context)->symbolTable->head), &((*context)->symbolTable->tail), (*context)->currentSymbol->name, NULL, (int)atoi(word), 1, (*context)->order++);
		}
		return 1;
	}

	// ALIGN
	if(dir == ALIGN){
		line = trim(line, NULL);
		word = strtok(line, " \t");
		word = strtok(NULL, " \t");
		int val = atoi(word);
		if((*context)->currentSection->offset % val > 0){
			// PASS 1
			if(pass == 1)
				(*context)->currentSection->offset += (val - (*context)->currentSection->offset % val);
			// PASS 2
			else{
				int j = 0;
				int cnt = (*context)->currentSection->offset % val;
				for(j = 0; j < (val - cnt); j++){
					addContext(&((*context)->currentSection->table->head), &((*context)->currentSection->table->tail), (*context)->currentSection->offset, 0);
					(*context)->currentSection->offset++;
				}
			}
		}
		return 1;
	}	

	if(dir == SKIP){
		line = trim(line, NULL);
		word = strtok(line, " \t");
		word = strtok(NULL, " \t");
		int val = atoi(word);
		// PASS 1
		if(pass == 1)
			(*context)->currentSection->offset += val;
		// PASS 2
		else{
			int j = 0;
				for(j = 0; j < val; j++){
					addContext(&((*context)->currentSection->table->head), &((*context)->currentSection->table->tail), (*context)->currentSection->offset, 0);
					(*context)->currentSection->offset++;
				}
		}
		return 1;
	}

	if(dir == BYTE || dir == WORD){
		line = trim(line, NULL);
		word = strtok(line, " \t");
		int val = 0;
		word = strtok(NULL, ",");
		// PASS 1
		if(pass == 1){
			while(word != NULL){
				val++;
				word = strtok(NULL, ",");
			}
			if(dir == WORD)
				val = val << 1;
			(*context)->currentSection->offset += val;
		}
		// PASS 2
		else{
			while(word != NULL){
				if(dir == BYTE){
					addContext(&((*context)->currentSection->table->head), &((*context)->currentSection->table->tail), (*context)->currentSection->offset, (char)atoi(word));
					(*context)->currentSection->offset++;				
				}
				else{
					addContext(&((*context)->currentSection->table->head), &((*context)->currentSection->table->tail), (*context)->currentSection->offset, (char)(atoi(word) >> 8));
					(*context)->currentSection->offset++;
					addContext(&((*context)->currentSection->table->head), &((*context)->currentSection->table->tail), (*context)->currentSection->offset, (char)(atoi(word)));
					(*context)->currentSection->offset++;
				}
				word = strtok(NULL, ",");
			}
		}
		return 1;
	}

	if(dir == ASCII || dir == ASCIIZ){
		line = trim(line, NULL);
		word = strtok(line, " \t");
		word = strtok(NULL, "\n");
		word = trim(word, NULL);
		// PASS 1
		if(pass == 1){
			if(dir == ASCII)
				(*context)->currentSection->offset += strlen(word) - 2;
			else
				(*context)->currentSection->offset += strlen(word) - 1;
		}
		// PASS 2
		else{
			int j;
			for(j = 1; j < strlen(word) - 1; j++){
				addContext(&((*context)->currentSection->table->head), &((*context)->currentSection->table->tail), (*context)->currentSection->offset, word[j]);
				(*context)->currentSection->offset++;
			}
			if(dir == ASCIIZ){
				addContext(&((*context)->currentSection->table->head), &((*context)->currentSection->table->tail), (*context)->currentSection->offset, 0);
				(*context)->currentSection->offset++;
			}
		}
		return 1;
	}

	return 0;
}

void updateOffset(Context** context){
	OpCode opr = (*context)->instruction->operation;
	char* op1 = (*context)->instruction->op1;
	char* op2 = (*context)->instruction->op2;
	AddrMode addr1 = decodeAddrMode(op1);
	AddrMode addr2 = decodeAddrMode(op2);
	char size = (*context)->instruction->size;
	
	// INSTRUKCIJE BEZ OPERANADA (1 BAJT)
	if(opr == HALT || opr == RET || opr == IRET){
		(*context)->currentSection->offset++;
		return;
	}

	// INSTRUKCIJE SA JEDNIM OPERANDOM (2 - 4 BAJTA)
	if(opr == INT || opr == NOT || opr == PUSH || opr == POP || opr == JMP || opr == JEQ || opr == JNE || opr == JGT || opr == CALL || opr == INTB || opr == NOTB || opr == PUSHB || opr == POPB || opr == JMPB || opr == JEQB || opr == JNEB || opr == JGTB || opr == CALLB || opr == INTW || opr == NOTW || opr == PUSHW || opr == POPW || opr == JMPW || opr == JEQW || opr == JNEW || opr == JGTW || opr == CALLW){
		// 2 BAJTA		
		if(addr1 == REGDIR || addr1 == REGIND){
			(*context)->currentSection->offset += 2;
			return;
		}
		// 3 BAJTA
		if(addr1 == REGIND8S || (addr1 == IMMED && size == 0)){
			(*context)->currentSection->offset += 3;
			return;
		}
		// 4 BAJTA
		(*context)->currentSection->offset += 4;
		return;
	}

	// INSTRUKCIJE SA DVA OPERANDA (3 - 7 BAJTOVA)
	if(opr == XCHG || opr == MOV || opr == ADD || opr == SUB || opr == MUL || opr == DIV || opr == CMP || opr == AND || opr == OR || opr == XOR || opr == TEST || opr == SHL || opr == SHR || opr == XCHGB || opr == MOVB || opr == ADDB || opr == SUBB || opr == MULB || opr == DIVB || opr == CMPB || opr == ANDB || opr == ORB || opr == XORB || opr == TESTB || opr == SHLB || opr == SHRB || opr == XCHGW || opr == MOVW || opr == ADDW || opr == SUBW || opr == MULW || opr == DIVW || opr == CMPW || opr == ANDW || opr == ORW || opr == XORW || opr == TESTW || opr == SHLW || opr == SHRW){
		// 2 BAJTA		
		if(addr1 == REGDIR || addr1 == REGIND)
			(*context)->currentSection->offset += 2;
		// 3 BAJTA
		else{
			if(addr1 == REGIND8S || (addr1 == IMMED && size == 0))
				(*context)->currentSection->offset += 3;
			// 4 BAJTA
			else
				(*context)->currentSection->offset += 4;
		}

		// + 1 BAJT		
		if(addr2 == REGDIR || addr2 == REGIND){
			(*context)->currentSection->offset++;
			return;
		}
		// + 2 BAJTA
		if(addr2 == REGIND8S || (addr2 == IMMED && size == 0)){
			(*context)->currentSection->offset += 2;
			return;
		}
		// + 3 BAJTA
		(*context)->currentSection->offset += 3;
		return;
	}
}

void updateSection(char* name, Context** context){
	if(*context == NULL)
		initializeContext(context);

	if(strlen(name) > 0)
		(*context)->currentSection = addSection(&((*context)->sectionList->head), &((*context)->sectionList->tail), name);
}

int updateSymbolTable(Context** context){
	char* name = (*context)->currentSymbol->name;
	Section* sec = (*context)->currentSection;
	if(match(name, "^\\.[_[:alpha:]][_[:alnum:]]*$") == 0 && findSymbol(&((*context)->symbolTable->head), name) != NULL){
		printf("\n\tERROR: MULTIPLE DEFINITION OF SYMBOL %s \n\t\tLINE %d\n", name, (*context)->lineCount);
		return 0;
	}

	if(match(name, "^\\.[_[:alpha:]][_[:alnum:]]*$") && findSymbol(&((*context)->symbolTable->head), name) != NULL)
		return 1;

	addSymbol(&((*context)->symbolTable->head), &((*context)->symbolTable->tail), name, sec, sec->offset, 1, (*context)->order++);
	
	return 1;
}

// DRUGI PROLAZ
void resetSectionsCounter(Context** context){
	SectionNode* temp = (*context)->sectionList->head;
	while(temp != NULL){
		temp->data->offset = 0;
		temp = temp->next;
	}
}


void updateContext(Context** context){
	OpCode opr = (*context)->instruction->operation;
	char* op1 = (*context)->instruction->op1;
	char* op2 = (*context)->instruction->op2;
	AddrMode addr1 = decodeAddrMode(op1);
	AddrMode addr2 = decodeAddrMode(op2);
	char size = (*context)->instruction->size;
	Section* section = (*context)->currentSection;
	int value = 0;
	int operandCount = 0;

	// INSTRUKCIJE BEZ OPERANADA (1 BAJT)
	if(opr == HALT || opr == RET || opr == IRET){
		value = opr << 3;
		addContext(&(section->table->head), &(section->table->tail), section->offset++, (char)value);
		return;
	}

	// PRVI BAJT
	if(opr >= XCHGB && opr <= CALLB)
		value = (opr >> 4) << 3;
	else{
		if(opr >= XCHGW)
			value = (opr >> 8) << 3;
		else
			value = opr << 3;
	}
	value = value | (size << 2);
	addContext(&(section->table->head), &(section->table->tail), section->offset++, (char)value);

	// INSTRUKCIJE SA JEDNIM OPERANDOM (2-4 BAJTA)
	if(opr == INT || opr == NOT || opr == PUSH || opr == POP || opr == JMP || opr == JEQ || opr == JNE || opr == JGT || opr == CALL || opr == INTB || opr == NOTB || opr == PUSHB || opr == POPB || opr == JMPB || opr == JEQB || opr == JNEB || opr == JGTB || opr == CALLB || opr == INTW || opr == NOTW || opr == PUSHW || opr == POPW || opr == JMPW || opr == JEQW || opr == JNEW || opr == JGTW || opr == CALLW)
		operandCount = 1;
	else
		operandCount = 2;

	// 2 BAJTA		
	if(addr1 == REGDIR || addr1 == REGIND){
		value = addr1 << 5;
		if(addr1 == REGIND)
			op1 = trim(op1, "[]");
		value = value | (decodeRegister(op1) << 1);
		if(addr1 == REGDIR && size == 0 && match(op1, "[[:digit:]]h"))
			value = value | 1;
		addContext(&(section->table->head), &(section->table->tail), section->offset++, (char)value);
		if(operandCount == 1)
			return;
	}

	// 3 BAJTA
	if(addr1 == REGIND8S){
		char* temp = malloc(sizeof(char)*128);
		value = addr1 << 5;
		strcpy(temp, op1);
		temp[3] = 0;
		temp = trim(temp, "[");

		value = value | (decodeRegister(temp) << 1);
		addContext(&(section->table->head), &(section->table->tail), section->offset++, (char)value);

		strcpy(temp, &op1[3]);
		temp = trim(temp, "[]");
		if(match(temp, DECIMAL_8_POSITIVE) || match(temp, DECIMAL_8_NEGATIVE)){
			value = (int)strtol(temp, NULL, 10);
		}
		else{
			value = (int)strtol(temp, NULL, 0);
		}
		addContext(&(section->table->head), &(section->table->tail), section->offset++, (char)value);

		free(temp);
		if(operandCount == 1)
			return;
	}

	// 4 BAJTA
	if(addr1 == REGIND16S){
		char* temp = malloc(sizeof(char)*128);
		value = addr1 << 5;

		strcpy(temp, op1);
		temp[3] = 0;
		temp = trim(temp, "[");

		value = value | (decodeRegister(temp) << 1);
		addContext(&(section->table->head), &(section->table->tail), section->offset++, (char)value);

		strcpy(temp, &op1[3]);
		temp = trim(temp, "[]");
		if(match(temp, DECIMAL_16_POSITIVE) || match(temp, DECIMAL_16_NEGATIVE))
			value = (int)strtol(temp, NULL, 10);
		else{
			if(match(temp, HEXADECIMAL_16_POSITIVE) || match(temp, HEXADECIMAL_16_NEGATIVE))
				value = (int)strtol(temp, NULL, 0);
			else{
				Symbol* sym = findSymbol(&((*context)->symbolTable->head), temp);
				if(sym == NULL){
					sym = addSymbol(&((*context)->symbolTable->head), &((*context)->symbolTable->tail), temp, NULL, -1, -1, (*context)->order++);
					addRelocation(&((*context)->relocationTable->head), &((*context)->relocationTable->tail), section, section->offset, R16, sym->order);
					value = 0;
				}
				else{
					if(sym->section == NULL && sym->isLocal == 1)
						value = sym->offset;
					else{
						addRelocation(&((*context)->relocationTable->head), &((*context)->relocationTable->tail), section, section->offset, sym->section == NULL?R16:PC16, sym->order);
						value = 0;
					}
				}
			}
		}
		addContext(&(section->table->head), &(section->table->tail), section->offset++, (char)value);
		addContext(&(section->table->head), &(section->table->tail), section->offset++, (char)(value >> 8));

		free(temp);
		if(operandCount == 1)
			return;
	}

	// 4 BAJTA
	if(addr1 == MEMDIR){
		value = addr1 << 5;
		addContext(&(section->table->head), &(section->table->tail), section->offset++, (char)value);

		op1 = trim(op1, "*");
		if(match(op1, DECIMAL_8_POSITIVE) || match(op1, DECIMAL_8_NEGATIVE) || match(op1, DECIMAL_16_POSITIVE) || match(op1, DECIMAL_16_NEGATIVE))
			value = (int)strtol(op1, NULL, 10);
		else
			value = (int)strtol(op1, NULL, 0);

		addContext(&(section->table->head), &(section->table->tail), section->offset++, (char)value);
		addContext(&(section->table->head), &(section->table->tail), section->offset++, (char)(value >> 8));
		if(operandCount == 1)
			return;
	}

	// 3 - 4 BAJTA
	if(addr1 == IMMED){
		value = addr1 << 5;
		addContext(&(section->table->head), &(section->table->tail), section->offset++, (char)value);

		if(match(op1, "&")){
			op1 = trim(op1, "&");
			Symbol* sym = findSymbol(&((*context)->symbolTable->head), op1);
			if(sym == NULL)
				sym = addSymbol(&((*context)->symbolTable->head), &((*context)->symbolTable->tail), op1, NULL, -1, -1, (*context)->order++);
				
			addRelocation(&((*context)->relocationTable->head), &((*context)->relocationTable->tail), section, section->offset, size?R16:R8, sym->order);
			value = 0;
		}
		else{
			if(match(op1, DECIMAL_8_POSITIVE) || match(op1, DECIMAL_8_NEGATIVE) || match(op1, DECIMAL_16_POSITIVE) || match(op1, DECIMAL_16_NEGATIVE))
				value = (int)strtol(op1, NULL, 10);
			else
				value = (int)strtol(op1, NULL, 0);
		}

		addContext(&(section->table->head), &(section->table->tail), section->offset++, (char)value);
		if(size == 1 || match(op1, "&"))
			addContext(&(section->table->head), &(section->table->tail), section->offset++, (char)(value >> 8));

		if(operandCount == 1)
			return;
	}
	
	// 4 BAJTA
	if(addr1 == ABS){
		Symbol* sym = findSymbol(&((*context)->symbolTable->head), op1);

		// LOKALNI SIMBOL KOJI NIJE APSOLUTAN ILI SIMBOL KOJI NIJE DEFINISAN (MEMDIR)
		if(sym == NULL || (sym->section == NULL && sym->isLocal == 0 && sym->offset == -1) || sym->section != NULL){
			value = MEMDIR << 5;
			addContext(&(section->table->head), &(section->table->tail), section->offset++, (char)value);
			if(sym == NULL)
				sym = addSymbol(&((*context)->symbolTable->head), &((*context)->symbolTable->tail), trim(op1, NULL), NULL, -1, -1, (*context)->order++);
			value = 0;
			addRelocation(&((*context)->relocationTable->head), &((*context)->relocationTable->tail), section, section->offset, sym->section == NULL?R16:PC16, sym->order);
			
			addContext(&(section->table->head), &(section->table->tail), section->offset++, (char)value);
			addContext(&(section->table->head), &(section->table->tail), section->offset++, (char)value);
			if(operandCount == 1)
				return;
		}
		// LOKALNI APSOLUTNI SIMBOL	-- NEISPRAVNO ZA PRVI OPERAND
		else{
			value = IMMED << 5;
			addContext(&(section->table->head), &(section->table->tail), section->offset++, (char)value);
			value = sym->offset;
			addContext(&(section->table->head), &(section->table->tail), section->offset++, (char)value);
			if(size == 1)
				addContext(&(section->table->head), &(section->table->tail), section->offset++, (char)(value >> 8));
			if(operandCount == 1)
				return;
		}
	}

	// 4 BAJTA	-- GRESKA!! KREIRA SIMBOL   U SVAKOM SLUCAJU
	if(addr1 == PCREL){
		value = REGIND16S << 5;
		addContext(&(section->table->head), &(section->table->tail), section->offset++, (char)value);

		op1 = trim(op1, "$");
		Symbol* sym = findSymbol(&((*context)->symbolTable->head), op1);
		if(sym == NULL)
			sym = addSymbol(&((*context)->symbolTable->head), &((*context)->symbolTable->tail), op1, NULL, -1, -1, (*context)->order++);
		value = 0;
		if(sym->section == NULL && sym->isLocal == 1)
			value = sym->offset;
		else
			addRelocation(&((*context)->relocationTable->head), &((*context)->relocationTable->tail), section, section->offset, PC16, sym->order);
		addContext(&(section->table->head), &(section->table->tail), section->offset++, (char)value);
		addContext(&(section->table->head), &(section->table->tail), section->offset++, (char)(value >> 8));
		if(operandCount == 1)
			return;
	}
	


	// INSTRUKCIJE SA 2 OPERANDA
	// 2 BAJTA		
	if(addr2 == REGDIR || addr2 == REGIND){
		value = addr2 << 5;
		if(addr2 == REGIND)
			op2 = trim(op2, "[]");
		value = value | (decodeRegister(op2) << 1);
		if(addr2 == REGDIR && size == 0 && match(op2, "[[:digit:]]h"))
			value = value | 1;
		addContext(&(section->table->head), &(section->table->tail), section->offset++, (char)value);
		return;
	}

	// 3 BAJTA
	if(addr2 == REGIND8S){
		char* temp = malloc(sizeof(char)*128);
		value = addr2 << 5;
		strcpy(temp, op2);
		temp[3] = 0;
		temp = trim(temp, "[");

		value = value | (decodeRegister(temp) << 1);
		addContext(&(section->table->head), &(section->table->tail), section->offset++, (char)value);

		strcpy(temp, &op2[3]);
		temp = trim(temp, "[]");
		if(match(temp, DECIMAL_8_POSITIVE) || match(temp, DECIMAL_8_NEGATIVE))
			value = (int)strtol(temp, NULL, 10);
		else
			value = (int)strtol(temp, NULL, 0);
		addContext(&(section->table->head), &(section->table->tail), section->offset++, (char)value);

		free(temp);
		return;
	}

	// 4 BAJTA
	if(addr2 == REGIND16S){
		char* temp = malloc(sizeof(char)*128);
		value = addr2 << 5;

		strcpy(temp, op2);
		temp[3] = 0;
		temp = trim(temp, "[");

		value = value | (decodeRegister(temp) << 1);
		addContext(&(section->table->head), &(section->table->tail), section->offset++, (char)value);

		strcpy(temp, &op2[3]);
		temp = trim(temp, "[]");
		if(match(temp, DECIMAL_16_POSITIVE) || match(temp, DECIMAL_16_NEGATIVE))
			value = (int)strtol(temp, NULL, 10);
		else{
			if(match(temp, HEXADECIMAL_16_POSITIVE) || match(temp, HEXADECIMAL_16_NEGATIVE))
				value = (int)strtol(temp, NULL, 0);
			else{
				Symbol* sym = findSymbol(&((*context)->symbolTable->head), temp);
				if(sym == NULL){
					sym = addSymbol(&((*context)->symbolTable->head), &((*context)->symbolTable->tail), temp, NULL, -1, -1, (*context)->order++);
					addRelocation(&((*context)->relocationTable->head), &((*context)->relocationTable->tail), section, section->offset, R16, sym->order);
					value = 0;
				}
				else{
					if(sym->section == NULL && sym->isLocal == 1)
						value = sym->offset;
					else{
						addRelocation(&((*context)->relocationTable->head), &((*context)->relocationTable->tail), section, section->offset, sym->section == NULL?R16:PC16, sym->order);
						value = 0;
					}
				}
			}
		}
		addContext(&(section->table->head), &(section->table->tail), section->offset++, (char)value);
		addContext(&(section->table->head), &(section->table->tail), section->offset++, (char)(value >> 8));

		free(temp);
		return;
	}

	// 4 BAJTA
	if(addr2 == MEMDIR){
		value = addr2 << 5;
		addContext(&(section->table->head), &(section->table->tail), section->offset++, (char)value);

		op2 = trim(op2, "*");
		if(match(op2, DECIMAL_8_POSITIVE) || match(op2, DECIMAL_8_NEGATIVE) || match(op2, DECIMAL_16_POSITIVE) || match(op2, DECIMAL_16_NEGATIVE))
			value = (int)strtol(op2, NULL, 10);
		else
			value = (int)strtol(op2, NULL, 0);

		addContext(&(section->table->head), &(section->table->tail), section->offset++, (char)value);
		addContext(&(section->table->head), &(section->table->tail), section->offset++, (char)(value >> 8));
		return;
	}

	// 3 - 4 BAJTA
	if(addr2 == IMMED){
		value = addr2 << 5;
		addContext(&(section->table->head), &(section->table->tail), section->offset++, (char)value);

		if(match(op2, "&")){
			op2 = trim(op2, "&");
			Symbol* sym = findSymbol(&((*context)->symbolTable->head), op2);
			if(sym == NULL)
				sym = addSymbol(&((*context)->symbolTable->head), &((*context)->symbolTable->tail), op2, NULL, -1, -1, (*context)->order++);
				
			addRelocation(&((*context)->relocationTable->head), &((*context)->relocationTable->tail), section, section->offset, size?R16:R8, sym->order);
			value = 0;
		}
		else{
			if(match(op2, DECIMAL_8_POSITIVE) || match(op2, DECIMAL_8_NEGATIVE) || match(op2, DECIMAL_16_POSITIVE) || match(op2, DECIMAL_16_NEGATIVE))
				value = (int)strtol(op2, NULL, 10);
			else
				value = (int)strtol(op2, NULL, 0);
		}

		addContext(&(section->table->head), &(section->table->tail), section->offset++, (char)value);
		if(size == 1 || match(op2, "&"))
			addContext(&(section->table->head), &(section->table->tail), section->offset++, (char)(value >> 8));

		return;
	}
	
	// 4 BAJTA
	if(addr2 == ABS){
		Symbol* sym = findSymbol(&((*context)->symbolTable->head), op2);

		// LOKALNI SIMBOL KOJI NIJE APSOLUTAN ILI SIMBOL KOJI NIJE DEFINISAN (MEMDIR)
		if(sym == NULL || (sym->section == NULL && sym->isLocal == 0 && sym->offset == -1) || sym->section != NULL){
			value = MEMDIR << 5;
			addContext(&(section->table->head), &(section->table->tail), section->offset++, (char)value);
			if(sym == NULL)
				sym = addSymbol(&((*context)->symbolTable->head), &((*context)->symbolTable->tail), op2, NULL, -1, -1, (*context)->order++);

			value = 0;
			addRelocation(&((*context)->relocationTable->head), &((*context)->relocationTable->tail), section, section->offset, sym->section==NULL?R16:PC16, sym->order);
			
			addContext(&(section->table->head), &(section->table->tail), section->offset++, (char)value);
			addContext(&(section->table->head), &(section->table->tail), section->offset++, (char)value);
			return;
		}
		// LOKALNI APSOLUTNI SIMBOL
		else{
			value = IMMED << 5;
			addContext(&(section->table->head), &(section->table->tail), section->offset++, (char)value);
			value = sym->offset;
			addContext(&(section->table->head), &(section->table->tail), section->offset++, (char)value);
			if(size == 1)
				addContext(&(section->table->head), &(section->table->tail), section->offset++, (char)(value >> 8));
			return;
		}
	}

	// 4 BAJTA		- IZMENITI I ZA PRVU ADRESU
	if(addr2 == PCREL){
		value = REGIND16S << 5;
		addContext(&(section->table->head), &(section->table->tail), section->offset++, (char)value);

		op2 = trim(op2, "$");
		Symbol* sym = findSymbol(&((*context)->symbolTable->head), op2);
		if(sym == NULL)
			sym = addSymbol(&((*context)->symbolTable->head), &((*context)->symbolTable->tail), op2, NULL, -1, -1, (*context)->order++);
		value = 0;
		if(sym->section == NULL && sym->isLocal == 1)
			value = sym->offset;
		else
			addRelocation(&((*context)->relocationTable->head), &((*context)->relocationTable->tail), section, section->offset, PC16, sym->order);
		
		addContext(&(section->table->head), &(section->table->tail), section->offset++, (char)value);
		addContext(&(section->table->head), &(section->table->tail), section->offset++, (char)(value>>8));
		return;
	}
}



