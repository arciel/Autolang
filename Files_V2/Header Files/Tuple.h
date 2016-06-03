#ifndef TUPLE_H
#define TUPLE_H

#include "Elem.h"
#include <vector>

using std::vector;
using std::string;

class Tuple : public Elem
{
public:
	vector<Elem *> *elems;
	Tuple();				// Default constructor, empty tuple.
	Tuple(vector<Elem *> *);		// Tuple-ize an existing vector of element_pointers (Copy pointers).
	Tuple(vector<Elem *> *, int);		// Tuple-ize an existing vector of element_pointers (Direct assign).
	Elem * deep_copy();			// Returns a tuple which is a deep_copy of this tuple.
	bool has(Elem &);			// Checks if a given element is present in the tuple.
	const Elem * operator[](int) const;     // R-value access.
	Elem * & operator[](int);		// L-value access.
	bool operator==(Elem &);		// Checks two tuples for equality.
	int size();				// Returns the size (or 'dimension') of this tuple.
	string to_string();			// Returns a string representation of the tuple.
	~Tuple();				// Destructor.
};

#endif