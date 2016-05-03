#include "Set.h"

/* Implementations for methods in the class Set. */

Set::Set() : Elem(SET)                     // Default constructor.
{
	elems = new vector < Elem * >;
}

Set::Set(vector<Elem *> *elems): Elem(SET)     // It is possible to initialize a set with an existing vector.
{
	this->elems = elems;                   // Just make the set's vector_pointer point to that existing vector.
}

int Set::cardinality()                         // Returns the cardinality of the set.
{
        return elems->size(); 
}

Set Set::cartesian_product(Set &other)         // Returns the cartesian product of this set and the other set.
{
	Set product;                                  // Make an empty set.
	for (auto &elem_p1 : *elems)                  // For every element_pointer in the vector of element_pointers in this set ...
		for (auto &elem_p2 : *(other.elems))  // ... taken with every element pointer in the vector of element_pointers in the other ...
			product.elems->push_back(                    // ... <convoluted_code> Push into the cartesian_product (cart) ...
			new Tuple(                                   // ... a pointer to a Tuple object, that is ... 
			new vector < Elem * >{ elem_p1, elem_p2 })   // ... constructed using a vector initialized with elem_p1 and elem_p2.                                                 
			);                                           // </convoluted_code>
	return product;
}

Elem* Set::deep_copy()                         // Returns a new set which is a deep-copy of this set.
{
	Set *clone = new Set;					// Make an empty clone set.
	for (auto &elem_p1 : *elems)				// For every element_pointer in the vector of element_pointers in this set ...
		clone->elems->push_back(elem_p1->deep_copy());  // ... push into the clone, a deep_copy of the object pointed to by the pointer. 
	return clone;						// Return a pointer to the clone.
}

Set Set::exclusion(Set &exclude)	       // Returns a set containing the elements of this set, minus those of the argument. 
{
	Set exclusive;						// Make an empty set.
	for (auto &elem_p1 : *elems)				// For every (any) element_pointer in the vector of element_pointers in this ...  
		if (!exclude.has(*elem_p1))			// ... if the element it points to is not present in the set to be excluded ...
			exclusive.elems->push_back(elem_p1);    // ... then add a pointer to that element in the exclusive set.
	return exclusive;
}

bool Set::has(Elem &elem)                      // Looks for an element in the set.
{
	for (auto &elem_p : *elems)            // For each (any) element_pointer in the vector of element_pointers ...
		if (*elem_p == elem)           // ... if the object pointed to by that element_pointer is == the argument ...
			return true;           // ... then return true.
	return false;                          // Else return false.
}

Set Set::intersection(Set &other)              // Intersection with a second set.
{
	Set intersect;                                             // Make an initially empty set since we don't know what we may have to add.
	for (int i{ 0 }; i < elems->size(); i++)                   // For every element_pointer in this set, check to see if ...
		if (other.has(*(*elems)[i]))                       // ... the element pointed to by it is in the other one too. If it is ...
			intersect.elems->push_back((*elems)[i]);   // ... then add a pointer to that element to their intersection.
	return intersect;
}

const Elem & Set::operator[](int index) const    // R-value access.
{
	return *(*elems)[index];                 // Return a reference to an element pointed to by the element_pointer at index. 
}

Elem *& Set::operator[](int index)               // L-value access.
{
	return(*elems)[index];                   // Return an element_pointer at index.     
}

bool Set::operator==(Set &other)                 // Checks two sets for equality.
{
	if (cardinality() != other.cardinality())    // If the cardinalities are different ... 
		return false;                        // ... then they are obviously not equal.
	return this->subset_of(other);               // But if they are the same, then if either is a subset of the other, they indeed are equal.
}

Set Set::subset(int start, int end)              // Returns a subset of the set containing elements in [start, end).
{
	Set subset;                                      // Make an empty set.
	for (int i{ start }; i < end; i++)               // Start at the starting index, go up till the ending index.
		subset.elems->push_back((*elems)[i]);    // Add all the element_pointers between the limits to the subset.
	return subset;                                   // Then return the subset.
}

bool Set::subset_of(Set &candidate_superset)     // Checks if this set is a subset of the candidate_superset.
{
	for (auto &elem_p : *elems)                    // For every (any) element_pointer in this set's vector ...
		if (!candidate_superset.has(*elem_p))  // ... if the candidate_superset does not have a pointer to the object it refers to ...
			return false;                  // ... return false.
	return true;                                   // Else return true.
}

string Set::to_string()                          // Returns a string representation of the set.
{
	string representation{ "{ " };
	int i{ 0 };
	for (auto &elem_p : *elems)
	{
		representation += elem_p->to_string();   // Recursive, awesome representations. ;)
		if (i != elems->size() - 1)
			representation += ", ";
	}
	return representation + " }";
}

Set Set::_union(Set &other)                // Union with a second set.
{
	Set unified(this->elems);
	for (auto &elem_p : *(other.elems))        // For every other element_pointer in the other set's vector ...
		if (!this->has(*elem_p))           // ... if the object pointed to by it is not already present in this set ...
			elems->push_back(elem_p);  // ... add a pointer to it to this set as well.
	return unified;
}
