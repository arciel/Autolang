#ifndef DELTA_H
#define DELTA_H

#include "State.h"
#include <vector>

/* The definition of the transition function. */

struct state_char_pair
{
	State *st;
	char input;
};

class Transition {
	std::vector<state_char_pair> lhs;
	std::vector<State *> rhs;
public:
	State* operator[](state_char_pair);
	const State* operator[](state_char_pair) const;
	void addtransition(state_char_pair, State *);
};

#endif