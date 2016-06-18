#ifndef INT_H
#define INT_H

#include "Elem.h"

class Int : public Elem
{
public:
	int elem;						// Only going to implement the bare-minimum methods.
	Int() : Elem(INT) { elem = 0; }				// Default constructor.
	Int(int x) : Elem(INT) { elem = x; }			// Parameterized constructor.
	Int(string x) : Elem(INT) { elem = std::stoi(x); }	// Construct an int object using a string representation of it.
	Elem* deep_copy() { return new Int(elem); }		// Return the address of a new Int object constructed with the value of this.
	bool operator==(Elem &x)
	{
		if (x.type != INT) return false;		// Check for types first and foremost.
		Int *_x = (Int *)&x;
		return (_x->elem == this->elem);
	}	
	bool operator<(Elem &x)					// Compares two ints.
	{
		Int *_x = (Int *)&x;
		return this->elem < _x->elem;
	}
	string to_string() 					// Return a string constructed using the integer elem cast to a character array.
	{
		return std::to_string(elem);			// Use C++'s own to_string function.
	} 
	~Int() { }						// Empty destructor.
};

#endif