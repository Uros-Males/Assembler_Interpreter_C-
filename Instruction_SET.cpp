#include "Instruction_SET.h"

#include<iostream>
#include<string>
#include<stdlib.h>

using namespace std;

void Instruction_SET::executeInstruction(int *k) {
	//cout << this->operands_for_line.size();
	this->operands_for_line[0]->value = this->operands_for_line[1]->value;
	(*k)++;
	return;
}