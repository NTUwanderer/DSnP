/****************************************************************************
  FileName     [ cirFraig.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir FRAIG functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2012-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <cassert>
#include "cirMgr.h"
#include "cirGate.h"
#include "sat.h"
#include "myHashMap.h"
#include "util.h"

using namespace std;

// TODO: Please keep "CirMgr::strash()" and "CirMgr::fraig()" for cir cmd.
//       Feel free to define your own variables or functions
size_t getHashKey(CirGate*);
void inheriteOutputs(CirGate*, CirGate*);
/*******************************/
/*   Global variable and enum  */
/*******************************/

/**************************************/
/*   Static varaibles and functions   */
/**************************************/

/*******************************************/
/*   Public member functions about fraig   */
/*******************************************/
// _floatList may be changed.
// _unusedList and _undefList won't be changed
void CirMgr::strash() {
	HashMap<size_t, CirGate*>* map = new HashMap<size_t, CirGate*>(dfsList.size());
	
	for (unsigned i = 0, size = dfsList.size(); i < size; ++i) {
		CirGate* g = dfsList[i];
		if (!g->isAig())	continue;
		size_t hashKey = getHashKey(g);
		if (!map->insert_check(hashKey, g)) {
			CirGate* g1 = dfsList[i];
			cout << "Strashing: " << g->getGateId() << " merging "
				<< g1->getGateId() << "...\n";
			g1->getInput(0)->deleteOutput(g1);
			g1->getInput(1)->deleteOutput(g1);
			inheriteOutputs(g1, g);

			gateList[g1->getGateId()] = 0;
			delete g1;
			--ands;
		}
	}
	delete map;

	createDFS();
}

void CirMgr::fraig() {

}

/********************************************/
/*   Private member functions about fraig   */
/********************************************/

size_t getHashKey(CirGate* g) {
	size_t id1 = (g->getInput(0)->getGateId() << 1) + g->isInv(0),
			 id2 = (g->getInput(1)->getGateId() << 1) + g->isInv(1);
	if (id1 < id2)	return (id1 << (sizeof(size_t) / 2)) + id2;
	else				return (id2 << (sizeof(size_t) / 2)) + id1;
}

void inheriteOutputs(CirGate* g, CirGate* ng) {
	for (unsigned i = 0, n = g->getOutputSize(); i < n; ++i) {
		CirGate* temp = g->getOutput(i);
		unsigned index = 1;
		if (temp->getInput(0) == g)	index = 0;
		temp->addInput(ng, temp->isInv(index));
		ng->addOutput(temp);
		temp->deleteInput(index);
	}
}


