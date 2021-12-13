#include "Instruction_ELSE.h"

#include<iostream>
#include<string>
#include<stdlib.h>

using namespace std;

void Instruction_ELSE::executeInstruction(int* k) {
	(*k) = this->paired_endif; 
	return;
}