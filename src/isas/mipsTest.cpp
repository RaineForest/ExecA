
#include "instructionset.h"
#include "instruction.h"
#include "mips.h"
#include <cstdio>
#include <cstdint>
#include <string>
#include <sstream>

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

	vector<Instruction> out;
	
	try {
		mips.disassemble(data, size, &out, 0x400000);
	} catch (string err) {
		printf("Error: %s\n", err);
		delete[] data;
		return 1;
	}

	FILE* outfile = fopen(argv[2], "w");
	for(vector<Instruction>::iterator it = out.begin(); it != out.end(); ++it) {
		stringstream ss;
		ss << hex << it->getAddress() << ":\t" + it->toString() + "\n";
		fwrite(ss.str().c_str(), 1, ss.str().length(), outfile);
	}
	fclose(outfile);

	delete[] data;

	return 0;
}

