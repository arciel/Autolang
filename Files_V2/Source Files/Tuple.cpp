#include "../Header Files/Tuple.h"

/* Implementations for the methods in the class Tuple. */

Tuple::Tuple() : Elem(TUPLE)				// Default constructor, empty tuple.
{
	elems = new vector < Elem * >;
}

Tuple::Tuple(vector<Elem *> *elems) : Elem(TUPLE)	// Tuple-ize an existing vector of element_pointers.
{
	this->elems = new vector<Elem *>(*elems);
}

Elem* Tuple::deep_copy()				// Returns a tuple which is a deep_copy of this tuple.
{
	Tuple *clone = new Tuple;				// Make an empty clone tuple.
	for (auto &elem_p1 : *elems)				// For every element pointed to in the vector of element_pointers in this ...
		clone->elems->push_back(elem_p1->deep_copy());  // ... tuple, add to the clone tuple a deep_copy (recursively gen) of it.
	return clone;						// Then return the clone.
}

const Elem & Tuple::operator[](int index) const         // R-value access.
{
	return *(*elems)[index];                        // Return a reference to an element pointed to by the element_pointer at index. 
}

bool Tuple::operator==(Elem &other_tuple)		// Checks two tuples for equality.	
{
	if (other_tuple.type != TUPLE) return false;
	Tuple *other = (Tuple *)&other_tuple;
	if (size() != other->size())				// If the sizes are different ... 
		return false;					// ... then they are obviously not equal.
	for (int i{ 0 }; i < size(); i++)			// So the sizes of the tuples are the same. Now if at any index i ...
		if (!(*(*elems)[i] == *(*other->elems)[i]))	// ... the elements in the two tuples are different, then ...
			return false;				// ... then the tuples are not equal.
	return true;						// Otherwise they are.
}

int Tuple::size()					// Returns the size (or 'dimension') of this tuple.
{
	return elems->size();
}

string Tuple::to_string()				// Returns a string representation of the tuple.
{
	string representation{ "(" };
	int i{ 0 };
	for (auto &elem_p : *elems)
	{
		representation += elem_p->to_string();  // Recursive, awesome representations. ;)
		if (i != elems->size() - 1)
			representation += ", ";
		i++;
	}
	return representation + ")";
}

Tuple::~Tuple()				  // Destructor.
{
	for (auto &elem_p1 : *elems)	  // For every element_pointer in the vector of element_pointers in this tuple ...
		delete elem_p1;		  // ... delete the object pointed to by that pointer.
	delete elems;			  // And when done, delete the vector too.
}