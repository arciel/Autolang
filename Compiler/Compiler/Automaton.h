#ifndef AUTOMATON_H
#define AUTOMATON_H

#include "States.h"
#include "Delta.h"

/* The definition of an Automaton in AL. */

class Automaton
{
	States states;						// The set of states, stored against their labels.
	std::list<char> sigma;				// The character-set, or the alphabet.
	Transition delta;					// The map representing the transition function.
	State *initial;						// The pointer to the initial state.
	States accepting;					// The set of accepting states.
	State *current;						// The current state (to be set to 'initial' before each query).
	void next(char);					// The transition function.
	void query(std::string&);			// The extended transition function.
public:
	Automaton()  {}
    ~Automaton() {}
	bool accepts(std::string&);
	void addstate(State *);				
	void addtransition(std::string&, char, std::string&);

};

#endif