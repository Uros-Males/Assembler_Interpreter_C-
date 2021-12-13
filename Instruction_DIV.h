#ifndef INSTRUCTION_DIV_H
#define INSTRUCTION_DIV_H

#include "Instruction.h"
#include<iostream>
#include<stdlib.h>

using namespace std;

class Instruction_DIV : public Instruction {
public:
	void executeInstruction(int *k);
	friend class Machine;
	friend class Operand;
	friend class Instruction;
};


#endif
