
#include "InstructionBase.h"
#include <map>
#include <string>

using std::map;
using std::string;


class MIPSIBase : public InstructionBase {
	private:
		typedef std::map<string, MIPSInstruction> IMap;
		IMap base;

	public:
		int load(string filename);
		MIPSInstruction get(string name);
};
