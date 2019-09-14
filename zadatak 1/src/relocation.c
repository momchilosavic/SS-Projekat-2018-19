#include <stdio.h>
#include <stdlib.h>
#include "relocation.h"

void addRelocation(RelocationNode** head, RelocationNode** tail, Section* section, int offset, int type, int order){
	Relocation* newRelocation = (Relocation*)malloc(sizeof(Relocation));
	newRelocation->section = section;
	newRelocation->offset = offset;
	newRelocation->type = type;
	newRelocation->order = order;

	RelocationNode* newNode = (RelocationNode*)malloc(sizeof(RelocationNode));
	newNode->data = newRelocation;
	newNode->next = NULL;

	if(*head == NULL)
		*head = newNode;
	else
		(*tail)->next = newNode;
	*tail = newNode;
}

void removeAllRelocation(RelocationNode** head){
	RelocationNode* toRemove = NULL;
	while(*head != NULL){
		toRemove = *head;
		*head = (RelocationNode*)(*head)->next;
		free((RelocationNode*)toRemove->data);
		toRemove->data = NULL;
		free(toRemove);
	}
}
