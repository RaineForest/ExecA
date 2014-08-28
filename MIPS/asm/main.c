#include <stdio.h>

#define INSTR_LEN 6
#define NUM_INSTR 42

typedef struct instruction {
	char name[INSTR_LEN];
	unsigned char type;
	unsigned char opcode;
	unsigned char funct;
} instruction_t;

int main(int argc, char* argv[]) {
/*
	if(argc < 3) {
		printf("Usage: %s input_file output_file\n", argv[0]);
		return 1;
	}
	char* inFile = argv[1];
	char* outFile = argv[2];
*/	
	FILE* lut = fopen("instruction.hex", "rb");
	instruction_t instructions[NUM_INSTR];
	for(int i = 0; i < NUM_INSTR; i++) {
		fgets(instructions[i].name, INSTR_LEN, lut);
		instructions[i].type = fgetc(lut);
		instructions[i].opcode = fgetc(lut);
		instructions[i].funct = fgetc(lut);
		printf("%s, %d, %d, %d\n", instructions[i].name, (int)instructions[i].type, (int)instructions[i].opcode, (int)instructions[i].funct);
	}
	fclose(lut);
/*	
	FILE* inHandle = fopen(inFile, "r");
	FILE* outHandle = fopen(outFile, "wb");
	
	
	
	fclose(inHandle);
	fclose(outHandle);
*/
	return 0;
}
