#include "KeyNode.h"

using namespace std;

KeyNode::KeyNode(const size_t& id1, const size_t& id2) {
	num = getKeyValue(id1, id2);
}

bool KeyNode::operator == (const KeyNode& node) const {
	return (num == node.num);
}

bool KeyNode::operator != (const KeyNode& node) const {
	return (num != node.num);
}

bool KeyNode::operator <  (const KeyNode& node) const {
	return (num < node.num);
}

bool KeyNode::operator >  (const KeyNode& node) const {
	return (num > node.num);
}

bool KeyNode::operator <= (const KeyNode& node) const {
	return (num <= node.num);
}

bool KeyNode::operator >= (const KeyNode& node) const {
	return (num >= node.num);
}

KeyNode KeyNode::operator = (const KeyNode& node) {
	num = node.num;
	return *this;
}

KeyNode KeyNode::operator = (const size_t& number) {
	num = number;
	return *this;
}

size_t KeyNode::operator () () const {
	return num;
}

void KeyNode::setKeyValue (const size_t& id1, const size_t& id2) {
	num = getKeyValue(id1, id2);
}

static size_t getKeyValue (const size_t& id1, const size_t& id2) {
	if (id1 < id2)	return (id1 << (sizeof(size_t) / 2)) + id2;
	else				return (id2 << (sizeof(size_t) / 2)) + id1;
}

