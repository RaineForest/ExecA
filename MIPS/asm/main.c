
#define _GNU_SOURCE

#include <stdio.h>
#include "dlList.h"
#include <stdlib.h>
#include <string.h>

#define INSTR_LEN 6
#define REG_LEN 6
#define NUM_INSTR 42
#define NUM_REG 32
#define TOKENS " ,()\n"
#define MAX_LINE 80

typedef struct instruction {
	char name[INSTR_LEN];
	char type;
	unsigned char opcode;
	unsigned char funct;
} instruction_T;

typedef struct register_ {
	char name[INSTR_LEN];
	unsigned char reg;
} register_T;

struct label {
	char* labelname;
	unsigned short address;
};

//sequestial search, cuz not sorted
int findInstr(instruction_T arr[], int size, char* string, int len) {
	for(int i = 0; i < size; i++) {
		if(strncmp(string, arr[i].name, len) == 0) {
			return i;
		}
	}
	return -1;
}

DlList_T getParams(char* str) {
	DlList_T params;
	params = dll_create();
	char* part = strtok(str, TOKENS);
	while(part != NULL) {
		dll_append(params, part);
		part = strtok(NULL, TOKENS);
	}
	return params;
}

int main(int argc, char* argv[]) {
	if(argc < 3) {
		printf("Usage: %s input_file output_file\n", argv[0]);
		return 1;
	}
	char* inFile = argv[1];
	char* outFile = argv[2];

	FILE* lut = fopen("instruction.hex", "rb");
	instruction_T instructions[NUM_INSTR];
	for(int i = 0; i < NUM_INSTR; i++) {
		fgets(instructions[i].name, INSTR_LEN, lut);
		instructions[i].type = fgetc(lut);
		instructions[i].opcode = fgetc(lut);
		instructions[i].funct = fgetc(lut);
//		printf("%5s, %c, %d, %d\n", instructions[i].name, (int)instructions[i].type, (int)instructions[i].opcode, (int)instructions[i].funct);
	}
	fclose(lut);

	FILE* reg = fopen("register.hex", "rb");
	register_T registers[NUM_REG];
	for(int j = 0; j < NUM_REG; j++) {
		fgets(registers[j].name, REG_LEN, reg);
		registers[j].reg = fgetc(reg);
		fseek(reg, 2, SEEK_CUR);
//		printf("%5s, %d\n", registers[j].name, registers[j].reg);
	}
	fclose(reg);

	DlList_T labels = dll_create();
	FILE* inHandle = fopen(inFile, "r");
	FILE* outHandle = fopen(outFile, "wb");
	
	for(int i = 0; !feof(inHandle); i+=4) {
		unsigned int binaryinterp = 0;
		unsigned int linelen = MAX_LINE;
		char* line = (char*)malloc(MAX_LINE);
		int ret = getline(&line, &linelen, inHandle);
		if (ret == -1) {
			free(line);
			break;
		}
		printf("%s\n", line);
		
		DlList_T params = getParams(line);

		int size = dll_size(params);
		for(int j = 0; j < size; j++) {
			char* stuff = (char*)dll_get(params, j);
			printf("%s\n", stuff);
		}
/*
		int index = 0;
		if(index = findInstr(instructions, NUM_INSTR, dll_get(params, 0)) >= 0) {
			binaryinterp = instructions[index].opcode;
			binaryinterp << 26;
			switch(instructions[index].type) {
				case 'R':
					
					break;
				case 'I':
					break;
				case 'J':
					break;
				default:
			}
		}
*/
		dll_destroy(params);
		free(line);
	}
	
	fclose(inHandle);
	fclose(outHandle);
	dll_destroy(labels);
	return 0;
}
