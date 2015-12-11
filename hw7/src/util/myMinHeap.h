/****************************************************************************
  FileName     [ myMinHeap.h ]
  PackageName  [ util ]
  Synopsis     [ Define MinHeap ADT ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2014-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef MY_MIN_HEAP_H
#define MY_MIN_HEAP_H

#include <algorithm>
#include <vector>

template <class Data>
class MinHeap
{
public:
   MinHeap(size_t s = 0) { if (s != 0) _data.reserve(s); }
   ~MinHeap() {}

   void clear() { _data.clear(); }

   // For the following member functions,
   // We don't respond for the case vector "_data" is empty!
   const Data& operator [] (size_t i) const { return _data[i]; }   
   Data& operator [] (size_t i) { return _data[i]; }

   size_t size() const { return _data.size(); }

   // TODO
   const Data& min() const {
		if (! _data.empty())	return _data[0];
		return Data();
	}
   void insert(const Data& d) {
		_data.push_back(d);
		size_t i = size();
		while (i != 0) {
			size_t up = (i + 1) / 2 - 1;
			if (d < _data[up]) {
				_data[i] = _data[up];
				i = up;
			}
			else {
				_data[i] = d;
				break;
			}
		}
	}
   void delMin() { delData(0); }
   void delData(size_t i) {
		size_t size = _data.size();
		if (i >= size)	return;
		Data temp = _data.back();
		_data.pop_back();

		while (true) {
			size_t n = 2 * i + 1;
			if (n >= size)	break;
			if (n + 1 < size && _data[n + 1] < _data[n])	n += 1;
			_data[i] = _data[n];
			i = n;
		}
		_data[i] = temp;
	}

private:
   // DO NOT add or change data members
   vector<Data>   _data;
};

#endif // MY_MIN_HEAP_H
