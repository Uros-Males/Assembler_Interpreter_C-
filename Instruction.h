#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include<vector>
#include<string>
#include "Machine.h"
#include "Operand.h"

using namespace std;

class Instruction {
public:
	virtual void executeInstruction(int *k) = 0; 
	friend class Machine;
	friend class Operand;
protected:
	vector<Operand*> operands_for_line; 
	char _type;
	int paired_else; 
	int paired_endif;
	int paired_endloop;
	int how_many;
};

#endif
