#ifndef INSTRUCTION_ELSE_H
#define INSTRUCTION_ELSE_H

#include "Instruction.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

class Instruction_ELSE : public Instruction {
public:
	void executeInstruction(int* k);
	friend class Machine;
	friend class Operand;
	friend class Instruction;
};

#endif
