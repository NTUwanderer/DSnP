#ifndef KEYNODE
#define KEYNODE

#include <iostream>

using namespace std;

class KeyNode {
public:
	KeyNode(): num(0) {};
	KeyNode(const size_t&, const size_t&);
	~KeyNode();

	bool operator == (const KeyNode&) const;
	bool operator != (const KeyNode&) const;
	bool operator <  (const KeyNode&) const;
	bool operator >  (const KeyNode&) const;
	bool operator <= (const KeyNode&) const;
	bool operator >= (const KeyNode&) const;
	KeyNode operator = (const KeyNode&);
	KeyNode operator = (const size_t&);
	size_t operator () () const;

	void setKeyValue(const size_t&, const size_t&); 

private:
	size_t num;

	static size_t getKeyValue(const size_t&, const size_t&);
};

#endif // KEYNODE
