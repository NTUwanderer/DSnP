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

#define NEG 0x1

using namespace std;

class CirGate;

//------------------------------------------------------------------------
//   Define classes
//------------------------------------------------------------------------
// TODO: Define your own data members and member functions, or classes
class CirGate {
public:
   CirGate(unsigned no = 0, unsigned id = 0) lineNo(no), gateId(id) {}
   virtual ~CirGate() {}

   // Basic access methods
	void setGateType(GateType g) { gateType = g; }
   string getTypeStr() const; 
	void setLineNo(unsigned no) { lineNo = no; }
   unsigned getLineNo() const { return lineNo; }
	void setGateId(unsigned id) { gateId = id; }
	unsigned getGateId() const { return gateId; }

   // Printing functions
   virtual void printGate() const = 0;
   void reportGate() const;
   void reportFanin(int level) const;
   void reportFanout(int level) const;

	bool getConstValue() const { return false; }
	bool isInv(int i = 1) const { return false; }

	void setInput1(CirGate* g) {}
	CirGate* getInput1() const { return 0; }

	void setInput2(CirGate* g) {}
	CirGate* getInput2() const { return 0; }

	void addOutput(CirGate* g) {}
	GateList getOutput() { GateList g; return g; }
	void clearOutput() {}

protected:
	GateType gateType;
	unsigned lineNo;
	unsigned gateId;
};

class CirConstGate: public CirGate {
public:
	CirConstGate(unsigned no = 0, unsigned id = 0): CirGate(no, id) {}
	~CirConstGate() {}

	void printGate() const {}
};

class CirAigGate: public CirGate {
public:
	CirAigGate(unsigned no = 0, unsigned id = 0): CirGate(no, id), input1(0), input2(0) {}
	~CirAigGate() {}

	void printGate() const {}
	bool isInv(int i = 1) const {
		if (i == 1)	return (input1 & NEG);
		if (i == 2) return (input2 & NEG);
		return false;
	}

	void setInput1(CirGate* g, bool inv) { input1 = (inv)? (size_t)g: (size_t)g + NEG; }
	CirGate* getInput1() const { return (CirGate*)(input1 & ~size_t(NEG)); }
	void setInput2(CirGate* g, bool inv) { input2 = (inv)? (size_t)g: (size_t)g + NEG; }
	CirGate* getInput2() { return (CirGate*)(input2 & ~size_t(NEG)); }
	void addOutput(CirGate* g) { output.push_back(g); }
	GateList getOutput() { return output; }
	void clearOutput() { output.clear(); }

private:
	size_t input1;
	size_t input2;
	GateList output;
};

class CirPiGate: public CirGate {
public:
	CirPiGate(unsigned no = 0, unsigned id = 0): CirGate(no, id) {}
	~CirPiGate() {}

	void printGate() {}
	void addOutput(CirGate* g) { output.push_back(g); }
	GateList getOutput() { return output; }
	void clearOutput() { output.clear(); }

private:
	GateList output;
};

class CirPoGate: public CirGate {
public:
	CirPoGate(unsigned no = 0, unsigned id = 0): CirGate(no, id), input1(0) {}
	~CirPoGate() {}

	void printGate() {}
	bool isInv(int i = 1) const { return (input1 & NEG); }

	void setInput1(CirGate* g, bool inv) { input1 = (inv)? (size_t)g: (size_t)g + NEG; }
	CirGate* getInput1() const { return (CirGate*)(input1 & ~size_t(NEG)); }

private:
	size_t input1;
};


#endif // CIR_GATE_H
