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

extern CirMgr *cirMgr;

unsigned CirGate::index = 0;
// TODO: Implement memeber functions for class(es) in cirGate.h

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
	//cout << index << ": gateId " << gateId << endl;
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
			else if (isInv(i))	info += '!';
			info += unsToStr(getInput(i)->getGateId());
		}
	}
	cout 	<< '[' << index++ << "] " << setw(4) << left 
			<< getTypeStr() << info << endl;
	flag = true;
}

void CirGate::reportGate() const {
	cout << "==================================================\n";
	stringstream ss;
	ss << "= " + getTypeStr() << '(' << getGateId() << "), line " << getLineNo();
	cout << setw(49) << left << ss.str() << "=\n";
	cout << "==================================================\n";
}

void CirGate::reportFanin(int level) const {
   assert (level >= 0);
}

void CirGate::reportFanout(int level) const {
   assert (level >= 0);
}

