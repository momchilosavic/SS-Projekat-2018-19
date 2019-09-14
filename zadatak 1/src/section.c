#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "section.h"

Section* findSection(SectionNode** head, char* name){
	if(*head == NULL)
		return NULL;

	SectionNode* toReturn = *head;
	while(toReturn != NULL && strcmp(toReturn->data->name, name) != 0)
		toReturn = (SectionNode*)toReturn->next;

	if(toReturn == NULL)
		return NULL;
	return toReturn->data;
}

Section* addSection(SectionNode** head, SectionNode** tail, char* name){
	Section* newSection = NULL;
	newSection = findSection(head, name);
	if(newSection != NULL)
		return newSection;

	newSection = malloc(sizeof(Section));
	newSection->name = malloc(sizeof(char)*(strlen(name) + 1));
	strcpy(newSection->name, name);
	newSection->offset = 0;
	newSection->table = malloc(sizeof(SectionTable));
	newSection->table->head = NULL;
	newSection->table->tail = NULL;

	SectionNode* newNode = (SectionNode*)malloc(sizeof(SectionNode));
	newNode->data = newSection;
	newNode->next = 0;

	if(*head == NULL){
		*head = newNode;
		*tail = newNode;
	}
	else{
		(*tail)->next = (SectionNode*)newNode;
		*tail = (SectionNode*)((*tail)->next);
	}
	return newSection;
}

int removeSection(SectionNode** head, char* name){
	if(*head == NULL)
		return 0;

	SectionNode* toRemove = *head;
	SectionNode* previous = NULL;

	while(toRemove != NULL && strcmp(toRemove->data->name, name) != 0){
		previous = toRemove;
		toRemove = (SectionNode*)(toRemove->next);
	}

	if(toRemove == NULL)
		return 0;

	if(previous == NULL){
		head = toRemove->next;
		removeAllContext(&(toRemove->data->table->head));
		free(toRemove->data->table);
		toRemove->data->table = NULL;
		free(toRemove->data->name);
		toRemove->data->name = NULL;
		free(toRemove->data);
		toRemove->data = NULL;
		free(toRemove);
		toRemove = NULL;
	}
	else{
		previous->next = toRemove->next;
		removeAllContext(&(toRemove->data->table->head));
		free(toRemove->data->table);
		toRemove->data->table = NULL;
		free(toRemove->data->name);
		toRemove->data->name = NULL;
		free(toRemove->data);
		toRemove->data = NULL;
		free(toRemove);
		toRemove = NULL;
	}
	return 1;
}

void removeAllSections(SectionNode** head, SectionNode** tail){
	SectionNode* toRemove = NULL;
	while(*head != NULL){
		toRemove = *head;
		*head = (*head)->next;
		removeAllContext(&(toRemove->data->table->head));
		free(toRemove->data->table);
		toRemove->data->table = NULL;
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

/* SADRZAJ SEKCIJE */

void addContext(SectionContextNode** head, SectionContextNode** tail, int offset, char context){
	SectionContext* newContext = malloc(sizeof(SectionContext));
	newContext->context = context;
	newContext->offset = offset;
	
	SectionContextNode* newNode = malloc(sizeof(SectionContextNode));
	newNode->data = newContext;
	newNode->next = NULL;

	if(*head == NULL)
		*head = newNode;
	else
		(*tail)->next = newNode;
	*tail = newNode;
}

void removeAllContext(SectionContextNode** head){
	SectionContextNode* toRemove = NULL;
	while(*head != NULL){
		toRemove = *head;
		*head = (*head)->next;
		free(toRemove->data);
		toRemove->data = NULL;
		free(toRemove);
	}	
}
