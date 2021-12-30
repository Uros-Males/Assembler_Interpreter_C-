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
#include "Exceptions.h"

#define _CRTDBG_MAP_ALLOC
#include<iostream>
#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

#include<vector>
#include<stack>
#include<string>
#include<stdlib.h>
#include<fstream>

using namespace std;

//funkcija koja vraca operand s nazivom char c
//ukoliko takav operand ne postoji u nasem kodu, vracamo NULL
Operand* Machine::findOperand(char c) {
	for (int i = 0; i < operands.size(); i++) {
		if (c - 'A' == operands[i]->id) return operands[i]; 
	}
	return NULL;
}

//konstruktor klase Machine
Machine::Machine() {
	this->failed = 0;
	this->program_counter = 0;
	this->program_size = 0;
}

//provera da li je operand dobro unet (da li je slovo/broj)
bool Machine::checkOperand(string s) {
	if (s.size() == 0) return false; 
	if (s.size() == 1 && !((s[0] >= 'A' && s[0] <= 'Z') || (s[0] >= '0' && s[0] <= '9'))) return false;
	if (s.size() > 1){
		if(s[0]=='-') for (int i = 1; i < s.size(); i++) if (s[i] < '0' || s[i]>'9') return false;
		if(s[0]!='-') for (int i = 0; i < s.size(); i++) if (s[i] < '0' || s[i]>'9') return false;
	}
	return true;
}

// u ovoj funkciji prolazimo kroz trenutnu liniju naseg koda (sadrzanu u stringu tmps)
// u njoj deklarisemo jedan po jedan operand gde svaki od njih procitamo iz tmps u string tmptmp
// ukoliko tmptmp nije korektan naziv operanda, program prijavljuje gresku
// ukoliko je operand promenljiva koja se jos uvek nije pojavila u kodu 
// (a to proveravamo preko vektora labels, koji sadrzi sve nazive promenljivih pomenute do tog trenutka u kodu)
// stavljamo ga u vektor labels i konstruisemo novi objekat klase Operand
// tako sto pozovemo funkciju clanicu klase Operand, getFromString
// koja nam iz stringa tmptmp izvlaci podatke (odnosno IME promenljive) i pravi novi objekat klase Operand
// ukoliko je operand brojna vrednost u tom slucaju opet pozivamo funkciju getFromString
// koja nam iz stringa tmptmp izvlaci podatke (odnosno VREDNOST) i pravi novi objekat klase Operand
// kako napravimo operand, stavimo ga u vektor tmp->operands_for_line
// tmp je instrukcija koju trenutno obradjujemo (tmp=temporary, privremeno) 
// a operands_for_line je vektor objekata klase Operand, u klasi Instruction koji sadrzi sve operande te instrukcije 
// dakle kada smo u instrukciju tmp ubacili sve njene operande (preciznije u vektor operands_for_line instrukcije tmp)
// sada tu instrukciju tmp ubacujemo u vektor list_of_instructions klase Machine koji ce na kraju funkcije loadProgram
// sadrzati sve instrukcije koje se pojavljuju u kodu i koje cemo potom redom izvrsavati
void Machine::processLine(Instruction* tmp, string tmps, int& i, vector<int>& labels) {
	while (i < tmps.size()) {
		string tmptmp = "";
		while (tmps[i] != ' ' && tmps[i] != '\n' && i<tmps.size()) {
			tmptmp += tmps[i];
			i++;
		}
		i++;
		if (!checkOperand(tmptmp)) {
			throw new OperandSyntaxError(this->program_size, tmptmp); 
			return;
		}
		if (tmptmp.size() == 1 && (tmptmp[0] >= 'A' && tmptmp[0] <= 'Z')) {
			if (find(labels.begin(), labels.end(), tmptmp[0] - 'A') != labels.end()) {
				tmp->operands_for_line.push_back(findOperand(tmptmp[0]));
			}
			else {
				Operand* op = new Operand;
				op->getFromString(tmptmp);
				labels.push_back(tmptmp[0] - 'A');
				tmp->operands_for_line.push_back(op);
				operands.push_back(op);
			}
			continue;
		}
		Operand* op = new Operand;
		op->getFromString(tmptmp);
		tmp->operands_for_line.push_back(op);
		this->to_delete.push_back(op);
	}
    list_of_instructions.push_back(tmp);
	return;
}

// brisanje sadrzaja list_of_instructions 
// i operanada u njima 
 void Machine::clear() {
	for (int i = 0; i < this->list_of_instructions.size(); i++) {
		delete this->list_of_instructions[i];
		for (int j = 0; j < this->list_of_instructions[i]->operands_for_line.size(); j++) {
			delete this->list_of_instructions[i]->operands_for_line[j]; 
		}
	}
	for (int i = 0; i < this->to_delete.size(); i++) delete this->to_delete[i];
	for (int i = 0; i < this->operands.size(); i++) delete this->operands[i];
}

//u ovoj funkciji prolazimo kroz nas file i idemo liniju po liniju
// ukoliko fajl s imenom filepath ne postoji prijavljujemo gresku
// kada obradjujemo trenutnu liniju za nju imamo izdvojen njen string
// nalazenjem prve reci u tom stringu dobijamo tip instrukcije (type u kodu) - ukoliko ne postoji instrukcija tipa type javljamo gresku
// potom deklarisemo tu instrukciju u zavisnosti od njenog tipa (da li je ADD,SET,...)
// i proverimo njen broj operanada (koji smo dobili u funkciji processLine)
// ako broj operanada ne odgovara datom tipu instrukcije prijavljujemo gresku
// sve vreme imamo velicinu this->program_size koja nam govori koju liniju trenutno obradjujemo 
// i koja ce nam kasnije dati broj linija naseg koda
// kod obradi instrukcija granjanja ili loop-ova pazimo da svaka instrukcija bude povezana 
// s odgovarajucom instrukcijom (npr endloop s njegovim loop) a to realizujemo preko stack-a
// kad naidjemo na instrukciju loop ubacimo je u stack a kad naidjemo na endloop povezemo ga s vrhom steka i popujemo stack
// slicno i za if/endif i endif/else
// ukoliko nesto nije dobro upareno odnosno ukoliko je neki stack prazan a treba nam njegov vrh, prijavljujemo gresku
// endloop u sebi sadrzi broj ponavljanja "njegovog" loopa i u svakoj iteraciji loopa smanjujemo ga za 1 dok ne dostigne 0
// tad izlazimo iz loop-a a vrednost endloopa resetujemo na pocetnu (u slucaju ugnjezdenih petlji npr)
void Machine::loadProgram(const string& filepath) {
	try {
		fstream my_file;
		my_file.open(filepath, ios::in);
		if (!my_file) {
			throw new FileError(filepath);
		}
		else {
			bool new_line = false;
			char c;
			int i = 0;
			vector<int> labels;
			string tmps = "";

			stack<int> ifgr_ifeq;
			stack<int> _else;  
			stack<int> loop;

			while (1) {
				my_file.get(c);
				if (my_file.eof()) break;
				if (c == '\n') new_line = true;
				if (!new_line) {
					tmps += c;
					if (my_file.peek() == EOF) goto here;
				}
				else {

				here: new_line = false;
					int k = 0;
					string type = "";
					while (tmps[i] != ' ' && i < tmps.size()) {
						type += tmps[i];
						i++;
					}
					i++;
					if (type == "SET") {
						k++;	
						Instruction* tmp = new Instruction_SET;
						processLine(tmp, tmps, i, labels);
						if (this->list_of_instructions[this->program_size]->operands_for_line.size() != 2) throw new OperandSyntaxError(type, this->program_size);
					}
					if (type == "ADD") {
						k++;
						Instruction* tmp = new Instruction_ADD;
						processLine(tmp, tmps, i, labels);
						if (this->list_of_instructions[this->program_size]->operands_for_line.size() != 3) throw new OperandSyntaxError(type, this->program_size);

					}
					if (type == "SUB") {
						k++;
						Instruction* tmp = new Instruction_SUB;
						processLine(tmp, tmps, i, labels);
						if (this->list_of_instructions[this->program_size]->operands_for_line.size() != 3) throw new OperandSyntaxError(type, this->program_size);
					}
					if (type == "MUL") {
						k++;
						Instruction* tmp = new Instruction_MUL;
						processLine(tmp, tmps, i, labels);
						if (this->list_of_instructions[this->program_size]->operands_for_line.size() != 3) throw new OperandSyntaxError(type, this->program_size);

					}
					if (type == "DIV") {
						k++;
						Instruction* tmp = new Instruction_DIV;
						processLine(tmp, tmps, i, labels);
						if (this->list_of_instructions[this->program_size]->operands_for_line.size() != 3) throw new OperandSyntaxError(type, this->program_size);
					}
					if (type == "GOTO") {
						k++;
						Instruction* tmp = new Instruction_GOTO;
						processLine(tmp, tmps, i, labels);
						if (this->list_of_instructions[this->program_size]->operands_for_line.size() != 1) throw new OperandSyntaxError(type, this->program_size);
					}
					if (type == "IFEQ" || type == "IFGR") {
						k++;
						ifgr_ifeq.push(this->program_size);
						Instruction* tmp = new Instruction_IF;
						if (type == "IFEQ") tmp->_type = 'e';
						if (type == "IFGR") tmp->_type = 'g';
						processLine(tmp, tmps, i, labels);
						if (this->list_of_instructions[this->program_size]->operands_for_line.size() != 2) throw new OperandSyntaxError(type, this->program_size);

					}
					if (type == "ELSE") {
						k++;
						if (ifgr_ifeq.empty()) {
							throw new InstructionSyntaxError();
						}
						if (!ifgr_ifeq.empty()) { 
							int x = ifgr_ifeq.top();
							ifgr_ifeq.pop();
							list_of_instructions[x]->paired_else = this->program_size;
						}
						_else.push(this->program_size);
						Instruction* tmp = new Instruction_ELSE;
						processLine(tmp, tmps, i, labels);
						if (this->list_of_instructions[this->program_size]->operands_for_line.size() != 0)  throw new OperandSyntaxError(type, this->program_size);
					}
					if (type == "ENDIF") {
						k++;
						if (_else.empty()) {
							throw new InstructionSyntaxError(); 
						}
						if (!_else.empty()) { 
							int x = _else.top();
							_else.pop();
							list_of_instructions[x]->paired_endif = this->program_size;
						}
						Instruction* tmp = new Instruction_ENDIF;
						processLine(tmp, tmps, i, labels);
						if (this->list_of_instructions[this->program_size]->operands_for_line.size() != 0) throw new OperandSyntaxError(type, this->program_size);
					}
					if (type == "LOOP") {
						k++;
						loop.push(this->program_size);
						Instruction* tmp = new Instruction_LOOP;
						processLine(tmp, tmps, i, labels);
						if (this->list_of_instructions[this->program_size]->operands_for_line.size() > 1) throw new OperandSyntaxError(type, this->program_size);
					}
					if (type == "ENDLOOP") {
						k++;
						Instruction* tmp = new Instruction_ENDLOOP;
						processLine(tmp, tmps, i, labels);
						if (loop.empty()) {
							throw new InstructionSyntaxError();
						}
						if (!loop.empty()) { 
							int x = loop.top();
							loop.pop();
							list_of_instructions[this->program_size]->paired_endloop = x;
							if (list_of_instructions[x]->operands_for_line.size() == 1) {
								list_of_instructions[this->program_size]->how_many = list_of_instructions[x]->operands_for_line[0]->value;
								list_of_instructions[this->program_size]->original = list_of_instructions[x]->operands_for_line[0]->value;
							}
							else list_of_instructions[this->program_size]->how_many = -2;
						}
						if (this->list_of_instructions[this->program_size]->operands_for_line.size() != 0) throw new OperandSyntaxError(type, this->program_size);
					}
					if (!k) {
						throw new InstructionSyntaxError(type, this->program_size);
					}
					tmps = "";
					i = 0;
					if(k) this->program_size++;
				}
			}
			if (!loop.empty() || !ifgr_ifeq.empty() || !_else.empty()) throw new InstructionSyntaxError();

		}
		my_file.close();
	}
	catch (InstructionSyntaxError* e) {
		this->failed = 1;
		e->what();
		this->clear();
		delete e;
		return;
	}
	catch (OperandSyntaxError* e) {
		this->failed = 1;
		e->what();
		this->clear();
		delete e;
		return;
	}
	catch (FileError* e) {
		this->failed = 1;
		e->what(); 
		this->clear();
		delete e;
		return;
	}
}

// u ovoj funkciji vrsimo konacno izvrsavanje koda
// imamo velicinu this->program_counter koja nam govori na kojoj smo trenutno liniji izvrsavanja
// a koju update-ujemo u odgovarajucim pozivima instrukcija (executeInstruction)
// sve dok ne dodje do kraja programa i kasnije rezultat promenljivih (pozivom findOperand) upisujemo u izlazni fajl
// ovde su obradjene greske za fajlove i za deljenje nulom
void Machine::execute(const string& filepath) {
	if (this->failed) {
		ofstream output(filepath); 
		output << "failed";
		//this->clear();
		return; 
	}
	if (!this->failed) {
		this->program_counter = 0;
		try {
			while (this->program_counter < this->program_size) {
				int tmp = this->program_counter;
				list_of_instructions[this->program_counter]->executeInstruction(&this->program_counter);
				if (tmp == this->program_counter) throw new InstructionSyntaxError(tmp);
			}
			int last_one = 0;
			for (int i = 0; i < 26; i++) if (this->findOperand((char)('A' + i)) != NULL) last_one = i;
			ofstream output(filepath);
			if (output.is_open()) {
				for (int i = 0; i < 26; i++) {
					if (this->findOperand((char)('A' + i)) == NULL) continue;
					//cout << (char)('A' + i) << "=" << this->findOperand((char)('A' + i))->value << endl;
					output << (char)('A' + i) << "=" << this->findOperand((char)('A' + i))->value;
					if (i != last_one) output << endl;
				}
				this->clear();
				output.close();
			}
			else {
				throw new FileError(filepath);
			}
		}
		catch (InstructionSyntaxError* e) {
			this->failed = 1;
			ofstream output(filepath);
			output << "failed";
			e->what();
			this->clear();
			delete e;
			return;
		}
		catch (FileError* e) {
			ofstream output(filepath);
			this->failed = 1;
			e->what();
			this->clear();
			delete e;
			return;
		}
	}
}





