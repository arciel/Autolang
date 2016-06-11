#ifndef AUTO_H
#define AUTO_H

#include "Map.h"
#include "String.h"
#include "Char.h"
#include "Logical.h"

#define mytuple std::tuple<Set*,Set*,Elem*,Map*> 

// Using C++'s tuple over Autolang's Tuple because 1. Memory 2. Doesn't recursively delete data.

class Auto : public Elem			 // DFA.
{
private: 
	mytuple * make_super_automata(Auto *);	 // Makes the states = Q1 x Q2, and M((q1, q2), c) = (M1(q1, c), M2(q2, c)).
public:
	Set *sigma;			// The alphabet of the automaton. (Homotype == Character).
	Set *states;			// The set of states (labels really). (Homoset).
	Map *delta;			// The transition function from sigma to states.
	Elem *start;			// The label of (or tuples of labels for) the starting state.
	Set *accepting;			// The set of accepting states (labels thereof).
										    
	Auto(Set *, Set *, Elem *, Map *, Set *);		// Constructor (Copy elements).
	Auto(Set *, Set *, Elem *, Map *, Set *, int);		// Constructor (Direct assign).

	Logical accepts(String &);		// Returns True if the automaton is in an accepting state after running the query string.
	Auto * accepts_union(Auto *);		// Returns an automaton that accepts the union of the languages of this and other automaton.
	Auto * accepts_intersection(Auto *);	// Returns an automaton that accepts the intersection of the languages of this and other auto.
	Auto * accepts_exclusively(Auto *);	// Returns an automaton that accepts all string in L(this) that are not in L(other).
	Elem * deep_copy();			// Returns a deep_copy of this automaton.
	bool operator==(Elem &);		// The overloaded == operator for Automata.
	string to_string();			// Returns a string representation of the automaton.

	~Auto();
};

#endif