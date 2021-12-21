#include "Instruction_ENDLOOP.h"

#include <iostream>
#include <stdlib.h>

using namespace std;

void Instruction_ENDLOOP::executeInstruction(int *k) {
	if (this->how_many == -2) (*k) = this->paired_endloop;
	else {
		how_many--;
		if (this->how_many == 0) {
			(*k)++; 
			this->how_many = this->original;
		}
		else (*k) = this->paired_endloop;
	}
	return;
}