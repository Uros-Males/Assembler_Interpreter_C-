#ifndef OPERAND_H
#define OPERAND_H

#include<iostream>
#include<stdlib.h>
#include<string>

using namespace std;

class Operand {
public:
	Operand(); 
	Operand(int new_value, int idx);
	void changeValue(int new_value);
	void setId(int idx); 
	void getFromString(string s);
	friend class Machine;
	friend class Instruction;
	friend class Instruction_SET;
	friend class Instruction_ADD;
	friend class Instruction_SUB;
	friend class Instruction_MUL;
	friend class Instruction_DIV;
	friend class Instruction_GOTO;
	friend class Instruction_IF;
	friend class Instruction_ELSE;
	friend class Instruction_ENDIF;
	friend class Instruction_LOOP;
	friend class Instruction_ENDLOOP;
protected:
	int value;
	int id;
};


#endif 
