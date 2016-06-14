
#ifndef _EXECUTABLE_H_
#define _EXECUTABLE_H_

#include <string>

using namespace std;

class Executable {

public:
	virtual void read(string filename) = 0;

	//virtual InstructionSet* getArch() = 0;
};

#endif

