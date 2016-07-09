
#ifndef _EXECUTABLE_H_
#define _EXECUTABLE_H_

#include "../util.h"
#include <string>

using namespace std;

class Executable {

public:
	virtual void read(string filename) = 0;

	virtual int getSectionSize(int s) = 0;
	virtual void getSection(int sectionNum, uint8_t* buffer, unsigned int len) const = 0;
	virtual int getNumSections() const = 0;

	virtual int getNumProgramHeaders() const = 0;

	virtual Endianness getEndianness() const = 0;

	virtual int getWordSize() const = 0;

	//virtual InstructionSet* getArch() = 0;
	
	virtual void printHeaderInfo() = 0;
	virtual void printProgramHeaderInfo(int p) = 0;
	virtual void printSectionHeaderInfo(int s) = 0;
};

Executable* makeExecutable(string filename);

#endif

