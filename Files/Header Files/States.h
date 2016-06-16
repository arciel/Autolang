#ifndef STATES_H
#define STATES_H

#include "State.h"
#include <unordered_map>	

/* The definition of a set of states. */

typedef std::unordered_map<std::string, State *> States;

#endif 