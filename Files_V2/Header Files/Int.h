#ifndef INT_H
#define INT_H

#include "Elem.h"
#include <sstream>

class Int : public Elem
{
public:
	int elem;						// Only going to implement the bare-minimum methods.
	Int() : Elem(INT) { elem = 0; }				// Default constructor.
	Int(int x) : Elem(INT) { elem = x; }			// Parameterized constructor.
	Int(string x) : Elem(INT) { elem = atoi(x.c_str()); }	// Construct an int object using a string representation of it.
	Elem* deep_copy() { return new Int(elem); }		// Return the address of a new Int object constructed with the value of this.
	bool operator==(Elem &x)
	{
		if (x.type != INT) return false;		// Check for types first and foremost.
		Int *_x = (Int *)&x;
		return (_x->elem == this->elem);
	}
	string to_string() 					// Return a string constructed using the integer elem cast to a character array.
	{
		std::stringstream representation;
		representation << elem;
		return representation.str();
	} 
	~Int() { }						// Empty destructor.
};

#endif