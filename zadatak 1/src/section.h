#ifndef _SECTION_H_
#define _SECTION_H_

typedef struct{
	int offset;
	char context;
} SectionContext;

typedef struct{
	SectionContext* data;
	struct SectionContextNode* next;
} SectionContextNode;

typedef struct{
	SectionContextNode* head;
	SectionContextNode* tail;
} SectionTable;

typedef struct{
	char* name;
	int offset;
	SectionTable* table;
} Section;

typedef struct{
	Section* data;
	struct SectionNode* next;
} SectionNode;

typedef struct{
	SectionNode* head;
	SectionNode* tail;
} SectionList;

Section* addSection(SectionNode** head, SectionNode** tail, char* name);
int removeSection(SectionNode** head, char* name);
Section* findSection(SectionNode** head, char* name);
void removeAllSections(SectionNode** head, SectionNode** tail);

/* SADRZAJ SEKCIJE */
void addContext(SectionContextNode** head, SectionContextNode** tail, int offset, char context);
void removeAllContext(SectionContextNode** head);


#endif
