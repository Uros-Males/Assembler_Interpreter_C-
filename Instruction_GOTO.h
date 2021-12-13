#ifndef INSTRUCTION_GOTO_H
#define INSTRUCTION_GOTO_H

#include "Instruction.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

class Instruction_GOTO : public Instruction {
public:
	void executeInstruction(int* k); 
	friend class Machine;
	friend class Operand;
	friend class Instruction;
};


#endif 
