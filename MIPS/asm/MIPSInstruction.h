
#ifndef _MIPSINSTRUCTION_H_
#define _MIPSINSTRUCTION_H_

#include "Instruction.h"
#include <string>
#include <stdint.h>

#define MNEMONIC_LEN 5

using std::string;

enum MIPSIType {
    R,
    I,
    J
};

class MIPSInstruction : public Instruction {
private:
    char mnemonic[MNEMONIC_LEN+1]; // not null terminated
    MIPSIType type;
    uint8_t numArgs;
    uint8_t argMap[3];
    uint8_t opcode;
    uint8_t funct;

public:
    MIPSInstruction(char mem[MNEMONIC_LEN+1],
                    MIPSIType t,
                    uint8_t numArg,
                    uint8_t args[3],
                    uint8_t op,
                    uint8_t fun)
            : mnemonic(mem),
              type(t),
              numArgs(numArg),
              argMap(args),
              opcode(op),
              funct(fun) {}
    void toExecutable(uint32_t* bytes, uint32_t* args);

    char* getMnemonic();
    MIPSIType getType();
    uint8_t getNumArgs();
    uint8_t* getArgMap();
    uint8_t getOpcode();
    uint8_t getFunct();
};

#endif
