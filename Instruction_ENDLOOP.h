#ifndef INSTRUCTION_ENDLOOP_H
#define INSTRUCTION_ENDLOOP_H

#include "Instruction.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

class Instruction_ENDLOOP : public Instruction {
public:
	void executeInstruction(int* k);
	friend class Machine;
	friend class Operand;
	friend class Instruction;
};

#endif
