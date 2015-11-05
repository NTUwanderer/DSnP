#include "p2_Data.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

struct SortData{
	bool operator() (const Data& d1, Data& d2){
		int counter = 0;
		for(vector<size_t>::iterator it = _sortOrder.begin(); it != _sortOrder.end(); it ++){
			if(d1[*it] > d2[*it])	return false;
			if(d1[*it] < d2[*it])	return true;
			counter ++;
		}
		return false;
	}
	void pushOrder(size_t i){
		_sortOrder.push_back(i);
	}
	void clear(){
		_sortOrder.clear();
	}
	vector<size_t> _sortOrder;
} StrictWeakOrdering;
void printAll(vector<Data> table, int col){
	for(vector<Data>::iterator it = table.begin(); it != table.end(); it ++){
		for(int i = 0; i < col; i ++){
			if((*it)[i] == 101)	cout << right << setw(4) << "";
			else				cout << right << setw(4) << (*it)[i];
		}cout << "\n";
	}
}
int main(){
	vector<Data> table;
    string fileName;
    int row, col;

    cout<<"Please enter the file name:";
	cin>>fileName;
	cout<<"Please enter the number of rows and columes:";
	cin>>row>>col;
	
    for(int i = 0; i < row; i++)    table.push_back(Data(col));
	
	ifstream myfile(fileName.c_str(), ifstream::in);
	
	if(myfile.is_open()){
		cout<<"File \"" + fileName + "\" was read in successfully\n";
        string line;
        int counter1 = 0, counter2 = 0;
		char c;
		while(myfile.get(c)){
			if(c == ','){
				if(line != ""){
					stringstream ss(line);
					int number;
					ss >> number;
					if(counter1 < row && counter2 < col)	table[counter1][counter2] = number;
				}
				line = "";
				counter2 ++;
			}else if(c == 10 || c == 13){
				if(line != ""){
					stringstream ss(line);
					int number;
					ss >> number;
					if(counter1 < row && counter2 < col)	table[counter1][counter2] = number;
				}
				line = "";
				counter1 ++;
				counter2 = 0;
			}else line += c;
		}
		if(line != ""){
			stringstream ss(line);
			int number;
			ss >> number;
			if(counter1 < row && counter2 < col)	table[counter1][counter2] = number;
		}
	}
	else	cout<<"Unable to open file.\n";
	myfile.close();
	string command;
	while(cin >> command){
		if(command == "PRINT"){
			printAll(table, col);
		}else if(command == "ADD"){
			row++;
			table.push_back(Data(col));
			string input;
			for(int i = 0; i < col; i ++){
				cin>>input;
				if(input != "-"){
					stringstream ss(input);
					int number;
					ss >> number;
					table[row-1][i] = number;
				}
			}
		}else if(command == "SUM"){
			int ll, sum = 0;
			cin >> ll;
			for(int i = 0; i < row; i ++){
				int temp = table[i][ll];
				if(temp != 101)	sum += temp;
			}
			cout << "The summation of data in column #" << ll << " is " << sum << ".\n";
		}else if(command == "AVE"){
			int ll, count = 0, sum = 0;
			cin >> ll;
			for(int i = 0; i < row; i ++){
				int temp = table[i][ll];
				if(temp != 101){
					sum += temp;
					count++;
				}
			}
			double ave = 1.0 * sum / count;
			cout << "The average of data in column #" << ll << " is " << ave << ".\n";
		}else if(command == "MAX"){
			int ll, max = -100;
			cin >> ll;
			for(int i = 0; i < row; i ++){
				int temp = table[i][ll];
				if(temp > max && temp != 101)	max = temp;
			}
			cout << "The maximum of data in column #" << ll << " is " << max << ".\n";
		}else if(command == "MIN"){
			int ll, min = 101;
			cin >> ll;
			for(int i = 0; i < row; i ++){
				int temp = table[i][ll];
				if(temp < min)	min = temp;
			}
			cout << "The minimum of data in column #" << ll << " is " << min << ".\n";
		}else if(command == "COUNT"){
			int ll, count = 0;
			vector<int> all;
			cin >> ll;
			for(int i = 0; i < table.size(); i ++){
				int temp = table[i][ll];
				if(temp != 101){
					bool flag = true;
					for(int j = 0; j < all.size(); j ++){
						if(temp == all[j]){
							flag = false;
							break;
						}
					}
					if(flag){
						count++;
						all.push_back(temp);
					}
				}
			}
			cout << "The distinct count of data in column #" << ll << " is " << count << ".\n";
		}else if(command == "SORT"){
			string line;
			getline(cin, line);
			int number = 0;
			bool stored = false;
			for(int i = 0; i < line.length(); i ++){
				if(line[i] == ' '){
					if(stored)	StrictWeakOrdering.pushOrder(number);
					number = 0;
					stored = false;
				}
				else{
					number = 10 * number + line[i] - 48;
					stored = true;
				}
			}
			if(stored)	StrictWeakOrdering.pushOrder(number);
			sort(table.begin(), table.end(), StrictWeakOrdering);
			StrictWeakOrdering.clear();
		}
	}	

	return 0;
 
}
