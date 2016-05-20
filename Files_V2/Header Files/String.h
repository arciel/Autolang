#ifndef STRING_H
#define STRING_H

#include "Elem.h"

using std::string;

class String : public Elem
{
public:
	string elem;

	String() : Elem(STRING) { elem = ""; }			// Default constructor.

	String(string &x) : Elem(STRING) { elem = x; }		// Parameterized constructor 1.

	String(const char* x) : Elem(STRING) { elem = x; }	// Parameterized constructor 2.

	bool operator==(Elem &x)				// Overloaded == operator.
	{ 
		if (x.type != STRING) return false;		// Check for types first and foremost.
		String* _x = (String *)&x;
		return this->elem == _x->elem;			// Then check for value.
	} 

	Elem* deep_copy() { return new String(elem); }

	string to_string() { return "\"" + elem + "\""; }

	~String() {  }						// Destructor - Delete the elem object.
};

#endif