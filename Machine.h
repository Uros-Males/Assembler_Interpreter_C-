#ifndef MACHINE_H
#define MACHINE_H

#include <string>
#include <vector>
#include "Instruction.h"
#include "Operand.h"

using namespace std;

class Machine {
public:
	Machine();
	void loadProgram(const string& filepath);
	void execute(const string& filepath);
	Operand* findOperand(char c); 
	void processLine(Instruction* tmp, string tmps, int &i, vector<int> &labels);
	bool checkOperand(string s);
	void clear();
private:
	int failed;
	int program_size;
	int program_counter; 
	vector<Instruction*> list_of_instructions;
	vector<Operand*> operands; 
	vector<Operand*> to_delete;
};


#endif

