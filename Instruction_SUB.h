#ifndef INSTRUCTION_SUB_H
#define INSTRUCTION_SUB_H

#include "Instruction.h"
#include<iostream>
#include<stdlib.h>

using namespace std;

class Instruction_SUB : public Instruction {
public:
	void executeInstruction(int *k);
	friend class Machine;
	friend class Operand;
	friend class Instruction;
};


#endif
 