#include "Instruction_ADD.h"

#include<iostream>
#include<string>
#include<stdlib.h>

using namespace std;

void Instruction_ADD::executeInstruction(int *k) {
	this->operands_for_line[0]->value = this->operands_for_line[1]->value + this->operands_for_line[2]->value;
	(*k)++; 
	return;
}