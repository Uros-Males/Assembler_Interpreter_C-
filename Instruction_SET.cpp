#include "Instruction_SET.h"

#include<iostream>
#include<string>
#include<stdlib.h>

using namespace std;

Instruction_SET::~Instruction_SET() {
	for (int i = 0; i < this->operands_for_line.size(); i++) delete this->operands_for_line[i];
}

void Instruction_SET::executeInstruction(int *k) {
	//cout << this->operands_for_line.size();
	this->operands_for_line[0]->value = this->operands_for_line[1]->value;
	(*k)++;
	return;
}