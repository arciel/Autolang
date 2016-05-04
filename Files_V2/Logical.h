#ifndef LOGICAL_H
#define LOGICAL_H

#include "Elem.h"

class Logical : public Elem
{
public:
	bool elem;				// A boolean element is the internal representation.
	Logical();				// Default constructor.
	Logical(bool);				// Another constructor with argument.
	Elem* deep_copy();			// Deep_copy method, which of course returns just a logical instance since it's atomic.
	Logical operator!();			// Overloaded negation operator.
	Logical operator&&(Logical &x);		// Overloaded logical and. 
	Logical operator||(Logical &x);		// Overloaded logical or.
	const Logical& operator=(Logical &);	// Overloaded logical or.
	string to_string();			// Virtual to_string method for display.
};

#endif