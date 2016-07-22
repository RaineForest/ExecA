
#ifndef _INSTRUCTION_H_
#define _INSTRUCTION_H_

#include <string>
#include <vector>

using namespace std;

class Instruction {
private:
	uint64_t addr;
	vector<string> tokens;
	string text;

public:
	Instruction();
	~Instruction();

	void setAddress(uint64_t a);
	void addToken(string s);
	void setText(string t);

	int getNumTokens();
	string getToken(int i);

	uint64_t getAddress();

	string toString();
};

#endif

