/***************************************************************************
  FileName     [ dbTable.cpp ]
  PackageName  [ db ]
  Synopsis     [ Define database Table member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2015-present LaDs(III), GIEE, NTU, Taiwan ]
 ****************************************************************************/

#include <iomanip>
#include <string>
#include <sstream>
#include <cctype>
#include <cassert>
#include <set>
#include <algorithm>
#include "dbTable.h"

using namespace std;

/*****************************************/
/*          Global Functions             */
/*****************************************/
ostream& operator << (ostream& os, const DBRow& r)
{
	// TODO: to print out a row.
	// - Data are seperated by a space. No trailing space at the end.
	// - Null cells are printed as '.'
	for(int i = 0, size = r.size(); i < size; i++){
		DBTable::printData(os, r[i]);
		if(i != size - 1)	os << ' ';
	}
	os << endl;

	return os;
}

ostream& operator << (ostream& os, const DBTable& t)
{
	// TODO: to print out a table
	// - Data are seperated by setw(6) and aligned right.
	// - Null cells should be left blank (printed as ' ').
	size_t r = t.nRows(), c = t.nCols();
	for(size_t i = 0; i < r; i++){
		for(size_t j = 0; j < c; j++){
			if(t[i][j] == INT_MAX)	os << right << setw(6) << '.';
			else 					os << right << setw(6) << t[i][j];
		}
		os << endl;
	}
	return os;
}

ifstream& operator >> (ifstream& ifs, DBTable& t)
{
	// TODO: to read in data from csv file and store them in a table
	// - You can assume all the data of the table are in a single line.
	string line = "";
    int counter1 = 0, counter2 = 0;
	char c;
	DBRow row = DBRow();
	while(ifs.get(c)){
		if(c == ','){
			if(line != ""){
				stringstream ss(line);
				int number;
				ss >> number;
				row.addData(number);
			}else{
				row.addData(INT_MAX);
			}
			line = "";
			counter2 ++;
		}else if(c == 10 || c == 13){
			if(line != ""){
				stringstream ss(line);
				int number;
				ss >> number;
				row.addData(number);
				t.addRow(row);
				row.reset();
			}else{
				row.addData(INT_MAX);
				if(row.size() == t.nCols())	t.addRow(row);
				row.reset();
			}
			line = "";
			counter1 ++;
			counter2 = 0;
		}else line += c;
	}
	ifs.clear();
	return ifs;
}

/*****************************************/
/*   Member Functions for class DBRow    */
/*****************************************/
	void
DBRow::removeCell(size_t c)
{
	// TODO
	_data.erase(_data.begin() + c);
}

/*****************************************/
/*   Member Functions for struct DBSort  */
/*****************************************/
bool
DBSort::operator() (const DBRow& r1, const DBRow& r2) const
{
	// TODO: called as a functional object that compares the data in r1 and r2
	// based on the order defined in _sortOrder
	for(unsigned i = 0; i < _sortOrder.size(); i++){
		int n = _sortOrder[i];
		if(r1[n] > r2[n])	return false;
		if(r1[n] < r2[n])	return true;
	}
	return false;
}

/*****************************************/
/*   Member Functions for class DBTable  */
/*****************************************/
void
DBTable::reset()
{
	// TODO
	vector<DBRow> temp;
	temp.swap(_table);
}

void
DBTable::addCol(const vector<int>& d)
{
	// TODO: add a column to the right of the table. Data are in 'd'.
	for(size_t i = 0; i < d.size(); i++)	_table[i].addData(d[i]);
}

void
DBTable::delRow(int c)
{
	// TODO: delete row #c. Note #0 is the first row.
	_table.erase(_table.begin() + c);	
}

	void
DBTable::delCol(int c)
{
	// delete col #c. Note #0 is the first row.
	for (size_t i = 0, n = _table.size(); i < n; ++i)
		_table[i].removeCell(c);
}

// For the following getXXX() functions...  (except for getCount())
// - Ignore null cells
// - If all the cells in column #c are null, return NAN
// - Return "float" because NAN is a float.
float
DBTable::getMax(size_t c) const
{
	// TODO: get the max data in column #c
	int temp = 0;
	bool flag = false;
	for(int i = 0, n = nRows(); i < n; i++){
		int number = _table[i][c];
		if((!flag && number != INT_MAX) || (number > temp && number != INT_MAX)){
			temp = number;
			flag = true;
		}
	}
	if(flag)	return (float)temp;
	return NAN;
}

float
DBTable::getMin(size_t c) const
{
	// TODO: get the min data in column #c
	int temp = 0;
	bool flag = false;
	for(int i = 0, n = nRows(); i < n; i++){
		int number = _table[i][c];
		if((!flag && number != INT_MAX) || (number < temp && number != INT_MAX)){
			temp = number;
			flag = true;
		}
	}
	if(flag)	return (float)temp;
	return NAN;
}

float 
DBTable::getSum(size_t c) const
{
	// TODO: compute the sum of data in column #c
	int sum = 0;
	for(int i = 0, n = nRows(); i < n; i++){
		int number = _table[i][c];
		if(number != INT_MAX)	sum += number;
	}
	return sum;
}

int
DBTable::getCount(size_t c) const
{
	// TODO: compute the number of distinct data in column #c
	// - Ignore null cellsi
	set<int> sets;
	for(int i = 0, n = nRows(); i < n; i++){
		if(_table[i][c] != INT_MAX){
			sets.insert(_table[i][c]);
		}
	}	
	return sets.size();
}

float
DBTable::getAve(size_t c) const
{
	// TODO: compute the average of data in column #c
	int sum = 0, count = 0;
	for(int i = 0, n = nRows(); i < n; i++){
		int number = _table[i][c];
		if(number != INT_MAX){
			sum += number;
			count ++;
		}
	}
	if (count == 0)	return NAN;
	return 1.0 * sum / count;
}

void
DBTable::sort(const struct DBSort& s)
{
	// TODO: sort the data according to the order of columns in 's'
	std::sort(_table.begin(), _table.end(), s);
}

void
DBTable::printCol(size_t c) const
{
	// TODO: to print out a column.
	// - Data are seperated by a space. No trailing space at the end.
	// - Null cells are printed as '.'
	for(int i = 0, size = nRows(); i < size; i++){
		printData(cout, _table[i][c]);
		if(i != size - 1)	cout<< ' ';
	}
	cout << endl;
}

void
DBTable::printSummary() const
{
	size_t nr = nRows(), nc = nCols(), nv = 0;
	for (size_t i = 0; i < nr; ++i)
		for (size_t j = 0; j < nc; ++j)
			if (_table[i][j] != INT_MAX) ++nv;
	cout << "(#rows, #cols, #data) = (" << nr << ", " << nc << ", "
		<< nv << ")" << endl;
}

