
#ifndef _MIPSINSTRUCTION_H_
#define _MIPSINSTRUCTION_H_

#include "Instruction.h"
#include <string>
#include <cstdint>

#define MNEMONIC_LEN 5

using std::string;

enum MIPSIType {
    R,
    I,
    J
};

class MIPSInstruction : public Instruction {
private:
    char mnemonic[MNEMONIC_LEN]; // not null terminated
    MIPSIType type;
    uint8_t numArgs;
    uint8_t argMap[3];
    uint8_t opcode;
    uint8_t funct;

public:
    void toExecutable(uint32_t* bytes, uint32_t* args);

};

#endif
