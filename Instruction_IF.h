#ifndef INSTRUCTION_IF_H
#define INSTRUCTION_IF_H

#include "Instruction.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

class Instruction_IF : public Instruction {
public:
	Instruction_IF();
	void executeInstruction(int* k);
	friend class Machine;
	friend class Operand;
	friend class Instruction;
};

#endif
