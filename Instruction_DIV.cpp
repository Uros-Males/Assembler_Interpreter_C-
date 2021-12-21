#include "Instruction_DIV.h"
#include "Exceptions.h"

#include<iostream>
#include<string>
#include<stdlib.h>

using namespace std;

void Instruction_DIV::executeInstruction(int *k) {
	if (this->operands_for_line[2]->value == 0) {
		return;
	}
	if(this->operands_for_line[2]->value != 0) this->operands_for_line[0]->value = this->operands_for_line[1]->value / this->operands_for_line[2]->value;
	(*k)++;
	return;
}