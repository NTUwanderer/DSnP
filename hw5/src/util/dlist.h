/****************************************************************************
  FileName     [ dlist.h ]
  PackageName  [ util ]
  Synopsis     [ Define doubly linked list package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef DLIST_H
#define DLIST_H

#include <cassert>
#include <algorithm>

template <class T> class DList;

// DListNode is supposed to be a private class. User don't need to see it.
// Only DList and DList::iterator can access it.
//
// DO NOT add any public data member or function to this class!!
//
template <class T>
class DListNode
{
   friend class DList<T>;
   friend class DList<T>::iterator;

   DListNode(const T& d, DListNode<T>* p = 0, DListNode<T>* n = 0):
      _data(d), _prev(p), _next(n) {}

   T              _data;
   DListNode<T>*  _prev;
   DListNode<T>*  _next;
};


template <class T>
class DList
{
public:
   DList() {
      _head = new DListNode<T>(T());
      _head->_prev = _head->_next = _head; // _head is a dummy node
   }
   ~DList() { clear(); delete _head; }

   // DO NOT add any more data member or function for class iterator
   class iterator
   {
      friend class DList;

   public:
      iterator(DListNode<T>* n= 0): _node(n) {}
      iterator(const iterator& i) : _node(i._node) {}
      ~iterator() {} // Should NOT delete _node

      // TODO: implement these overloaded operators
      const T& operator * () const { return _node->_data; }
      T& operator * () { return _node->_data; }
      iterator& operator ++ () { _node = _node->_next; return *(this); }
      iterator operator ++ (int) { iterator temp(_node); _node = _node->_next; return temp; }
      iterator& operator -- () { _node = _node->_prev; return *(this); }
      iterator operator -- (int) { iterator temp(_node); _node = _node->_prev; return temp; }

      iterator& operator = (const iterator& i) { _node = i._node; return *(this); }

      bool operator != (const iterator& i) const { return (_node != i._node); }
      bool operator == (const iterator& i) const { return (_node == i._node); }

   private:
      DListNode<T>* _node;
   	
	};

   // TODO: implement these functions
   iterator begin() const { return iterator(_head); }
   iterator end() const { return iterator(_head->_prev); }
   bool empty() const { return (_head == _head->_prev); }
   size_t size() const { 
		size_t count = 0;
		for (iterator i(begin()), dummyEnd(end()); i != dummyEnd; ++i)
			++ count;
		return count;
	}

   void push_back(const T& x) { 
		
		if (empty()) {
			DListNode<T>* temp = new DListNode<T>(x, _head, _head);
			_head->_prev = temp;	_head->_next = temp;
			_head = temp;
			return;
		}
		DListNode<T>* temp = new DListNode<T>(x, _head->_prev->_prev, _head->_prev);
		temp->_prev->_next = temp;	temp->_next->_prev = temp;
	}
   void pop_front() { erase(begin()); }
   void pop_back() { erase(iterator(--end())); }

   // return false if nothing to erase
   bool erase(iterator pos) { 
		if (pos == end() || pos._node == 0) return false;
		if (pos == begin())	_head = _head->_next;
		DListNode<T> *prev = pos._node->_prev, *next = pos._node->_next;
		prev->_next = next;	next->_prev = prev;
		delete pos._node;
		pos._node = 0;
		return true;
	}
   bool erase(const T& x) { 
		iterator i(begin()), dummyEnd(end());
		while (true) {
			if (i == dummyEnd)	return false;
			else if (*i == x)	break;
			++i;
		}
		return erase(i);
	}

   void clear() { 
		DListNode<T>* end = _head->_prev;
		while (_head != end) {
			DListNode<T>* temp = _head;
			_head = _head->_next;
			delete temp;
		}
		_head->_prev = _head;	_head->_next = _head;
   }  // delete all nodes except for the dummy node

   void sort() const {
		iterator begin = iterator(_head);
		for (iterator i = begin, end = iterator(_head->_prev); i != end; ++i) {
			T temp = *i;
			iterator j = i;
			while (true) {
				if (j == begin) {
					*j = temp;
					break;
				}
				iterator k = j;
				--j;
				if (temp < *j)	{
					*k = *j;
					continue;
				}
				*k = temp;
				break;
			}
		}
	}

private:
   DListNode<T>*  _head;  // = dummy node if list is empty

   // [OPTIONAL TODO] helper functions; called by public member functions
};

#endif // DLIST_H
