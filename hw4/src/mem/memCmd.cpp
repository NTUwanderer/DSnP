/****************************************************************************
  FileName     [ memCmd.cpp ]
  PackageName  [ mem ]
  Synopsis     [ Define memory test commands ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2007-present LaDs(III), GIEE, NTU, Taiwan ]
 ****************************************************************************/
#include <iostream>
#include <iomanip>
#include <string>
#include "memCmd.h"
#include "memTest.h"
#include "cmdParser.h"
#include "util.h"

using namespace std;

extern MemTest mtest;  // defined in memTest.cpp

bool initMemCmd()
{
	if (!(cmdMgr->regCmd("MTReset", 3, new MTResetCmd) &&
				cmdMgr->regCmd("MTNew", 3, new MTNewCmd) &&
				cmdMgr->regCmd("MTDelete", 3, new MTDeleteCmd) &&
				cmdMgr->regCmd("MTPrint", 3, new MTPrintCmd)
		 )) {
		cerr << "Registering \"mem\" commands fails... exiting" << endl;
		return false;
	}
	return true;
}


//----------------------------------------------------------------------
//    MTReset [(size_t blockSize)]
//----------------------------------------------------------------------
CmdExecStatus MTResetCmd::exec(const string& option)
{
	// check option
	string token;
	if (!CmdExec::lexSingleOption(option, token))
		return CMD_EXEC_ERROR;
	if (token.size()) {
		int b;
		if (!myStr2Int(token, b) || b < int(toSizeT(sizeof(MemTestObj)))) {
			cerr << "Illegal block size (" << token << ")!!" << endl;
			return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
		}
#ifdef MEM_MGR_H
		mtest.reset(toSizeT(b));
#else
		mtest.reset();
#endif // MEM_MGR_H
	}
	else
		mtest.reset();
	return CMD_EXEC_DONE;
}

void MTResetCmd::usage(ostream& os) const
{  
	os << "Usage: MTReset [(size_t blockSize)]" << endl;
}

void MTResetCmd::help() const
{  
	cout << setw(15) << left << "MTReset: " 
		<< "(memory test) reset memory manager" << endl;
}


//----------------------------------------------------------------------
//    MTNew <(size_t numObjects)> [-Array (size_t arraySize)]
//----------------------------------------------------------------------
CmdExecStatus MTNewCmd::exec(const string& option)
{
	// TODO
	vector<string> toks;
	if (!CmdExec::lexOptions(option, toks))
		return CMD_EXEC_ERROR;

	if (toks.empty())
		return CmdExec::errorOption(CMD_OPT_MISSING, "");

	size_t size = toks.size(), numObjects = 0, arraySize = 0;
	for (size_t i = 0; i < size; ++i) {
		int temp = 0;
		if (myStrNCmp("-Array", toks[i], 2) == 0) {
			if (arraySize)	return CmdExec::errorOption(CMD_OPT_ILLEGAL, toks[i]);
			if (++i >= size) // ++i
				return CmdExec::errorOption(CMD_OPT_MISSING, toks[i - 1]);
			if (!myStr2Int(toks[i], temp)) 
				return CmdExec::errorOption(CMD_OPT_ILLEGAL, toks[i]);
			if(temp <= 0)	return CmdExec::errorOption(CMD_OPT_ILLEGAL, toks[i]);
			arraySize = static_cast<size_t>(temp);
			continue;
		} else if (numObjects)
			return CmdExec::errorOption(CMD_OPT_EXTRA, toks[i]);
		else if (!myStr2Int(toks[i], temp))
			return CmdExec::errorOption(CMD_OPT_ILLEGAL, toks[i]);
		if (temp <= 0)	return CmdExec::errorOption(CMD_OPT_ILLEGAL, toks[i]);
		numObjects = static_cast<size_t>(temp);
	}
	if (numObjects == 0)	CmdExec::errorOption(CMD_OPT_MISSING, toks[size - 1]);
	if (arraySize)	mtest.newArrs(numObjects, arraySize);
	else			mtest.newObjs(numObjects);

	return CMD_EXEC_DONE;
}

void MTNewCmd::usage(ostream& os) const
{  
	os << "Usage: MTNew <(size_t numObjects)> [-Array (size_t arraySize)]\n";
}

void MTNewCmd::help() const
{  
	cout << setw(15) << left << "MTNew: " 
		<< "(memory test) new objects" << endl;
}


//----------------------------------------------------------------------
//    MTDelete <-Index (size_t objId) | -Random (size_t numRandId)> [-Array]
//----------------------------------------------------------------------
CmdExecStatus MTDeleteCmd::exec(const string& option)
{
	// TODO
	vector<string> toks;
	if (!CmdExec::lexOptions(option, toks))
		return CMD_EXEC_ERROR;

	if (toks.empty())
		return CmdExec::errorOption(CMD_OPT_MISSING, "");

	size_t size = toks.size(), index = 0, n; //index: 1, random: 2
	string s, r;
	bool array = false;
	for (size_t i = 0; i < size; ++i) {
		int temp = 0;
		if (myStrNCmp("-Array", toks[i], 2) == 0) {
			if (array)	return CmdExec::errorOption(CMD_OPT_EXTRA, toks[i]);
			array = true;
		} else if (myStrNCmp("-Index", toks[i], 2) == 0) {
			if (index)	return CmdExec::errorOption(CMD_OPT_EXTRA, toks[i]);
			index = 1;
			if (++i >= size) // ++i
				return CmdExec::errorOption(CMD_OPT_MISSING, toks[i - 1]);
			if (!myStr2Int(toks[i], temp))
				return CmdExec::errorOption(CMD_OPT_ILLEGAL, toks[i]);
			if (temp < 0)	return CmdExec::errorOption(CMD_OPT_ILLEGAL, toks[i]);
			n = temp;
			s = toks[i];
		} else if (myStrNCmp("-Random", toks[i], 2) == 0) {
			r = toks[i];
			if (index)	return CmdExec::errorOption(CMD_OPT_EXTRA, toks[i]);
			index = 2;
			if (++i >= size) // ++i
				return CmdExec::errorOption(CMD_OPT_MISSING, toks[i - 1]);
			if (!myStr2Int(toks[i], temp))
				return CmdExec::errorOption(CMD_OPT_ILLEGAL, toks[i]);
			if (temp <= 0)	return CmdExec::errorOption(CMD_OPT_ILLEGAL, toks[i]);
			n = temp;
			s = toks[i];
		} else	return CmdExec::errorOption(CMD_OPT_ILLEGAL, toks[i]);
	}
	if (index == 1) {
		if (array) {
			if (n >= mtest.getArrListSize()) {
				cerr << "Size of array list (" << mtest.getArrListSize()
					<< ") is <= " << n << "!!\n";
				return CmdExec::errorOption(CMD_OPT_ILLEGAL, s);
			}
			mtest.deleteArr(n);
		} else {
			if (n >= mtest.getObjListSize()) {
				cerr << "Size of object list (" << mtest.getObjListSize()
					<< ") is <= " << n << "!!\n";
				return CmdExec::errorOption(CMD_OPT_ILLEGAL, s);
			}
			mtest.deleteObj(n);
		}
	} else if (index == 2) {
		if (array) {
			size_t aSize = mtest.getArrListSize();
			if (aSize == 0) {
				cout << "Size of array list is 0!!\n";
				return CmdExec::errorOption(CMD_OPT_ILLEGAL, r);
			}
			for (size_t i = 0; i < n; ++i)
				mtest.deleteArr(rnGen(aSize));
		} else {
			size_t oSize = mtest.getObjListSize();
			if (oSize == 0) {
				cout << "Size of object list is 0!!\n";
				return CmdExec::errorOption(CMD_OPT_ILLEGAL, r);
			}
			for (size_t i = 0; i < n; ++i)
				mtest.deleteObj(rnGen(oSize));
		}
	} else	return CmdExec::errorOption(CMD_OPT_MISSING, "");
	return CMD_EXEC_DONE;
}

void MTDeleteCmd::usage(ostream& os) const
{  
	os << "Usage: MTDelete <-Index (size_t objId) | "
		<< "-Random (size_t numRandId)> [-Array]" << endl;
}

void MTDeleteCmd::help() const
{  
	cout << setw(15) << left << "MTDelete: " 
		<< "(memory test) delete objects" << endl;
}


//----------------------------------------------------------------------
//    MTPrint
//----------------------------------------------------------------------
CmdExecStatus MTPrintCmd::exec(const string& option)
{
	// check option
	if (option.size())
		return CmdExec::errorOption(CMD_OPT_EXTRA, option);
	mtest.print();

	return CMD_EXEC_DONE;
}

void MTPrintCmd::usage(ostream& os) const
{  
	os << "Usage: MTPrint" << endl;
}

void MTPrintCmd::help() const
{  
	cout << setw(15) << left << "MTPrint: " 
		<< "(memory test) print memory manager info" << endl;
}


