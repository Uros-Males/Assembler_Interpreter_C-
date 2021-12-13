#ifndef INSTRUCTION_SET_H
#define INSTRUCTION_SET_H

#include "Instruction.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

class Instruction_SET : public Instruction {
public:
	virtual void executeInstruction(int *k);
	friend class Machine;
	friend class Operand;
	friend class Instruction;
};
 

#endif
