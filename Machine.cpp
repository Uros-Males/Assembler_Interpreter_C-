#include "Machine.h"
#include "Instruction.h"
#include "Instruction_SET.h"
#include "Instruction_ADD.h"
#include "Instruction_MUL.h"
#include "Instruction_SUB.h"
#include "Instruction_DIV.h"
#include "Instruction_GOTO.h"
#include "Instruction_IF.h"
#include "Instruction_ELSE.h"
#include "Instruction_ENDIF.h"
#include "Instruction_LOOP.h"
#include "Instruction_ENDLOOP.h"


#include<vector>
#include<stack>
#include<iostream>
#include<string>
#include<stdlib.h>
#include<fstream>

using namespace std;

Operand* Machine::findOperand(char c) {
	for (int i = 0; i < operands.size(); i++) {
		if (c - 'A' == operands[i]->id) return operands[i]; 
	}
	return NULL;
}

Machine::Machine() {
	this->program_counter = 0;
	this->program_size = 0;
}

void Machine::processLine(Instruction* tmp, string tmps, int& i, vector<int>& labels) {
	while (i < tmps.size()) {
		//cout << tmp->_type;
		//cout << tmps;
		string tmptmp = "";
		while (tmps[i] != ' ' && tmps[i] != '\n' && i<tmps.size()) {
			tmptmp += tmps[i];
			i++;
		}
		//cout << tmps[i];
		i++;
		if (tmptmp.size() == 1 && (tmptmp[0] >= 'A' && tmptmp[0] <= 'Z')) {
			if (find(labels.begin(), labels.end(), tmptmp[0] - 'A') != labels.end()) {
				//cout << tmptmp;
				tmp->operands_for_line.push_back(findOperand(tmptmp[0]));
			}
			else {
				Operand* op = new Operand;
				op->getFromString(tmptmp);
				//cout << tmptmp;
				labels.push_back(tmptmp[0] - 'A');
				tmp->operands_for_line.push_back(op);
				operands.push_back(op);
			}
			continue;
		}
		//cout << tmptmp << endl;
		// dakle broj je
		Operand* op = new Operand;
		op->getFromString(tmptmp);
		tmp->operands_for_line.push_back(op);
	}
	//cout << labels.size() << endl;
	//cout << "izasao";
	//cout << tmps << endl;
	//cout << tmp->_type;
    list_of_instructions.push_back(tmp);
	return;
}

// seti se da dodas liniju za zavrsetak koda zbog while-a u Machine::execute()
void Machine::loadProgram(const string& filepath) {
	fstream my_file;
	my_file.open(filepath, ios::in); 
	if (!my_file) {
		cout << "No such file";
	}
	else {
		bool new_line = false;
		char c;
		int i = 0;
		vector<int> labels;
		string tmps = "";

		// ifgr, ifeq, else, endif

		stack<int> ifgr_ifeq;
		stack<int> _else; // uparen s sledecim endinf 
		stack<int> loop;

		while (1) {
			my_file.get(c);
			//cout << c;
			if (my_file.eof()) break;
			//cout << c;
			if (c == '\n') new_line = true;
			if (!new_line) {
				tmps += c;
				if (my_file.peek() == EOF) goto here;
			}
			else {
				
			here: new_line = false;
				// obradjujemo string tmps
				string type = "";
				//cout << tmps;
				while (tmps[i] != ' ' && i < tmps.size()) {
					type += tmps[i];
					i++;
				}
				i++;
				if (type == "SET") {
					//cout << labels.size() << "before" << endl;
					Instruction* tmp = new Instruction_SET;
					processLine(tmp, tmps, i, labels);
					//cout << labels.size() << "after" << endl;
				}
				if (type == "ADD") {
					Instruction* tmp = new Instruction_ADD;
					processLine(tmp, tmps, i, labels);
				}
				if (type == "SUB") {
					Instruction* tmp = new Instruction_SUB;
					processLine(tmp, tmps, i, labels);
				}
				if (type == "MUL") {
					Instruction* tmp = new Instruction_MUL;
					processLine(tmp, tmps, i, labels);
				}
				if (type == "DIV") {
					//cout << "DIJDD";
					Instruction* tmp = new Instruction_DIV;
					processLine(tmp, tmps, i, labels);
				}
				if (type == "GOTO") {
					Instruction* tmp = new Instruction_GOTO;
					processLine(tmp, tmps, i, labels);
				}
				if (type == "IFEQ" || type == "IFGR") {
					ifgr_ifeq.push(this->program_size); 
					//cout << "ABCD";
					Instruction* tmp = new Instruction_IF;
					if (type == "IFEQ") tmp->_type = 'e';
					if (type == "IFGR") tmp->_type = 'g';
					//cout << tmp->_type;
					processLine(tmp, tmps, i, labels);
				}
				if (type == "ELSE") {
					if (!ifgr_ifeq.empty()) { // inace ce ovo biti greska
						int x = ifgr_ifeq.top();
						ifgr_ifeq.pop(); 
						list_of_instructions[x]->paired_else = this->program_size;
						//cout << x << "else" << this->program_size  << endl;
					}
					_else.push(this->program_size);
					Instruction* tmp = new Instruction_ELSE;
					processLine(tmp, tmps, i, labels);
				}
				if (type == "ENDIF") {
					if (!_else.empty()) { // inace ce ovo biti greska
						int x = _else.top();
						_else.pop(); 
						list_of_instructions[x]->paired_endif = this->program_size;
						//cout << x << "endif" << this->program_size << endl;
					}
					Instruction* tmp = new Instruction_ENDIF;
					processLine(tmp, tmps, i, labels);
				}
				if (type == "LOOP") {
					loop.push(this->program_size); 
					Instruction* tmp = new Instruction_LOOP;
					processLine(tmp, tmps, i, labels);
				}
				if (type == "ENDLOOP") {
					Instruction* tmp = new Instruction_ENDLOOP;
					processLine(tmp, tmps, i, labels);
					if (!loop.empty()) { // inace greska
						int x = loop.top();
						loop.pop();
						list_of_instructions[this->program_size]->paired_endloop = x;
						if (list_of_instructions[x]->operands_for_line.size() == 1) list_of_instructions[this->program_size]->how_many = list_of_instructions[x]->operands_for_line[0]->value;
						else list_of_instructions[this->program_size]->how_many = -2;
						//cout << x << "AUB" << this->program_size << endl;
						//cout << list_of_instructions[this->program_size]->how_many;
					}
				}
				tmps = "";
				i = 0;
				this->program_size++;
			}
		}
		// dodavanje END linije
		//string 
	}
	my_file.close();
	//cout << operands.size();
}


void Machine::execute(const string& filepath) {
	this->program_counter = 0;
	//cout << list_of_instructions.size();
	while (this->program_counter < this->program_size) {
		//cout << list_of_instructions[this->program_counter]->paired_else;
		list_of_instructions[this->program_counter]->executeInstruction(&this->program_counter); 
	}
	int last_one = 0;
	for (int i = 0; i < 26; i++) if (this->findOperand((char)('A' + i)) != NULL) last_one = i;
	ofstream output(filepath);
	if (output.is_open()) {
		for (int i = 0; i < 26; i++) {
			if (this->findOperand((char)('A' + i)) == NULL) continue;
			output << (char)('A' + i) << "=" << this->findOperand((char)('A' + i))->value;
			if (i != last_one) output << endl;
			//cout << (char)('A' + i) << "=" << this->findOperand((char)('A'+i))->value<<endl;
		}
	}
	else {
		cout << "ERROR" << endl;
	}
	/*cout << 'A' + 1;
	fstream my_file;
	my_file.open(filepath, ios::out);
	for (int i = 0; i < 26; i++) {
		if (this->findOperand('A' + i) == NULL) continue;
		my_file << 'A' + i << "=" << this->findOperand('A' + i)->value;
		cout << 'A' + i;
	}*/
	//cout << this->findOperand('A')->value << endl;
	//cout << this->findOperand('B')->value << endl;
	//cout << this->findOperand('C')->value << endl;
	// ovde bih stampao rez
}





