
#ifndef _MIPSIBASE_H_
#define _MIPSIBASE_H_

#include "InstructionBase.h"
#include "MIPSInstruction.h"
#include <map>
#include <string>

using std::map;
using std::string;

/**
 * class MIPSIBase - a database for MIPS instructions
 * @author Dakota Williams
 */
class MIPSIBase : public InstructionBase {
private:
    typedef std::map<string, MIPSInstruction*> IMap;
    typedef std::pair<string, MIPSInstruction*> IPair;
    IMap base;

public:
    MIPSIBase();
    ~MIPSIBase();

    int load(string filename);
    Instruction* get(string name);
};

#endif

