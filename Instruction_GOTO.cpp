#include "Instruction_GOTO.h"

#include<iostream>
#include<string>
#include<stdlib.h>

using namespace std;

void Instruction_GOTO::executeInstruction(int* k) {
	(*k) += this->operands_for_line[0]->value;
	return;
}