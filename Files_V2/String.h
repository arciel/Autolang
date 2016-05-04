#ifndef STRING_H
#define STRING_H

#include "Elem.h"

using std::string;

class String : public Elem
{
public:
	string elem;

	String() : Elem(STRING) { elem = ""; }	// Default constructor.

	String(string &x) : Elem(STRING) { elem = x; } // Parameterized constructor.

	Elem* deep_copy() { return new String(elem); }

	string to_string() { return elem; }
};

#endif