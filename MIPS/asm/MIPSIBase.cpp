
#include "MIPSIBase.h"
#include <fstream>
#include <cstring>

#define INSTR_BYTES 12

using std::ifstream;

MIPSIBase::MIPSIBase() {
    base = IMap();
}

MIPSIBase::~MIPSIBase() {
    for(IMap::iterator i = IMap.begin(); i != IMap.end(); i++) {
        delete i->second;
    }
}

int MIPSIBase::load(string filename) {
    ifstream input(filename.c_str(), ifstream::binary);
    if(!input) {
        return -1;
    }

    char bytes[INSTR_BYTES+1];
    char mnem[MNEMONIC_LEN+1];
    mnem[MNEMONIC_LEN] = 0; //need the null
    MIPSIType type;
    uint8_t arglen;
    uint8_t argMap[3];
    while(!input.read(bytes, INSTR_BYTES).eof()) {
        memcpy(mnem, bytes, MNEMONIC_LEN);
        memcpy(argMap, &(bytes[7]), 3);
        switch(bytes[5]) {
            case 1:
                type = MIPSIType::R;
                arglen = 4;
                break;
            case 2:
                type = MIPSIType::I;
                arglen = 3;
                break;
            case 3:
                type = MIPSIType::J;
                arglen = 1;
                break;
            default:
                continue;
        }
        base.insert(IPair(string(mnem),
                          new MIPSInstruction(mnem,
                                              type,
                                              arglen,
                                              argMap,
                                              (uint8_t)bytes[10],
                                              (uint8_t)bytes[11])));
    }

    return 0;
}

Instruction* MIPSIBase::get(string name) {
    return base[name];
}

