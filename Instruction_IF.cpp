#include "Instruction_IF.h"

#include<iostream>
#include<string>
#include<stdlib.h>

using namespace std;

Instruction_IF::Instruction_IF() {
	this->_type = 'x';
	this->paired_else = -2;
}

void Instruction_IF::executeInstruction(int *k) {
	if (this->_type == 'g') {
		if (this->operands_for_line[0]->value > this->operands_for_line[1]->value) (*k)++;
		else (*k) = this->paired_else + 1;
	}
	if (this->_type == 'e') {
		if (this->operands_for_line[0]->value == this->operands_for_line[1]->value) (*k)++;
		else (*k) = this->paired_else + 1;
	}
	return;
}