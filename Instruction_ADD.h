#ifndef INSTRUCTION_ADD_H
#define INSTRUCTION_ADD_H

#include "Instruction.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

class Instruction_ADD : public Instruction {
public:
	void executeInstruction(int* k);
	friend class Machine;
	friend class Operand;
	friend class Instruction;
};

#endif
