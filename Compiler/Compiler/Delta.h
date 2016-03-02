#ifndef DELTA_H
#define DELTA_H

#include "State.h"
#include <map>

/* The definition of the transition function. */

struct state_char_pair
{
	State *st;
	char input;
};

typedef std::map<state_char_pair, State *> Transition;

#endif