
#ifndef _INSTRUCTIONBASE_H_
#define _INSTRUCTIONBASE_H_

#include "Instruction.h"
#include <string>

using std::string;

/**
 * class InstructionBase - a database for holding Instruction objects
 * @author Dakota Williams
 */
class InstructionBase {
	public:
		virtual int load(string filename) = 0;
		virtual Instruction get(string name) = 0;
};

#endif

