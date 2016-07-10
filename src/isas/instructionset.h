
#ifndef _INSTRUCTIONSET_H_
#define _INSTRUCTIONSET_H_

#include <string>

using namespace std;

class InstructionSet {
public:
	virtual void disassemble(uint8_t* data, int len, string* out) = 0;
	virtual void assemble(string asminstr, uint8_t** data, int* len) = 0;
};

//subclass singletons go here

#endif

