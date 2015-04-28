
default: MIPSInstruction.o MIPSIBase.o

MIPSInstruction.o: MIPSInstruction.h MIPSInstruction.cpp Instruction.h
	g++ -c MIPSInstruction.cpp -o MIPSInstruction.o

MIPSIBase.o: MIPSIBase.cpp MIPSIBase.h MIPSInstruction.h Instruction.h
	g++ -c MIPSIBase.cpp -o MIPSIBase.o