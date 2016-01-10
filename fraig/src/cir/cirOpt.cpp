/****************************************************************************
  FileName     [ cirSim.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir optimization functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <cassert>
#include "cirMgr.h"
#include "cirGate.h"
#include "util.h"

using namespace std;

// TODO: Please keep "CirMgr::sweep()" and "CirMgr::optimize()" for cir cmd.
//       Feel free to define your own variables or functions
void replaceInput(CirGate*, CirGate*, bool);
/*******************************/
/*   Global variable and enum  */
/*******************************/

/**************************************/
/*   Static varaibles and functions   */
/**************************************/

/**************************************************/
/*   Public member functions about optimization   */
/**************************************************/
// Remove unused gates
// DFS list should NOT be changed
// UNDEF, float and unused list may be changed
void CirMgr::sweep() {
	for (unsigned i = 0, n = dfsList.size(); i < n; ++i)
		dfsList[i]->flag = true;
	
	for (unsigned i = 0, n = gateList.size(); i < n; ++i) {
		if (gateList[i] != 0 && gateList[i]->flag == false 
				&& (gateList[i]->isAig() || gateList[i]->getType() == UNDEF_GATE)) {
			for (unsigned j = 0; j < 2; ++j) {
				CirGate* g = gateList[i]->getInput(j);
				if (g != 0 && (g->flag == true || g->getType() == PI_GATE)) {
					g->deleteOutput(gateList[i]);
				}
			}
			if (gateList[i]->isAig())	--ands;
			cout << "Sweeping: " + gateList[i]->getTypeStr() + '('
				<< gateList[i]->getGateId() << ") removed...\n";
			delete gateList[i];
			gateList[i] = 0;
		}
	}
	resetFlag();
}

// Recursively simplifying from POs;
// _dfsList needs to be reconstructed afterwards
// UNDEF gates may be delete if its fanout becomes empty...
void CirMgr::optimize() {
	CirGate* const0 = gateList[0];
	for (unsigned i = 0, n = dfsList.size(); i < n; ++i) {
		CirGate* g = dfsList[i];
		if (!g->isAig())	continue;
		CirGate	*g1 = g->getInput(0),
					*g2 = g->getInput(1);
		bool first, second, isOne = false;
		if (g1 == g2) { // check same input
			first = g->isInv(0);
			second = g->isInv(1);
			g1->deleteOutput(g);	g1->deleteOutput(g);
			CirGate* prev;
			if (first == second) {
				prev = g1;
				isOne = first;
			} else {
				prev = const0;
			}
			replaceInput(g, prev, isOne);

			gateList[g->getGateId()] = 0;
			delete g;
			--ands;
			continue;
		}
		unsigned index = 2; // check one of the inputs is const0
		first = false; second = false; isOne = false;
		if (g1 == const0) {
			index = 0;
			if (g->isInv(0))	isOne = true;
		} else if (g2 == const0) {
			index = 1;
			if (g->isInv(1))	isOne = true;
		}
		if (index == 2)	continue;
		if (isOne) {
			replaceInput(g, g->getInput(1 - index), g->isInv(1 - index));
		} else {
			replaceInput(g, const0, false);
		}
		const0->deleteOutput(g);
		g->getInput(1 - index)->deleteOutput(g);

		gateList[g->getGateId()] = 0;
		delete g;
		--ands;
	}
	createDFS();
	/*
	for (unsigned i = 0, n = const0->getOutputs().size(); i < n; ++i) {
		CirGate* g = const0->getOutput(i);
		if (!g->isAig() || g->flag == false)	continue;
		cout << "Simplifying: 0 merging " << g->getGateId() << "...\n";
		int type = -1; // 0: const0, 1: const1
		gateList[i] = 0;
		--i;
		for (unsigned j = 0; j < 2; ++j) {
			if (g->getInput(j) == const0) {
				if (g->isInv(j) && type != 0)
					type = 1;
				else	type = 0;
			}
		}
		if (type == 0) {
			unsigned index = 2;
			for (unsigned j = 0, size = g->getOutputSize(); j < size; ++j) {
				CirGate* gout = g->getOutput(j);
				if (gout->getInput(0) == g)	index = 0;
				else if (gout->getInput(1) == g)	index = 1;
				if (index == 2)	cout << "input and output don't match...\n";
				gout->addInput(const0, gout->isInv(index));
				gout->deleteInput(index);
			}
			index = 2;
			if (g->getInput(0) == const0)	index = 1;
			else if (g->getInput(1) == const0)	index = 0;
			if (index == 2)	cout << "input and output don't match....\n";
			g->getInput(index)->deleteOutput(g);

			delete g;
		}
		if (type == 1) {
			unsigned index = 2;
			if (g->getInput(0) == const0)	index = 1;
			else if (g->getInput(1) == const0)	index = 0;
			if (index == 2)	cout << "input and output don't match....\n";
			CirGate* gin = g->getInput(index);
			gin->deleteOutput(g);
			for (unsigned j = 0, size = g->getOutputSize(); j < size; ++j) {
				CirGate* gout = g->getOutput(j);
				index = 2;
				if (gout->getInput(0) == g)	index = 0;
				else if (gout->getInput(1) == g)	index = 1;
				if (index == 2)	cout << "input and output don't match....\n";
				gout->deleteInput(index);
				gout->addInput(gin);
				gin->addOutput(gout);
			}
			delete g;
		}

		else if (type == -1)	cout << "output and input don't match...\n";
		
	}
	resetFlag();
	*/
}

void replaceInput(CirGate* g, CirGate* prev, bool isOne) {
	cout << "Simplifying: " << prev->getGateId()	<< " merging ";
	if (isOne)	cout << '!';
	cout << g->getGateId() << "...\n";

	for (unsigned i = 0, n = g->getOutputSize(); i < n; ++i) {
		CirGate* temp = g->getOutput(i);
		unsigned index = 1;
		if (temp->getInput(0) == g)	index = 0;
		temp->addInput(prev, isOne != temp->isInv(index));
		prev->addOutput(temp);
		temp->deleteInput(index);
	}
}
/***************************************************/
/*   Private member functions about optimization   */
/***************************************************/
