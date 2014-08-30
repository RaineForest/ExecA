
#include <stdio.h>
#include "dlList.h"
#include <stdlib.h>
#include <string.h>
//#include <byteswap.h> not supported in MinGW

#define INSTR_LEN 6
#define REG_LEN 6
#define NUM_INSTR 43
#define NUM_REG 32
#define MAX_ARGS 3
#define TOKENS " ,()\n"
#define MAX_LINE 80

//since not defined on all systems
unsigned int bswap_32(unsigned int thing) {
	int new = (thing << 24) + (thing >> 24);
	new += ((thing >> 16) << 24) >> 16;
	new += ((thing << 16) >> 24) << 16;
	return new;
}

typedef struct instruction {
	char name[INSTR_LEN];
	char type;
	unsigned char numArgs;
	unsigned char argMap[MAX_ARGS];
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

int findReg(register_T arr[], int size, char* string, int len) {
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
		instructions[i].numArgs = fgetc(lut);
		for(int j = 0; j < MAX_ARGS; j++) {
			instructions[i].argMap[j] = fgetc(lut);
		}
		instructions[i].opcode = fgetc(lut);
		instructions[i].funct = fgetc(lut);
//		printf("%5s, %c, 0x%x, 0x%x\n", instructions[i].name, (int)instructions[i].type, (int)instructions[i].opcode, (int)instructions[i].funct);
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
	
	DlList_T toWrite = dll_create();
	
	int error = 0;
	for(int i = 0; ; i++) {
		unsigned int binaryinterp = 0;
		unsigned int linelen = MAX_LINE;
		char line[MAX_LINE];
		char* err = fgets(line, linelen, inHandle);

		if(err == NULL) {
			break;
		}
		printf("%s\n", line);
		
		DlList_T params = getParams(line);
/*
		int size = dll_size(params);
		for(int j = 0; j < size; j++) {
			char* stuff = (char*)dll_get(params, j);
			printf("\"%s\"\n", stuff);
		}
*/
		int index = 0;
		char* mnemonic = (char*)dll_get(params, 0);
		if((index = findInstr(instructions, NUM_INSTR, mnemonic, strlen(mnemonic))) >= 0) {
//			printf("%5s, %c, %#x, %#x\n", instructions[index].name, (int)instructions[index].type, (int)instructions[index].opcode, (int)instructions[index].funct);

			binaryinterp = instructions[index].opcode;
			binaryinterp <<= 26;
			switch(instructions[index].type) {
				case 'R': {
					unsigned int bits2add = 0;
					for(int j = 0; j < instructions[index].numArgs; j++) {	
						char* regname = (char*)dll_get(params, j+1);
						int regindex = 0;
						regindex = findReg(registers, NUM_REG, regname, strlen(regname));
						if(regindex == -1) {
							fprintf(stderr, "Unknown parameter %d on line %d\n", j+1, i+1);
							error = 1;
							continue;
						}
						bits2add = (unsigned int)registers[regindex].reg;
						binaryinterp += bits2add << (26 - 5*instructions[index].argMap[j]);
					}
					//funct
					bits2add = instructions[index].funct;
					binaryinterp += bits2add;
					break; }
				case 'I':
					break;
				case 'J':
					break;
				default:
					break;
			}
		}
		
		printf("0x%08x\n", binaryinterp);
		//switch endianness
		binaryinterp = bswap_32(binaryinterp);
		dll_append(toWrite, (void*)binaryinterp);
		//fwrite((void*)&binaryinterp, sizeof(binaryinterp), 1, outHandle);

		dll_destroy(params);
	}
	
	fclose(inHandle);
	dll_destroy(labels);
	
	if(error == 0) {
		FILE* outHandle = fopen(outFile, "wb");
		int numInstr = dll_size(toWrite);
		dll_move_to(toWrite, 0);
		unsigned int thing = (unsigned int)dll_get(toWrite, 0);
		for(int i = 0; i < numInstr; i++) {
			fwrite((void*)&thing, 1, sizeof(thing), outHandle);
			thing = (unsigned int)dll_next(toWrite);
		}
		fclose(outHandle);
	}
	
	dll_destroy(toWrite);
	return 0;
}
