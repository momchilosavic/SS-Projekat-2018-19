#ifndef _CONTEXT_H_
#define _CONTEXT_H_

#include "section.h"
#include "symbol.h"
#include "instruction.h"
#include "relocation.h"

typedef struct{
	int lineCount;
	int wordCount;
	int totalWordCount;

	int order;

	SymbolTable* symbolTable;
	Symbol* currentSymbol;

	SectionList* sectionList;
	Section* currentSection;

	Instruction* instruction;

	// DRUGI PROLAZ
	RelocationTable* relocationTable;
} Context;

typedef enum{
	GLOBAL = 1,
	EXTERN,
	EQU,
	END,
	BYTE,
	WORD,
	ALIGN,
	SKIP,
	ASCII,
	ASCIIZ
} Directive;

void initializeContext(Context** context);
void deleteContext(Context** context);
int isInstruction(char* line, Context** context, int pass);
int isLabel(char* line, Context** context, int pass);
int isSection(char* line, Context** context, int pass);
int isDirective(char* line, Context** context, int pass);
Directive decodeDirective(char* line);

void updateSection(char* name, Context** context);
void updateOffset(Context** context);
int updateSymbolTable(Context** context);

// DRUGI PROLAZ
void resetSectionsCounter(Context** context);
void updateContext(Context** context);
#endif
