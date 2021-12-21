#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <iostream>
#include <exception>

using namespace std;

class InstructionSyntaxError : public exception {
public: 
	InstructionSyntaxError(string& name, int line) {
		cout << "Syntax error in line " << line << ": Instruction " << name << " is non-existent." << endl;
	}
	InstructionSyntaxError(int line) {
		cout << "Syntax error in line " << line << ": Division by 0." << endl;
	}
	InstructionSyntaxError() {
		cout << "Syntax error: Instructions are not paired correctly." << endl;
	}
};

class OperandSyntaxError : public exception {
public:
	OperandSyntaxError(string &name, int line) {
		string tmp;
		if (name == "SET" || name == "IFGR" || name == "IFEQ") tmp = "2.";
		if (name == "ADD" || name == "SUB" || name == "DIV" || name == "MUL") tmp = "3.";
		if (name == "ELSE" || name == "ENDIF" || name == "ENDLOOP") tmp = "0.";
		if (name == "GOTO") tmp = "1.";
		if (name == "LOOP") tmp = "0 or 1.";
		cout << "Syntax error in line " << line << ": number of operands does not match the instruction." << endl;
		cout << "Expected number of operands for instruction " << name << " is " << tmp << endl;
	}
	OperandSyntaxError(int line, string op) {
		cout << "Syntax error in line " << line << ": operand " << op << " is not valid." << endl;
		cout << "Expected upper-case letter or integer." << endl;
	}
};

class FileError : public exception {
public:
	FileError(const string &name) {
		cout << "FileError: File named " << name << " not found." << endl;
	}
};

#endif