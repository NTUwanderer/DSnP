/****************************************************************************
  FileName     [ cirGate.h ]
  PackageName  [ cir ]
  Synopsis     [ Define basic gate data structures ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CIR_GATE_H
#define CIR_GATE_H

#include <string>
#include <vector>
#include <iostream>
#include "cirDef.h"
#include "sat.h"

#define NEG 0x1
using namespace std;

// TODO: Feel free to define your own classes, variables, or functions.

class CirGate;

//------------------------------------------------------------------------
//   Define classes
//------------------------------------------------------------------------
class CirGate
{
public:
   CirGate(GateType type, unsigned no = 0, unsigned id = 0):
    flag(false), gateType(type), lineNo(no), gateId(id), symbol("") {}
   virtual ~CirGate() { inputs.clear(); outputs.clear(); }

   // Basic access methods
  	void setGateType(GateType g) { gateType = g; }
  	GateType getType() const { return gateType; }
   string getTypeStr() const; 
  	void setLineNo(unsigned no) { lineNo = no; }
   unsigned getLineNo() const { return lineNo; }
  	void setGateId(unsigned id) { gateId = id; }
  	unsigned getGateId() const { return gateId; }
  	void setSymbol(string s) { symbol = s; }
  	string getSymbol() const { return symbol; }

   // Printing functions
   void printGate() const;
   void reportGate() const;
   void reportFanin(int level) const;
  	void myFanin(int level, bool inv) const;
   void reportFanout(int level) const;
  	void myFanout(int level, bool inv) const;
  	void printAig(string& s, unsigned& cnt) const;
  
  	void addInput(CirGate* g, bool inv = false) {
    	if (inv)  g = (CirGate*)((size_t)g + 1);
    	inputs.push_back(g);
  	}
  	CirGate* getInput(size_t i) const {
    	if (i >= inputs.size()) return 0;
    	return (CirGate*)(((size_t)inputs[i]) & ~size_t(NEG));
  	}
	
	void deleteInput(size_t);
	void deleteInput(CirGate*, bool);
  	bool isInv(size_t i) const { return ((size_t)inputs[i] & NEG); }

  	void addOutput(CirGate* g) { outputs.push_back(g); }
  	GateList getOutputs() const { return outputs; }
	unsigned getOutputSize() const {	return outputs.size(); }
  	CirGate* getOutput(size_t i) const {
    	if (i >= outputs.size()) return 0;
    	return outputs[i];
  	}
	void deleteOutput(CirGate*);
	void deleteOutput(unsigned);
  	void clearOutput() { outputs.clear(); }
  	void createDFS();

  	bool isAig() const { return gateType == AIG_GATE; }

  	static unsigned index;

  	mutable bool flag;
private:
  	GateType gateType;
  	unsigned lineNo;
  	unsigned gateId;
 	string symbol;

 	GateList inputs;
  	GateList outputs;
};

#endif // CIR_GATE_H
