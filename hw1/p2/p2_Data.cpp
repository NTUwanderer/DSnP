#include "p2_Data.h"
#include <iomanip>
using namespace std;

Data::Data(int i){
	_cols = new int[i];
	for(int j = 0; j < i; j++)	_cols[j] = 101;
}
const int Data::operator[](size_t i) const{
	return _cols[i];
}
int &Data::operator[](size_t i){
	return _cols[i];
}
void Data::print(){
	int size = sizeof(_cols) / sizeof(int);
	for(int i = 0; i < size; i++){
		if(_cols[i] == 101)	cout<<right<<setw(4)<<"";
		else				cout<<right<<setw(4)<<_cols[i];
	}
}
