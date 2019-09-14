#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol.h"

Symbol* findSymbol(SymbolNode** head, char* name){
	if(*head == NULL)
		return NULL;

	SymbolNode* toReturn = *head;
	while(toReturn != NULL && strcmp(toReturn->data->name, name) != 0)
		toReturn = (SymbolNode*)toReturn->next;

	if(toReturn == NULL)
		return NULL;
	return toReturn->data;
}

Symbol* addSymbol(SymbolNode** head, SymbolNode** tail, char* name, Section* section, int offset, char isLocal, int order){
	Symbol* newSymbol = NULL;
	newSymbol = findSymbol(head, name);
	if(newSymbol != NULL)
		return newSymbol;

	newSymbol = malloc(sizeof(Symbol));
	newSymbol->name = malloc(sizeof(char)*(strlen(name) + 1));
	strcpy(newSymbol->name, name);
	newSymbol->section = section;
	newSymbol->offset = offset;
	newSymbol->isLocal = isLocal;
	newSymbol->order = order;

	SymbolNode* newNode = (SymbolNode*)malloc(sizeof(SymbolNode));
	newNode->data = newSymbol;
	newNode->next = 0;

	if(*head == NULL){
		*head = newNode;
		*tail = newNode;
	}
	else{
		(*tail)->next = (SymbolNode*)newNode;
		*tail = (SymbolNode*)((*tail)->next);
	}
	return newSymbol;
}


int removeSymbol(SymbolNode** head, char* name){
	if(*head == NULL)
		return 0;

	SymbolNode* toRemove = *head;
	SymbolNode* previous = NULL;

	while(toRemove != NULL && strcmp(toRemove->data->name, name) != 0){
		previous = toRemove;
		toRemove = (SymbolNode*)(toRemove->next);
	}

	if(toRemove == NULL)
		return 0;

	if(previous == NULL){
		head = toRemove->next;
		free(toRemove->data->name);
		toRemove->data->name = NULL;
		free(toRemove->data);
		toRemove->data = NULL;
		free(toRemove);
		toRemove = NULL;
	}
	else{
		previous->next = toRemove->next;
		free(toRemove->data->name);
		toRemove->data->name = NULL;
		free(toRemove->data);
		toRemove->data = NULL;
		free(toRemove);
		toRemove = NULL;
	}
	return 1;
}

void removeAllSymbols(SymbolNode** head, SymbolNode** tail){
	SymbolNode* toRemove = NULL;
	while(*head != NULL){
		toRemove = *head;
		*head = (*head)->next;
		free(toRemove->data->name);
		toRemove->data->name = NULL;
		free(toRemove->data);
		toRemove->data = NULL;
		free(toRemove);
		toRemove = NULL;
	}
	*head = NULL;
	*tail = NULL;
}

Symbol* popSymbol(SymbolNode** head){
	if(*head == NULL)
		return NULL;
	SymbolNode* toRemove = *head;
	*head = (*head)->next;
	return toRemove->data;
}
