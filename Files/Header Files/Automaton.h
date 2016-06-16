#ifndef AUTOMATON_H
#define AUTOMATON_H

#include "States.h"
#include "Delta.h"

/* The definition of an Automaton in AL. */

class Automaton
{
	States states;						  // The set of states, stored against their labels.
	std::string sigma;					  // The character-set, or the alphabet.
	Transition delta;					  // The map representing the transition function.
	State *initial;						  // The pointer to the initial state.
	States accepting;					  // The set of accepting states.
	void next(State *&, char);			  // The transition function.
	void query(State *&, std::string &);  // The extended transition function.
public:
	Automaton()  { sigma = ""; initial = nullptr; }
    ~Automaton() {}
	bool accepts(std::string &);
	void add_state(std::string &);
	bool has_state(std::string &);
	void add_transition(std::string &, char, std::string &);
	void add_acceptingstate(std::string &);
	void set_initial(std::string &);
	void add_char(char &);
	bool has_char(char &);
};

#endif