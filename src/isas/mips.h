
#ifndef _MIPS_H_
#define _MIPS_H_

#include "instructionset.h"
#include <map>
#include <sqlite3.h>

using namespace std;

class MIPS : public InstructionSet {
public:
	void disassemble(uint8_t* data, int len, string* out);
	void assemble(string asminstr, uint8_t** data, int* len);
};

#endif

