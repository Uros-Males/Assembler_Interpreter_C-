#include "Instruction_LOOP.h"

#include <iostream>
#include <stdlib.h>

using namespace std;

void Instruction_LOOP::executeInstruction(int* k) {
	(*k)++;
	return;
}