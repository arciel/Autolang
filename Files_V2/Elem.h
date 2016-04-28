#ifndef ELEM_H
#define ELEM_H

using std::string;

/* The definition of a generic Elem(ent) in Autolang. */

enum Type { AUTO = 1, CHAR, INT, LOGICAL, MAP, SET, STRING, TUPLE };    // Enum for the data types.

class Elem {	                     // An element can only actively represent one of data fields at a time.
public:
	Type type;						 // Type of data stored in this element.
	Elem(Type type)                  // Generic type-setting constructor.
	{
		this->type = type;
	}
	virtual bool operator==(Elem &elem)   // The equality comparison between two elements.
	{
		return (this->type != elem.type) ? false : (*this == elem); // Here's where the virtuality will help.
	}
	virtual string to_string() = 0;  // Virtual to_string method for display.
};

#endif