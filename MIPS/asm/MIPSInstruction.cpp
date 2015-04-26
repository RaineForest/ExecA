
#include "MIPSInstruction.h"

#define OPCODE_OFF  26
static int offsets[4] = {21, 16, 11, 6};

void MIPSInstruction::toExecutable(uint32_t* bytes, uint32_t* args) {
    *bytes = 0;
    *bytes |= opcode;
    *bytes <<= OPCODE_OFF;
    switch (type) {
        case R:
            for (int i = 0; i < numArgs; i++) {
                *bytes |= args[i];
                *bytes <<= offsets[argMap[i]];
            }
            *bytes |= funct;
            break;
        case I:
            for (int i = 0; i < numArgs; i++) {
                *bytes |= args[i];
                *bytes <<= offsets[argMap[i]];
            }
            break;
        case J:
            *bytes |= *args;
            break;
        default:
            // do nothing
            break;
    }
}
