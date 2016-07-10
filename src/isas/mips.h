
#ifndef _MIPS_H_
#define _MIPS_H_

#include "instructionset.h"
#include <map>
#include <sqlite3.h>

using namespace std;

class MIPS : public InstructionSet {
private:
	typedef union {
		uint8_t opcode : 6;
		struct {
			uint8_t rs : 5;
			uint8_t rt : 5;
			uint8_t rd : 5;
			uint8_t shamt : 5;
			uint8_t funct : 6;
		} R;
		struct {
			uint8_t rs : 5;
			uint8_t rt : 5;
			uint16_t immediate : 16;
		} I;
		struct {
			uint32_t address : 26;
		} J;
	} MIPS_Instruction;

public:
	void disassemble(uint8_t* data, int len, string* out);
	void assemble(string asminstr, uint8_t** data, int* len);
};

#endif

