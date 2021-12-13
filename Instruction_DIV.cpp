#include "Instruction_DIV.h"

#include<iostream>
#include<string>
#include<stdlib.h>

using namespace std;

void Instruction_DIV::executeInstruction(int *k) {
	//cout << this->operands_for_line[1]->value << " " << this->operands_for_line[2]->value << endl;
	this->operands_for_line[0]->value = this->operands_for_line[1]->value / this->operands_for_line[2]->value;
	(*k)++;
	return;
}