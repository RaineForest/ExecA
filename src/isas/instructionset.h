
#ifndef _INSTRUCTIONSET_H_
#define _INSTRUCTIONSET_H_

#include <vector>
#include <cstdint>
#include "instruction.h"

using namespace std;

class InstructionSet {
public:
	virtual void disassemble(uint8_t* data, int len, vector<Instruction>* out, uint64_t offset) = 0;
	virtual void assemble(vector<Instruction>* instr, uint8_t** data, int* len, uint64_t offset) = 0;
};

//subclass singletons go here

#endif

