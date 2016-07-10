
#include "instructionset.h"
#include "mips.h"
#include <cstdio>
#include <cstdint>
#include <string>

using namespace std;

int main(int argc, char** argv) {
	if(argc != 3) {
		printf("Usage: mipsTest <infile> <outfile>\n");
		return 1;
	}

	FILE* infile = fopen(argv[1], "rb");
	fseek(infile, 0, SEEK_END);
	int size = ftell(infile);
	fseek(infile, 0, SEEK_SET);

	uint8_t* data = new uint8_t[size];

	fread(data, sizeof(uint8_t), size, infile);
	fclose(infile);

	MIPS mips;

	string out;

	mips.disassemble(data, size, &out);

	FILE* outfile = fopen(argv[2], "w");
	fwrite(out.c_str(), 1, out.length(), outfile);
	fclose(outfile);

	delete[] data;

	return 0;
}

