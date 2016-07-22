
#ifndef _MIPS_H_
#define _MIPS_H_

#include "instruction.h"
#include "instructionset.h"
#include <map>
#include <sqlite3.h>

using namespace std;

class MIPS : public InstructionSet {
public:
	void disassemble(uint8_t* data, int len, vector<Instruction>* out, uint64_t offset);
	void assemble(vector<Instruction>* asminstr, uint8_t** data, int* len, uint64_t offset);
};

#endif

