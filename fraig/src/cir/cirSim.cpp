/****************************************************************************
  FileName     [ cirSim.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir simulation functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <fstream>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cassert>
#include "cirMgr.h"
#include "cirGate.h"
#include "util.h"

using namespace std;

// TODO: Keep "CirMgr::randomSim()" and "CirMgr::fileSim()" for cir cmd.
//       Feel free to define your own variables or functions

size_t randomSize_t();
/*******************************/
/*   Global variable and enum  */
/*******************************/

/**************************************/
/*   Static varaibles and functions   */
/**************************************/

/************************************************/
/*   Public member functions about Simulation   */
/************************************************/
void CirMgr::randomSim() {
	IdList idList;	idList.resize(gateList.size(), 0);
	unsigned count_ins = resetFEC(idList), count = 0, max_round = 100; // max_round TBD
	srand(time(NULL));
	size_t* inputs = new size_t[count_ins], sims = new size_t[dfsList.size()];

	while (count < max_round) {
		for (unsigned i = 0; i < count_ins; ++i)
			inputs[i] = randomSize_t();
	
		simOneTime(idList, inputs, count_ins);
	}
	
}

void CirMgr::fileSim(ifstream& patternFile) {

}

/*************************************************/
/*   Private member functions about Simulation   */
/*************************************************/

unsigned CirMgr::resetFEC(IdList& idList) {
	FEC.clear();
	IFEC.clear();
	unsigned count_ins = 0;
	for (unsigned i = 0, size = dfsList.size(); i < size; ++i) {
		FEC.push_back(dfsList[i]->getGateId());
		idList[dfsList[i]->getGateId()] = i;
		if (dfsList[i]->getType() == PI_GATE)	++count_ins;
	}
	return count_ins;
}

void simOneTime(const IdList& idList, const size_t* inputs, const unsigned& count_ins) {
	unsigned count = 0;
	for (unsigned i = 0, size = dfsList.size(); i < size; ++i) {
		CirGate* g = dfsList[i];
		if (g->getType() == PI_GATE)
			sims[i] = inputs[count++];
		else if (g->getType() == AIG_GATE) {
			size_t in1 = sims[idList[g->getInput(0)->getGateId()]],
					 in2 = sims[idList[g->getInput(1)->getGateId()]];
			if (g->isInv(0))	in1 = !in1;
			if (g->isInv(1))	in2 = !in2;
			sims[i] = (in1 & in2);
		}
	}

}

size_t randomSize_t() {
	size_t temp = (size_t)rand();
	return (temp << 32) + ((size_t)rand());
}
