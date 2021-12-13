#ifndef INSTRUCTION_LOOP_H
#define INSTRUCTION_LOOP_H

#include "Instruction.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

class Instruction_LOOP : public Instruction {
public:
	void executeInstruction(int* k);
	friend class Machine;
	friend class Operand;
	friend class Instruction;
};

#endif
