#ifndef _RELOCATION_H_
#define _RELOCATION_H_

#include "section.h"

typedef enum{
	R8 = 0,
	R16,
	PC8,
	PC16
} RelocationType;

typedef struct{
	Section* section;
	int offset;
	int type;
	int order;
} Relocation;

typedef struct{
	Relocation* data;
	struct RelocationNode* next;
} RelocationNode;

typedef struct{
	RelocationNode* head;
	RelocationNode* tail;
} RelocationTable;

void addRelocation(RelocationNode** head, RelocationNode** tail, Section* section, int offset, int type, int order);
void removeAllRelocation(RelocationNode** head);

#endif
