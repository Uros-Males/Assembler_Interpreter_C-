#include "Operand.h"

Operand::Operand() {
	this->value = 0;
	this->id = -1;
}

Operand::Operand(int new_value, int idx) {
	this->value = new_value;
	this->id = idx;
}

void Operand::changeValue(int new_value) {
	this->value = new_value;
}

void Operand::setId(int idx) {
	this->id = idx;
}

void Operand::getFromString(string s) {
	if (s.size() == 1 && (s[0] >= 'A' && s[0] <= 'Z')) {
		this->id = s[0] - 'A'; 
		this->value = 0;
		return;
	}
	// dakle broj je
	if (s.size() == 1) {
		this->id = -1;
		this->value = stoi(s);
		return;
	}
	if (s[0] == '-') {
		string tmp; 
		for (int i = 1; i < s.size(); i++) tmp += s[i];
		this->id = -1;
		this->value = -1 * stoi(tmp);
	}
	else {
		this->id = -1;
		this->value = stoi(s);
		return;
	}
}