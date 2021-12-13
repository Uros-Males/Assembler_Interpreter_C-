#include "Instruction_ENDIF.h"

#include<iostream>
#include<string>
#include<stdlib.h>

using namespace std;

void Instruction_ENDIF::executeInstruction(int* k) {
	(*k)++;
	return;
}