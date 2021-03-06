/****************************************************************************
  FileName     [ cmdReader.cpp ]
  PackageName  [ cmd ]
  Synopsis     [ Define command line reader member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2007-2014 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include <cassert>
#include <cstring>
#include "cmdParser.h"
using namespace std;

//----------------------------------------------------------------------
//    Extrenal funcitons
//----------------------------------------------------------------------
void mybeep();
char mygetc(istream&);
ParseChar getChar(istream&);


//----------------------------------------------------------------------
//    Member Function for class Parser
//----------------------------------------------------------------------
void
CmdParser::readCmd()
{
   if (_dofile.is_open()) {
      readCmdInt(_dofile);
      _dofile.close();
   }
   else
      readCmdInt(cin);
}

void
CmdParser::readCmdInt(istream& istr)
{
   resetBufAndPrintPrompt();

   while (1) {
      ParseChar pch = getChar(istr);
      if (pch == INPUT_END_KEY) break;
      switch (pch) {
         case LINE_BEGIN_KEY :
         case HOME_KEY       : moveBufPtr(_readBuf); break;
         case LINE_END_KEY   :
         case END_KEY        : moveBufPtr(_readBufEnd); break;
         case BACK_SPACE_KEY : if(moveBufPtr(_readBufPtr - 1)){
							      deleteChar();
							   } break; // TODO
         case DELETE_KEY     : deleteChar(); break; //
         case NEWLINE_KEY    : addHistory();
                               cout << char(NEWLINE_KEY);
                               resetBufAndPrintPrompt(); break;
         case ARROW_UP_KEY   : moveToHistory(_historyIdx - 1); break;
         case ARROW_DOWN_KEY : moveToHistory(_historyIdx + 1); break;
         case ARROW_RIGHT_KEY: moveBufPtr(_readBufPtr + 1); break;
         case ARROW_LEFT_KEY : moveBufPtr(_readBufPtr - 1); break;
         case PG_UP_KEY      : moveToHistory(_historyIdx - PG_OFFSET); break;
         case PG_DOWN_KEY    : moveToHistory(_historyIdx + PG_OFFSET); break;
         case TAB_KEY        : insertChar(' ', 8 - (int)(_readBufPtr - _readBuf) % 8);	break;
         case INSERT_KEY     : // not yet supported; fall through to UNDEFINE
         case UNDEFINED_KEY:   mybeep(); break;
         default:  // printable character
            insertChar(char(pch)); break;
      }
	  #ifdef TA_KB_SETTING
      taTestOnly();
      #endif
   }
}


// This function moves _readBufPtr to the "ptr" pointer
// It is used by left/right arrowkeys, home/end, etc.
//
// Suggested steps:
// 1. Make sure ptr is within [_readBuf, _readBufEnd].
//    If not, make a beep sound and return false. (DON'T MOVE)
// 2. Move the cursor to the left or right, depending on ptr
// 3. Update _readBufPtr accordingly. The content of the _readBuf[] will
//    not be changed
//
// [Note] This function can also be called by other member functions below
//        to move the _readBufPtr to proper position.
bool
CmdParser::moveBufPtr(char* const ptr)
{
   // TODO...
   if(ptr < _readBuf || ptr > _readBufEnd){
      mybeep();
	  return false;
   }
   // move the cursor not done!
   while(ptr < _readBufPtr){
	   _readBufPtr--;
  	   cout<<char(8);
   }
   while(ptr > _readBufPtr){
       cout<<*_readBufPtr;
	   _readBufPtr++;
   }
   //_readBufPtr = ptr; 
   return true;
}

// [Notes]
// 1. Delete the char at _readBufPtr
// 2. mybeep() and return false if at _readBufEnd
// 3. Move the remaining string left for one character
// 4. The cursor should stay at the same position
// 5. Remember to update _readBufEnd accordingly.
// 6. Don't leave the tailing character.
// 7. Call "moveBufPtr(...)" if needed.
// For example,
// cmd> This is the command
//              ^                (^ is the cursor position)
// After calling deleteChar()---
//
// cmd> This is he command
//              ^
bool
CmdParser::deleteChar()
{
   // TODO...
   if(_readBufPtr == _readBufEnd){
      mybeep();
	  return false;
   }
   int count = 0;
   while(*(_readBufPtr + count) != 0){
	  char c = *(_readBufPtr + count + 1);
      *(_readBufPtr + count) = c;
	  cout<<c;
	  if(c == 0)	break;
	  count++;
   }
   cout<<' ';
   *_readBufEnd = 0;
   _readBufEnd--;

   count++;
   while(count != 0){
      cout<<char(8);
	  count--;
   }
   return true;
}

// 1. Insert character 'ch' for "repeat" times at _readBufPtr
// 2. Move the remaining string right for "repeat" characters
// 3. The cursor should move right for "repeats" positions afterwards
// 4. Default value for "repeat" is 1. You should assert that (repeat >= 1).
//
// For example,
//
// cmd> This is the command
//              ^                (^ is the cursor position)
//
// After calling insertChar('k', 3) ---
//
// cmd> This is kkkthe command
//                 ^
//
void
CmdParser::insertChar(char ch, int repeat)
{
   // TODO...
   assert(repeat >= 1);
   for(char* temp = _readBufEnd; temp >= _readBufPtr; temp--){
      *(temp + repeat) = *temp;
   }
   for(int i = 0; i != repeat; i++){
      *(_readBufPtr) = ch;
	  _readBufPtr++;
	  cout<<ch;
   }
   int count = 0;
   char* temp = _readBufPtr;
   while(*temp != 0){
	  cout<<*temp;
	  temp++;
      count++;
   }
   while(count != 0){
      count--;
	  cout<<char(8);
   }
   _readBufEnd += repeat;
   // move the cursor not done!
}

// 1. Delete the line that is currently shown on the screen
// 2. Reset _readBufPtr and _readBufEnd to _readBuf
// 3. Make sure *_readBufEnd = 0
//
// For example,
//
// cmd> This is the command
//              ^                (^ is the cursor position)
//
// After calling deleteLine() ---
//
// cmd>
//      ^
//
void
CmdParser::deleteLine()
{
   // TODO...
   while(_readBufPtr != _readBuf){
      _readBufPtr--;
	  cout<<char(8);
   }
   int count = 0;
   while(_readBuf[count] != 0){
      cout << ' ';
	  count++;
   }
   while(count != 0){
      cout<<char(8);
   	  count--;
   }
   _readBufPtr = _readBufEnd = _readBuf;
   *_readBufEnd = 0;
   // cursor...?
}


// This functions moves _historyIdx to index and display _history[index]
// on the screen.
//
// Need to consider:
// If moving up... (i.e. index < _historyIdx)
// 1. If already at top (i.e. _historyIdx == 0), beep and do nothing.
// 2. If at bottom, temporarily record _readBuf to history.
//    (Do not remove spaces, and set _tempCmdStored to "true")
// 3. If index < 0, let index = 0.
//
// If moving down... (i.e. index > _historyIdx)
// 1. If already at bottom, beep and do nothing
// 2. If index >= _history.size(), let index = _history.size() - 1.
//
// Assign _historyIdx to index at the end.
//
// [Note] index should not = _historyIdx
//
void
CmdParser::moveToHistory(int index)
{
   // TODO...
   unsigned int historySize = _history.size();
   if(_tempCmdStored)	historySize--;
   if(index < _historyIdx){
      if(_historyIdx == 0){
	     mybeep();
		 return;
	  }
	  if(index < 0) index = 0;
	  if((unsigned)_historyIdx == historySize){
	  	 if(! _tempCmdStored){
			 string s(_readBuf);
			 _history.push_back(s);
			 _tempCmdStored = true;
	     }
		 else _history[_history.size() - 1].assign(_readBuf);
	  }
   }
   else if(index > _historyIdx){
      if((unsigned)_historyIdx == historySize){
	     mybeep();
		 return;
	  }
      if((unsigned)index > historySize)	index = historySize;
     
   }
   if(_historyIdx != index){
      _historyIdx = index;
	  if(_historyIdx >= 0 && (unsigned)_historyIdx < _history.size())
	     retrieveHistory();
	  if((unsigned)_historyIdx == historySize && _tempCmdStored){
	      _history.pop_back();
		  _tempCmdStored = false;
	  }
   }
   // Is moving the _readBuf necessary?
   
}


// This function adds the string in _readBuf to the _history.
// The size of _history may or may not change. Depending on whether 
// there is a temp history string.
//
// 1. Remove ' ' at the beginning and end of _readBuf
// 2. If not a null string, add string to _history.
//    Be sure you are adding to the right entry of _history.
// 3. If it is a null string, don't add anything to _history.
// 4. Make sure to clean up "temp recorded string" (added earlier by up/pgUp,
//    and reset _tempCmdStored to false
// 5. Reset _historyIdx to _history.size() // for future insertion
//
void
CmdParser::addHistory()
{
   // TODO...
   int count1 = 0, count2 = 0;
   while(_readBuf[count1] == ' ')	count1++;
   if(count1 != 0){
      int temp = 0;
	  while(true){
	     _readBuf[temp] = _readBuf[temp + count1];
		 if(_readBuf[temp + count1] == 0)	break;
		 temp++;
	  }
	  _readBufEnd -= count1;
   }
   while(*(_readBufEnd - 1 - count2) == ' ')	count2++;
   _readBufEnd -= count2;
   for(int i = 0; i <= count1 + count2; i++)
	   *(_readBufEnd + i) = 0;
   bool exist = (_readBuf != _readBufEnd);
   if(_tempCmdStored){
	   if(exist)
          _history[_history.size() - 1].assign(_readBuf);
	   else	_history.pop_back();
   }
   else{
	   if(exist){
	      string s(_readBuf);
   	      _history.push_back(s);
       }
   }
   /*
   cout<<endl;
   for(unsigned int i = 0; i < _history.size(); i++){
      cout << _history[i] << '\t' << _history[i].size() << endl;
   }
   */
   _tempCmdStored = false;
   _historyIdx = _history.size();
}


// 1. Replace current line with _history[_historyIdx] on the screen
// 2. Set _readBufPtr and _readBufEnd to end of line
//
// [Note] Do not change _history.size().
//
void
CmdParser::retrieveHistory()
{
   deleteLine();
   strcpy(_readBuf, _history[_historyIdx].c_str());
   cout << _readBuf;
   _readBufPtr = _readBufEnd = _readBuf + _history[_historyIdx].size();
}
