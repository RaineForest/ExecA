
#include "instruction.h"
#include <sstream>
#include <istream>
#include <iterator>

Instruction::Instruction() {
	tokens = vector<string>();
}

Instruction::~Instruction() {}

void Instruction::setAddress(uint64_t a) {
	addr = a;
}

void Instruction::setText(string t) {
	text = t;
}

void Instruction::addToken(string s) {
	tokens.push_back(s);
}

int Instruction::getNumTokens() {
	return tokens.size();
}

string Instruction::getToken(int i) {
	return tokens[i];
}

uint64_t Instruction::getAddress() {
	return addr;
}

string Instruction::toString() {
	return text;
};

