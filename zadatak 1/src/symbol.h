#ifndef _SYMBOL_H_
#define _SYMBOL_H_

#include "section.h"

typedef struct{
	char* name;
	Section* section;
	int offset;
	char isLocal;
	int order;
} Symbol;

typedef struct{
	Symbol* data;
	struct SymbolNode* next;
} SymbolNode;

typedef struct{
	SymbolNode* head;
	SymbolNode* tail;
} SymbolTable;

Symbol* addSymbol(SymbolNode** head, SymbolNode** tail, char* name, Section* section, int offset, char isLocal, int order);
int removeSymbol(SymbolNode** head, char* name);
Symbol* findSymbol(SymbolNode** head, char* name);
void removeAllSymbols(SymbolNode** head, SymbolNode** tail);
Symbol* popSymbol(SymbolNode** head);

#endif
