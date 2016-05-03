#ifndef SET_H
#define SET_H

#include "Tuple.h"
#include <vector>

using std::vector;
using std::string;

/* Definition of a Set in Autolang. */

class Set : public Elem
{
public:
	vector<Elem *> *elems;                     // A set is internally implemented as a vector of element_pointers.

	Set();                                     // Default constructor.
	Set(vector<Elem *> *);                     // It is possible to initialize a set with an existing vector (well, a pointer to it).
	int cardinality();                         // Returns the cardinality of the set.	
	Set cartesian_product(Set &);              // Returns the cartesian product of this set and the other set.
	Elem* deep_copy();			   // Returns a new set which is a deep_copy of this set.
	Set exclusion(Set &);                      // Returns a set containing elements of this, minus those of the argument.
	bool has(Elem &);                          // Looks for an element in the set.
	Set intersection(Set &);                   // Intersection with a second set.
	Set subset(int, int);                      // Returns a subset of the set containing elements in [start, end).
	bool subset_of(Set &);                     // Checks if this set is a subset of the candidate_superset.
	const Elem &operator[](int) const;         // R-value access.
	Elem *&operator[](int);                    // L-value access.
	bool operator==(Set &);                    // Checks two sets for equality.
	string to_string();                        // Returns a string representation of the set.
	Set _union(Set &);                         // Union with a second set.
};

#endif