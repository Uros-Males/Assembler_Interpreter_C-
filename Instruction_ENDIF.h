#ifndef INSTRUCTION_ENDIF_H
#define INSTRUCTION_ENDIF_H

#include "Instruction.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

class Instruction_ENDIF : public Instruction {
public:
	void executeInstruction(int* k);
	friend class Machine;
	friend class Operand;
	friend class Instruction;
protected:
	
};

#endif
