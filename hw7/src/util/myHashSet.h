/****************************************************************************
  FileName     [ myHashSet.h ]
  PackageName  [ util ]
  Synopsis     [ Define HashSet ADT ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2014-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef MY_HASH_SET_H
#define MY_HASH_SET_H

#include <vector>

using namespace std;

//---------------------
// Define HashSet class
//---------------------
// To use HashSet ADT,
// the class "Data" should at least overload the "()" and "==" operators.
//
// "operator ()" is to generate the hash key (size_t)
// that will be % by _numBuckets to get the bucket number.
// ==> See "bucketNum()"
//
// "operator ()" is to check whether there has already been
// an equivalent "Data" object in the HashSet.
// Note that HashSet does not allow equivalent nodes to be inserted
//
template <class Data>
class HashSet
{
public:
   HashSet(size_t b = 0) : _numBuckets(0), _buckets(0) { if (b != 0) init(b); }
   ~HashSet() { reset(); }

   // TODO: implement the HashSet<Data>::iterator
   // o An iterator should be able to go through all the valid Data
   //   in the Hash
   // o Functions to be implemented:
   //   - constructor(s), destructor
   //   - operator '*': return the HashNode
   //   - ++/--iterator, iterator++/--
   //   - operators '=', '==', !="
   //
   class iterator
   {
      friend class HashSet<Data>;

   public:
		iterator (typename std::vector<Data>::iterator const& i,
					 vector<Data>* const& b, const size_t& s)
			: _i(i), _b(b), _s(s) {}
		iterator (const iterator& i) : _i(i._i), _b(i._b), _s(i._s) {}
		const Data& operator *() const { return *_i; }
		Data& operator *() { return *_i; }
		iterator& operator ++ () {
			if (_i == _b[_s - 1].end())	return *this;
			size_t n = bucketNum(*_i);
			if (_i != --_b[n].end()) {
				++_i;
				return *this;
			}
			while (++n < _s) {
				if (!_b[n].empty()) {
					_i = _b[n].begin();
					return *this;
				}
			}
			_i = _b[_s - 1].end();
			return *this;
		}
		iterator operator ++ (int) {
			iterator temp = iterator(*this);
			++(*this);
			return temp;
		}

		iterator& operator -- () {
			if (_s == 0 || _i == _b[0].begin())	return *this;
			size_t n = bucketNum(*_i);
			if (_i != _b[n].begin()) {
				--_i;
				return *this;
			}
			--n;
			while (true) {
				if (!_b[n].empty()) {
					_i = _b[n].back();
					return *this;
				}
				if (n == 0)	{
					_i = _b[0].begin();
					return *this;
				}
				--n;
			}
		}
		iterator operator -- (int) {
			iterator temp = iterator(*this);
			--(*this);
			return temp;
		}
		iterator& operator = (const iterator& temp) {
			_i = temp._i;
			_b = temp._b;
			_s = temp._s;
		}
		bool operator == (const iterator& temp) {
			return (_i == temp._i && _b == temp._b && _s == temp._s);
		}
		bool operator != (const iterator& temp) {
			return !(*this == temp);
		}
   private:
		typename std::vector<Data>::iterator 	_i;
		typename std::vector<Data>* 				_b;
		size_t 								_s;
		
		size_t bucketNum(const Data& d) const {
      	return (d() % _s); }

   };

   void init(size_t b) { _numBuckets = b; _buckets = new vector<Data>[b]; }
   void reset() {
      _numBuckets = 0;
      if (_buckets) { delete [] _buckets; _buckets = 0; }
   }
   void clear() {
      for (size_t i = 0; i < _numBuckets; ++i) _buckets[i].clear();
   }
   size_t numBuckets() const { return _numBuckets; }

   vector<Data>& operator [] (size_t i) { return _buckets[i]; }
   const vector<Data>& operator [](size_t i) const { return _buckets[i]; }

   // TODO: implement these functions
   //
   // Point to the first valid data
   iterator begin() const {
		size_t i = 0;
		while (i < _numBuckets) {
			if (!_buckets[i].empty())
				return iterator(_buckets[i].begin(), _buckets, _numBuckets);
			++i;
		}
		return end();
	}
   // Pass the end
   iterator end() const {
		return iterator(_buckets[_numBuckets - 1].end(), _buckets, _numBuckets);
		/*
		size_t i = _numBuckets - 1;
		while (true) {
			if (!_buckets[i].empty())
				return iterator(&_buckets[i].back());
			if (i == 0)	return iterator();
			--i;
		}
		*/
	}
   // return true if no valid data
   bool empty() const {
		for (size_t i = 0; i < _numBuckets; ++i)
			if (!_buckets[i].empty())	return false;
		return true;
	}
   // number of valid data
   size_t size() const {
		size_t s = 0;
		for (size_t i = 0; i < _numBuckets; ++i)
			s += _buckets[i].size();
		return s;
	}

   // check if d is in the hash...
   // if yes, return true;
   // else return false;
   bool check(const Data& d) const {
		size_t n = bucketNum(d);
		for (size_t i = 0, size = _buckets[n].size(); i < size; ++i)
			if (_buckets[n][i] == d)	return true;
		return false;
	}

   // query if d is in the hash...
   // if yes, replace d with the data in the hash and return true;
   // else return false;
   bool query(Data& d) const {
		size_t n = bucketNum(d);
		for (size_t i = 0, size = _buckets[n].size(); i < size; ++i)
			if (_buckets[n][i] == d) {
				d = _buckets[n][i];
				return true;
			}
		return false;
	}

   // update the entry in hash that is equal to d
   // if found, update that entry with d and return true;
   // else insert d into hash as a new entry and return false;
   bool update(const Data& d) {
		size_t n = bucketNum(d);
		for (size_t i = 0, size = _buckets[n].size(); i < size; ++i)
			if (_buckets[n][i] == d) {
				_buckets[n][i] = d;
				return true;
			}
		return false;
	}

   // return true if inserted successfully (i.e. d is not in the hash)
   // return false is d is already in the hash ==> will not insert
   bool insert(const Data& d) {
		if (check(d))	return false;
		_buckets[bucketNum(d)].push_back(d);
		cout << "Task node inserted: " << d << endl;
		return true;
	}

   // return true if removed successfully (i.e. d is in the hash)
   // return fasle otherwise (i.e. nothing is removed)
   bool remove(const Data& d) {
		size_t n = bucketNum(d);
		for (size_t i = 0, size = _buckets[n].size(); i < size; ++i)
			if (_buckets[n][i] == d) {
				_buckets[n].erase(_buckets[n].begin() + i);
				return true;
			}
		return false;
	}

private:
   // Do not add any extra data member
   size_t            _numBuckets;
   vector<Data>*     _buckets;

   size_t bucketNum(const Data& d) const {
      return (d() % _numBuckets); }
};

#endif // MY_HASH_SET_H
