#ifndef CHAR_H
#define CHAR_H

#include "Elem.h"

class Char : public Elem
{
public:
	char elem;

	Char() : Elem(CHAR) { elem = '\0'; }		// Default constructor.

	Char(char c) : Elem(CHAR) { this->elem = c; }	// Parametrized constructor.

	bool operator==(Elem &x)			// Overloaded == operator.
	{ 
		if (x.type != CHAR) return false;	// Check for types first and foremost.
		Char *_x = (Char *)&x;
		return this->elem == _x->elem;		// Then check for value.
	} 

	Elem* deep_copy() { return new Char(elem); }

	string to_string()
	{
		string representation = "'";
		representation += elem;
		representation += "'";
		return representation;
	}
};
#endif