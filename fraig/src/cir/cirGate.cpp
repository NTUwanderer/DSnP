/****************************************************************************
  FileName     [ cirGate.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define class CirAigGate member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdarg.h>
#include <cassert>
#include "cirGate.h"
#include "cirMgr.h"
#include "util.h"

using namespace std;

// TODO: Keep "CirGate::reportGate()", "CirGate::reportFanin()" and
//       "CirGate::reportFanout()" for cir cmds. Feel free to define
//       your own variables and functions.

extern CirMgr *cirMgr;

unsigned CirGate::index = 0;
/**************************************/
/*   class CirGate member functions   */
/**************************************/
string unsToStr(unsigned n) {
	stringstream ss;
	ss << n;
	return ss.str();
}

string CirGate::getTypeStr() const {
	if (gateType == UNDEF_GATE)	return "UNDEF";
	if (gateType == PI_GATE)	return "PI";
	if (gateType == PO_GATE)	return "PO";
	if (gateType == AIG_GATE)	return "AIG";
	if (gateType == CONST_GATE)	return "CONST";
	if (gateType == TOT_GATE)	return "TOT";
	return "UNDEF";
}

void CirGate::printGate() const {
/*
	if (flag)	return;
	if (gateType == UNDEF_GATE)	return;
	for (size_t i = 0; i < inputs.size(); ++i)
		getInput(i)->printGate(); 
	string info = "";
	if (gateType == CONST_GATE) info = "0";
	else {
		info = unsToStr(gateId);
		for (size_t i = 0; i < inputs.size(); ++i) {
			info += ' ';
			if (getInput(i)->getType() == UNDEF_GATE)	info += '*';
			if (isInv(i))	info += '!';
			info += unsToStr(getInput(i)->getGateId());
		}
		if (symbol != "")	info += " (" + symbol + ')';
	}
	cout 	<< '[' << index++ << "] " << setw(4) << left 
			<< getTypeStr() << info << endl;
	flag = true;
*/
	string info = "";
	if (gateType == CONST_GATE) info = "0";
	else {
		info = unsToStr(gateId);
		for (size_t i = 0; i < inputs.size(); ++i) {
			info += ' ';
			if (getInput(i)->getType() == UNDEF_GATE)	info += '*';
			if (isInv(i))	info += '!';
			info += unsToStr(getInput(i)->getGateId());
		}
		if (symbol != "")	info += " (" + symbol + ')';
	}
	cout 	<< setw(4) << left << getTypeStr() << info << endl;
}

void CirGate::reportGate() const {
	cout << "==================================================\n";
	stringstream ss;
	ss << "= " + getTypeStr() << '(' << getGateId() << ")";
	if (symbol != "")	ss << "\"" << symbol << "\"";
	ss << ", line " << getLineNo();
	cout << setw(49) << left << ss.str() << "=\n";
	cout << "==================================================\n";
}

void CirGate::reportFanin(int level) const {
	assert (level >= 0);
	myFanin(level, false);
	cirMgr->resetFlag();
}

void CirGate::myFanin(int level, bool inv) const {
	for (unsigned i = 0; i < index; ++i)	cout << "  ";
	if (inv)	cout << '!';
	cout << getTypeStr() << ' ' << gateId;
	if (level == 0)	cout << endl;
	else if (flag)	cout << " (*)\n";
	else {
		cout << endl;
		if (!inputs.empty())	flag = true;
		for (unsigned i = 0, size = inputs.size(); i < size; ++i) {
			++index;
			getInput(i)->myFanin(level - 1, isInv(i));
		}
	}
	if (index != 0)	--index;
}

void CirGate::reportFanout(int level) const {
	assert (level >= 0);
	myFanout(level, false);
	cirMgr->resetFlag();
}

void CirGate::myFanout(int level, bool inv) const {
	for (unsigned i = 0; i < index; ++i)	cout << "  ";
	if (inv)	cout << '!';
	cout << getTypeStr() << ' ' << gateId;
	if (level == 0)	cout << endl;
	else if (flag) cout << " (*)\n";
	else {
		cout << endl;
		if(!outputs.empty())	flag = true;
		for (unsigned i = 0, size = outputs.size(); i < size; ++i) {
			++index;
			const CirGate* g = getOutput(i);
			bool n_inv = false;
			unsigned j = 0;
			while (true) {
				CirGate* g2 = g->getInput(j);
				if (g2 == 0)	break;
				if (this == g2) {
					n_inv = g->isInv(j);
					break;
				}
				++j;
			}
			getOutput(i)->myFanout(level - 1, n_inv);
		}
	}
	if (index != 0)	--index;
}

void CirGate::printAig(string& s, unsigned& cnt) const {
	if (flag) return;
	for (unsigned i = 0; i < inputs.size(); ++i)
		getInput(i)->printAig(s, cnt);
	if (gateType == AIG_GATE) {
		flag = true;
		stringstream ss;
		ss << gateId * 2
			<< ' ' << getInput(0)->getGateId() * 2 + isInv(0)
			<< ' ' << getInput(1)->getGateId() * 2 + isInv(1)
			<< '\n';
		s += ss.str();
		++cnt;
	}
}

unsigned CirGate::printAig(vector<string>* input_gates, vector<string>* in_names, vector<string>* and_gates) const {
	if (flag || gateType == CONST_GATE)	return gateId;
	flag = true;
	if (gateType == PI_GATE) {
		if (symbol != "")
			in_names->push_back("i" + unsToStr(input_gates->size()) + ' ' + symbol);
		input_gates->push_back(unsToStr(2 * gateId));
		return gateId;
	}
	else if (gateType == AIG_GATE) {
		unsigned var1 = getInput(0)->printAig(input_gates, in_names, and_gates),
					var2 = getInput(1)->printAig(input_gates, in_names, and_gates);
		if (var2 > var1)	var1 = var2;
		and_gates->push_back(unsToStr(2 * gateId) + ' ' + 
				unsToStr(2 * getInput(0)->getGateId() + isInv(0)) + ' ' + 
				unsToStr(2 * getInput(1)->getGateId() + isInv(1)));
		if (gateId < var1)	return var1;
		return gateId;
	}
	cout << "printAig error, typeStr: " + getTypeStr() << endl;
	return gateId;
}

void CirGate::deleteInput(size_t i) {
	inputs.erase(inputs.begin() + i);
}

void CirGate::deleteInput(CirGate* g, bool inv = false) {
	for (size_t i = 0, n = inputs.size(); i < n; ++i)
		if (getInput(i) == g && isInv(i) == inv)
			inputs.erase(inputs.begin() + i);
}

void CirGate::deleteOutput(CirGate* g) {
	for (unsigned i = 0, n = outputs.size(); i < n; ++i) {
		if (g == outputs[i]) {
			outputs.erase(outputs.begin() + i);
			return;
		}	
	}
}

void CirGate::deleteOutput(unsigned i) {
	outputs.erase(outputs.begin() + i);
}

void CirGate::createDFS() {
	if (flag) 	return;
	flag = true;
	for (unsigned i = 0, size = inputs.size(); i < size; ++i)
		getInput(i)->createDFS();
	
	cirMgr->addInDfsList(this);
}


