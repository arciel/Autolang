#ifndef INT_H
#define INT_H

#include "Elem.h"
#include <cstdlib>

class Int : public Elem
{
public:
	int elem;					// Only going to implement the bare-minimum methods.
	Int() : Elem(INT) { elem = 0; }			// Default constructor.
	Int(int &x) : Elem(INT) { elem = x; }		// Parameterized constructor.
	Elem* deep_copy() { return new Int(elem); }	// Return the address of a new Int object constructed with the value of this.

	string to_string() { return string((char*)elem); } // Return a string constructed using the integer elem cast to a character array .
};

#endif