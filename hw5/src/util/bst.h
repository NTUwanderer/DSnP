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

	BSTreeNode(const T& d, BSTreeNode<T>* p = 0, BSTreeNode<T>* l = 0, BSTreeNode<T>* r = 0, size_t s = 1):
		_data(d), _parent(p), _left(l), _right(r), _nodes(s) {}

	T 					_data;
	BSTreeNode<T>*	_parent;
	BSTreeNode<T>*	_left;
	BSTreeNode<T>*	_right;
	size_t			_nodes;
};


template <class T>
class BSTree
{
public:
	BSTree(): _root(0), _size(0) { _dummy = new BSTreeNode<T>(T()); }
	~BSTree() { clear(); delete _dummy; }
   // TODO: design your own class!!
   class iterator {
		friend class BSTree;
	public:
		iterator(BSTreeNode<T>* n = 0): _node(n) {}
      iterator(const iterator& i) : _node(i._node) {}
      ~iterator() {} // Should NOT delete _node

      // TODO: implement these overloaded operators
      const T& operator * () const { return _node->_data; }
      T& operator * () { return _node->_data; }
      iterator& operator ++ () {
			if (_node == 0) return *this;
			if (_node->_right != 0) {
				_node = _node->_right;
				while (_node->_left != 0)
					_node = _node->_left;
				
				return (*this);
			}
			iterator temp = iterator(*this);
			while (true) {
				if (this->_node->_parent == 0) {
					*this = temp;
					return (*this);
				}
				if (checkParent(_node))	{
					this->_node = this->_node->_parent;
					return (*this);
				}
				this->_node = this->_node->_parent;
			}
		}
      iterator operator ++ (int) { 
			iterator temp = iterator(*this);
			++(*this);
			return temp;
		}
      iterator& operator -- () {
			if (_node == 0) return *this;
			if (_node->_left != 0) {
				_node = _node->_left;
				while (_node->_right != 0)
					_node = _node->_right;
				
				return (*this);
			}
			iterator temp = iterator(*this);
			while (true) {
				if (this->_node->_parent == 0) {
					*this = temp;
					return (*this);
				}
				if (checkParent(_node) == false) {
					this->_node = this->_node->_parent;
					return (*this);
				}
				this->_node = this->_node->_parent;
			}
		}
      iterator operator -- (int) { 
			iterator temp = iterator(*this);
			--(*this);
			return temp;
		}
		
		iterator& operator + (int i) const {}
		iterator& operator += (int i) {}

      iterator& operator = (const iterator& i) { _node = i._node; return *this; }

      bool operator != (const iterator& i) const { return ! (*this == i); }
      bool operator == (const iterator& i) const {
			return (_node == i._node);
		}

   private:
     	BSTreeNode<T>* _node;
	};


	iterator begin() {
		if (_root == 0)	return iterator(_dummy);
		BSTreeNode<T>* temp = _root;
		while (temp->_left != 0)
			temp = temp->_left;
		
		return iterator(temp);
	}
	iterator end() { return iterator(_dummy); }
	bool empty() const { return (_root == 0); }
	size_t size() const { return _size; }

	T& operator [] (size_t n) {
		iterator temp = begin();
		for (size_t i = 0; i < n; ++i)	++temp;
		return *temp;
	}
	const T& operator [] (size_t n) const {
		iterator temp = begin();
		for (size_t i = 0; i < n; ++i)	++temp;
		return *temp;
	}

	void insert(const T& x) {
		++_size;
		if (_root == 0) {
			_root = new BSTreeNode<T>(x);
			createLink(_root, _dummy);
			return;
		}
		BSTreeNode<T>* temp = _root;
		while (true) {
			if (x < temp->_data) {
				if (temp->_left != 0)	temp = temp->_left;
				else {
					createLink(temp, new BSTreeNode<T>(x), true);
					return;
				}
			}
			else if (x == temp->_data) {
				BSTreeNode<T>* t = new BSTreeNode<T>(x);
				if (temp->_left != 0)	createLink(t, temp->_left, true);
				createLink(temp, t, true);
				return;
			}
			else {
				if (temp->_right == 0) {
					createLink(temp, new BSTreeNode<T>(x));
					return;
				}
				else if (temp->_right == _dummy) {
					createLink(temp, new BSTreeNode<T>(x));
					createLink(temp->_right, _dummy);
					return;
				}
				else	temp = temp->_right;
			}
		}
	}
	
	void pop_front() { erase(begin()); }
	void pop_back() {	erase(--(end()));	}

	bool erase(iterator pos) {
		if (pos._node == 0 || pos._node == _dummy)	return false;
		--_size;
		if (pos._node->_left == 0 && pos._node->_right == 0) { // cannot be _root
			if (checkParent(pos._node))	pos._node->_parent->_left = 0;
			else									pos._node->_parent->_right = 0;
			delete pos._node;
			pos._node = 0;
			return true;
		}
		else if (pos._node->_left == 0) {
			if (pos._node == _root) {
				if (_root->_right == _dummy) {
					delete _root;
					_root = 0;
					_dummy->_parent = 0;
					pos._node = 0;
					return true;
				}
				_root = _root->_right;
				_root->_parent = 0;
				delete pos._node;
				pos._node = 0;
				return true;
			}
			if (checkParent(pos._node))
				createLink(pos._node->_parent, pos._node->_right, true);
			else
				createLink(pos._node->_parent, pos._node->_right);
			delete pos._node;
			pos._node = 0;
			return true;
		}
		else if (pos._node->_right == 0) {
			if (checkParent(pos._node))
				createLink(pos._node->_parent, pos._node->_left, true);
			else
				createLink(pos._node->_parent, pos._node->_left);
			delete pos._node;
			pos._node = 0;
			return true;
		}
		else {
			iterator temp = pos--;
			*temp = *pos;
			++_size;
			return erase(pos);
		}
		return true;
	}
	bool erase(const T& x) {
		BSTreeNode<T>* temp = _root;
		while (true) {
			if (temp == 0)	return false;
			if (temp->_data == x)	return erase(iterator(temp));
			if (temp->_data < x)	temp = temp->_right;
			else temp = temp->_left;
		}
	}

	void clear() {
		if (_root != 0)	deleteAll(_root);
		_size = 0;
		_root = 0;
		_dummy->_parent = 0;
	}
	void print() { 
		printData(_root, 0);
	}

	void sort() const {}

	//helper function
	static bool checkParent(BSTreeNode<T>* n) {
		if (n->_parent->_left == n)	return true;
		return false;
	}
private:
	BSTreeNode<T>* _root;
	BSTreeNode<T>* _dummy;
	size_t _size;

	void deleteAll(BSTreeNode<T>* n) {
		if (n->_left != 0)	deleteAll(n->_left);
		if (n->_right != 0 && n->_right != _dummy)	deleteAll(n->_right);
		delete n;
	}

	void createLink(BSTreeNode<T>* p, BSTreeNode<T>* c, bool left = false) {
		c->_parent = p;
		if (left)	p->_left = c;
		else		p->_right = c;
	}
	void printData(BSTreeNode<T>* n, size_t l) {
		for (size_t i = 0; i < l; ++i)	cout << "   ";
		if (n == 0 || n == _dummy)	cout << "[0]\n";
		else {
			cout << n->_data << endl;
			printData(n->_left, l + 1);
			printData(n->_right, l + 1);
		}
	}
};

#endif // BST_H
