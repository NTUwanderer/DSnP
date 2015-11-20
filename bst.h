/****************************************************************************
  FileName     [ bst.h ]
  PackageName  [ util ]
  Synopsis     [ Define binary search tree package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef BST_H
#define BST_H

#include <cassert>

using namespace std;

template <class T> class BSTree;

// BSTreeNode is supposed to be a private class. User don't need to see it.
// Only BSTree and BSTree::iterator can access it.
//
// DO NOT add any public data member or function to this class!!
//
template <class T>
class BSTreeNode
{
   // TODO: design your own class!!
	friend class BSTree<T>;
	friend class BSTree<T>::iterator;

	BSTreeNode(const T& d, BSTreeNode<T>* l = 0, BSTreeNode<T>* r = 0):
		_data(d), _left(l), _right(r) {}

	T					_data;
	BSTreeNode<T>*	_left;
	BSTreeNode<T>*	_right;
};


template <class T>
class BSTree
{
public:
	BSTree(): _root(0), _size(0) { }
	~BSTree() { clear(); }
   // TODO: design your own class!!
   class iterator {
		friend class BSTree;
	public:
		iterator(BSTreeNode<T>* n = 0, BSTreeNode<T>* r = 0, vector<bool> t = new vector<bool>()): _node(n), root(r), _trace(t) {}
      iterator(const iterator& i) : _node(i._node), root(i.root), _trace(i._trace) {}
      ~iterator() {} // Should NOT delete _node

      // TODO: implement these overloaded operators
      const T& operator * () const { return _node->_data; }
      T& operator * () { return _node->_data; }
      iterator& operator ++ () {
			if (_node == 0) return *this;
			if (_node->_right != 0) {
				_node = _node->_right;
				_trace.push_back(true);
				while (_node->_left != 0) {
					_node = _node->_left;
					_trace.push_back(false);
				}
				return (*this);
			}
			
			while (true) {
				if (_trace.empty()) {
					*this = getEnd();
					return (*this);
				}
				bool prev = _trace.back(); _trace.pop_back();
				if (prev == false)	{
					_node = getNode(_trace);
					return (*this);
				}
			}
		}
      iterator operator ++ (int) { iterator temp = iterator(*this); ++(*this); return temp; }
      iterator& operator -- () {
			if (_node != 0) {
				if (_node->_left != 0) {
					_node = _node->_left;
					_trace.push_back(false);
					while (_node->_right != 0) {
						_node = _node->_right;
						_trace.push_back(true);
					}
					return (*this);
				}
			}
			iterator temp = iterator(*this);
			while (true) {
				if (_trace.empty()) {
					*this = temp;
					return *this;
				}
				bool prev = _trace.back(); _trace.pop_back();
				if (prev) {
					_node = getNode(_trace);
					return (*this);
				}
			}
		}
      iterator operator -- (int) { iterator temp = iterator(*this); ++(*this); return temp; }

      iterator& operator = (const iterator& i) { _node = i._node; _trace = i._trace; return *(this); }

      bool operator != (const iterator& i) const { return ! (*this == i); }
      bool operator == (const iterator& i) const {
			return (_node == i._node && root == i.root && _trace == i._trace);
		}

   private:
     	BSTreeNode<T>* _node;
		BSTreeNode<T>* root;
   	vector<bool> _trace; // 0: left, 1: right

		BSTreeNode<T>* getNode(vector<bool> trace) {
			BSTreeNode<T>* temp = root;
			for (size_t i = 0; i < trace.size(); ++i) {
				if (temp == 0)	break;
				if (trace[i] == false) temp = temp->_left;
				else if (trace[i] == true) temp = temp->_right;
			}
			return temp;
		}
		iterator getBegin() {
			vector<bool> tempTrace;
			BSTreeNode<T>* temp = root;
			if (temp->_left != 0) {
				temp = temp->_left;
				tempTrace.push_back(false);
			}
			return iterator(temp, root, tempTrace);
		}
		iterator getEnd() {
			vector<bool> tempTrace;
			BSTreeNode<T>* temp = root;
			if (temp != 0) {
				temp = temp->_right;
				tempTrace.push_back(true);
			}
			return iterator(temp, root, tempTrace);
		}
	};


	iterator begin() {
		if (_root == 0)	return iterator(_root, _root, vector<bool>());
		BSTreeNode<T>* temp = _root;
		vector<bool> trace;
		while (temp->_left != 0) {
			temp = temp->_left;
			trace.push_back(false);
		}
		
		return iterator(temp, _root, trace);
	}
	iterator end() { 
		BSTreeNode<T>* temp = _root;
		vector<bool> trace;
		while (temp != 0) {
			temp = temp->_right;
			trace.push_back(true);
		}

		return iterator(temp, _root, trace);
	}
	bool empty() const { return (_root == 0); }
	size_t size() const { return _size; }

	T& operator [] (size_t n) {
		iterator temp = begin();
		for (size_t i = 0; i < n; ++i)	++temp;
		return *temp;
	}
	const T& operator[] (size_t n) const {
		iterator temp = begin();
		for (size_t i = 0; i < n; ++i)	++temp;
		return *temp;
	}

	void insert(const T& x) {
		++_size;
		if (_root == 0) _root = new BSTreeNode<T>(x);
		BSTreeNode<T>* temp = _root;
		while (true) {
			if (x < temp->_data) {
				if (temp->_left != 0)	temp = temp->_left;
				else {
					temp->_left = new BSTreeNode<T>(x);
					break;
				}
			}
			else {
				if (temp->_right != 0)	temp = temp->_right;
				else {
					temp->_right = new BSTreeNode<T>(x);
					break;
				}
			}
		}
	}
	void pop_front() {
		if (empty())	return;
		--_size;
		if (_root->_left == 0) {
			if (_root->_right == 0) {
				delete _root;
				_root = 0;
				return;
			}
			BSTreeNode<T>* temp = _root;
			_root = _root->_right;
			delete temp;
			return;
		}
		BSTreeNode<T>* temp = _root;
		while (true) {
			if (temp->_left->_left == 0) {
				if (temp->_left->_right != 0) {
					BSTreeNode<T>* temp2 = temp->_left;
					temp->_left = temp->_left->_right;
					delete temp2;
				}
				else {
					delete temp->_left;
					temp->_left = 0;
				}
				break;
			}
		}
	}
	void pop_back() {
		if (empty())	return;
		--_size;
		if (_root->_right == 0) {
			if (_root->_left == 0) {
				delete _root;
				_root = 0;
				return;
			}
			BSTreeNode<T>* temp = _root;
			_root = _root->_right;
			delete temp;
			return;
		}
		BSTreeNode<T>* temp = _root;
		while (true) {
			if (temp->_right->_right == 0) {
				if (temp->_right->_left != 0) {
					BSTreeNode<T>* temp2 = temp->_right;
					temp->_right = temp->_right->_left;
					delete temp2;
				}
				else {
					delete temp->_right;
					temp->_right = 0;
				}
				break;
			}
		}
	}

	bool erase(iterator pos) {
		if (pos._node == 0)	return false;
		--_size;
		if (pos._node->_left == 0 && pos._node->_right == 0) {
			if (pos._node == _root) {
				delete _root;
				_root = 0;
				return true;
			}
			bool left = pos._trace.back();	pos._trace.pop_back();
			BSTreeNode<T>* prev = pos.getNode(pos._trace);
			delete pos._node;
			if (left)	prev->_left = 0;
			else			prev->_right = 0;

			return true;
		}
		else if (pos._node->_left == 0) {
			BSTreeNode<T>* prev = pos._node, *temp = pos._node->_right;
			while (temp->_left != 0) {
				prev = temp;
				temp = temp->_left;
			}
			*pos = temp->_data;
			if (temp->_right == 0) {
				if (prev->_left == temp) {
					delete prev->_left;
					prev->_left = 0;
				}
				else {
					delete prev->_right;
					prev->_right = 0;
				}
			}
			else {
				if (prev->_left == temp) {
					prev->_left = temp->_right;
					delete temp;
				}
				else {
					prev->_right = temp->_right;
					delete temp;
				}
			}
		}
		else {
			BSTreeNode<T>* prev = pos._node, *temp = pos._node->_left;
			while (temp->_right != 0) {
				prev = temp;
				temp = temp->_right;
			}
			*pos = temp->_data;
			if (temp->_left == 0) {
				if (prev->_left == temp) {
					delete prev->_left;
					prev->_left = 0;
				}
				else {
					delete prev->_right;
					prev->_right = 0;
				}
			}
			else {
				if (prev->_left == temp) {
					prev->_left = temp->_left;
					delete temp;
				}
				else {
					prev->_right = temp->_left;
					delete temp;
				}
			}
		}
		return true;
	}
	bool erase(const T& x) {
		BSTreeNode<T>* temp = _root;
		vector<bool> tempTrace;
		while (true) {
			if (temp == 0)	return false;
			if (temp->_data == x)	return erase(iterator(temp, _root, tempTrace));
			if (temp->_data < x) {
				temp = temp->_right;
				tempTrace.push_back(true);
			}
			else {
				temp = temp->_left;
				tempTrace.push_back(false);
			}
		}
	}

	void clear() {
		if (_root != 0)	deleteAll(_root);
		_size = 0;
		_root = 0;
	}
	void print() { }

	void sort() const {}
private:
	BSTreeNode<T>* _root;
	size_t _size;

	void deleteAll(BSTreeNode<T>* n) {
		if (n->_left != 0)	deleteAll(n->_left);
		if (n->_right != 0)	deleteAll(n->_right);
		delete n;
	}
};

#endif // BST_H
