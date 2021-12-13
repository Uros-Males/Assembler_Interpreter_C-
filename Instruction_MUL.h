#ifndef INSTRUCTION_MUL_H
#define INSTRUCTION_MUL_H

#include "Instruction.h"
#include<iostream>
#include<stdlib.h>

using namespace std;

class Instruction_MUL : public Instruction {
public:
	void executeInstruction(int *k);
	friend class Machine;
	friend class Operand;
	friend class Instruction;
};


#endif
