#include <iostream>

class Data {
public:
	Data(int i);
	const int operator[] (size_t i) const;
	int &operator[] (size_t i);
	void print();
private:
	int *_cols;
};
