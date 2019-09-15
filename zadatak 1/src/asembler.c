#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>
#include "context.h"
#include "word.h"



/*
* Check if input arguments are correct (-o command followed by 
* output file and many input files)
*
* return 1 if correct, 0 if not
*/
int checkInputArguments(int argc, char** argv){
	char* outputPattern = "^[_|a-z|0-9]+.o$";//"[_[:alphanum:]]+.o";
	char* inputPattern = "^[_|a-z|0-9]+.s$";//"[_[:alphanum:]]+.s";
	int i;
	int outputFlag = 0;
	int inputFlag = 0;

	if(argc != 4)
		return 0;

	for(i = 1; i < argc; i++){
		if(match(argv[i], outputPattern) == 1 || match(argv[i], inputPattern) == 1 || ((strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "-O") == 0) && outputFlag == 0)){
			if(strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "-O") == 0){
				outputFlag++;
				i++;
				if(!(i < argc && match(argv[i], outputPattern) == 1)){
					return 0;
				}
			}
		}
		else{
			return 0;
		}
	}
	if(outputFlag == 0)
		return 0;
	return 1;
}

/*
* Contains information on passed files
*/
typedef struct {
	int inputFilesCount;
	FILE** inputFiles;
	FILE* outputFile;	
} Files;

typedef struct {
	char* fileName;
} Errors;

/*
* Check if passed files exist and correct
*
* Opens all files and returns 1 if everything is ok, 0 if not
*/
int openFiles(int argc, char** argv, Files* f, Errors* e){
	int i;
	int outputOpened = 0;
	int inputOpened = 0;
	char* outputPattern = "^[_|a-z|0-9]+.o$";//"[_[:alphanum:]]+.o";
	char* inputPattern = "^[_|a-z|0-9]+.s$";//"[_[:alphanum:]]+.s";
	
	f->inputFiles = calloc(argc - 2, sizeof(FILE*));

	for(i = 1; i < argc; i++){
		if(match(argv[i], inputPattern) == 1){
			if ((f->inputFiles[inputOpened] = fopen(argv[i], "r")) == NULL) {
				int j;
				for(j = 0; j < inputOpened; j++)
					fclose(f->inputFiles[0]);
				if(outputOpened)
					fclose(f->outputFile);
				free(f->inputFiles);				
				e->fileName = argv[i];
				return 0;
			} 
			inputOpened++; 
		}
		else{
			if(match(argv[i], outputPattern) == 1){
				outputOpened++;
				if((f->outputFile = fopen(argv[i], "w")) == NULL){
					int j;
					for(j = 0; j < inputOpened; j++)
						fclose(f->inputFiles[j]);
					fclose(f->outputFile);
					free(f->inputFiles);
					e->fileName = argv[i];
					return 0;
				}
			}
		}
	}
	f->inputFilesCount = inputOpened;
}

/*
* Closes all opened files
*/
void closeFiles(Files* f){
	int i;
	for(i = 0; i < f->inputFilesCount; i++)
		fclose(f->inputFiles[i]);
	fclose(f->outputFile);
	free(f->inputFiles);
}


int firstPass(FILE* input, Context** context){
	char line[128];
	(*context)->lineCount = 0;
	int ret = 0;
	while(fgets(line, 128, input)){
		(*context)->lineCount++;
		(*context)->wordCount = 0;
		if(match(line, "^[[:space:]]*\\\n$") == 0){
			line[strlen(line) - 1] = 0;
			ret = isSection(line, context, 1);
			if(ret == 1)
				continue;
			ret = isDirective(line, context, 1);
			if(ret == 1)
				continue;
			if(ret == -1)
				break;

			ret = isLabel(line, context, 1);
			if(ret > 0)
				continue;
			if(ret == -1)
				return 0;
		
			ret = isInstruction(line, context, 1);
			if(ret == 1){
				updateOffset(context);
				continue;
			}
			if(ret == 0){
				printf("\tSYNTAX ERROR: \n\t\tLINE %d:\n\t\t\tNEAR WORD: %s\n", (*context)->lineCount, line);
				//deleteContext(context);
				return 0;
			}
		}
	}
	return 1;
}

int secondPass(FILE* input, Context** context){
	char line[128];
	(*context)->lineCount = 0;
	int ret = 0;
	resetSectionsCounter(context);

	while(fgets(line, 128, input)){
		(*context)->lineCount++;
		(*context)->wordCount = 0;
		if(match(line, "^[[:space:]]*\\\n$") == 0){
			line[strlen(line) - 1] = 0;
			ret = isSection(line, context, 2);
			if(ret == 1)
				continue;
			ret = isDirective(line, context, 2);
			if(ret == 1)
				continue;
			if(ret == -1)
				break;
			ret = isLabel(line, context, 2);
			if(ret == 2){
				updateContext(context);
				continue;
			}
			else
				if(ret == 1)
					continue;
			ret = isInstruction(line, context, 2);
			if(ret == 1)
				updateContext(context);
			else{
				printf("\nERROR: SYNTAX ERROR:\n\tLINE %d: NEAR WORD %s\n", (*context)->lineCount, line);
				return 0; 
			}
		}
	}
	return 1;
}

void writeFile(FILE* output, Context** context){
	fprintf(output, "\n#tabela simbola");
	fprintf(output, "\n\t#ime\t\t\t\t#sekcija\t\t#vrednost\t\t#vidljivost\t\t#redni broj\n");
	while((*context)->symbolTable->head){
		Symbol* s = popSymbol(&((*context)->symbolTable->head));
		fprintf(output, "\t%-16s\t%-16s%-16d%-16c%d\n", s->name, s->section == NULL ? s->offset != -1 ? "*ABS*" : "*UND*" : s->section->name, s->offset, s->isLocal==0? 'g':'l', s->order);
	}

	SectionNode* secNode = (*context)->sectionList->head;
	while(secNode != NULL){
		fprintf(output, "\n\n#ret%s", secNode->data->name);
		fprintf(output, "\n\t#offset\t\t#tip\t\t#redni broj[%s]\n", secNode->data->name);
		RelocationNode* relNode = (*context)->relocationTable->head;
		while(relNode != NULL){
			if(relNode->data->section == secNode->data)
				fprintf(output, "\t%04hhx\t\t%s\t\t%d\n", relNode->data->offset, relNode->data->type == R8 ? "R_386_8" : relNode->data->type == R16 ? "R_386_16" : "R_386_PC16", relNode->data->order);
			relNode = relNode->next;
		}
		secNode = secNode->next;
	}

	secNode = (*context)->sectionList->head;
	while(secNode != NULL){
		fprintf(output, "\n\n#%s\n", secNode->data->name);
		SectionContextNode* sec = secNode->data->table->head;
		while(sec != NULL){
			fprintf(output, "%02hhx ", (unsigned char)sec->data->context);
			sec = (SectionContextNode*)sec->next;
		}
		secNode = (SectionNode*)secNode->next;
	}
	
}


int main(int argc, char** argv){
	Files files;
	Errors errors;
	Context* context = NULL;
	int ret;

	initializeContext(&context);

	if(checkInputArguments(argc, argv) == 0){
		printf("%s error: fatal error: invalid input arguments\n", argv[0]);
		return -1;
	}
	if(openFiles(argc, argv, &files, &errors) == 0){
		printf("%s error: \n\tFile %s could not be opened\n", argv[0], errors.fileName);
		return -2;
	}

	ret = firstPass(files.inputFiles[0], &context);
	if(ret == 1){
		fseek(files.inputFiles[0], 0, SEEK_SET);
		ret = secondPass(files.inputFiles[0], &context);
		if(ret == 1){
			writeFile(files.outputFile, &context);
		}
	}
	deleteContext(&context);
	closeFiles(&files);
}
