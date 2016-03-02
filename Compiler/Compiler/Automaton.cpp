/* Implementations for the methods in the class 'Automaton'. */

#include "Automaton.h"

void Automaton::next(char c) 
{
	current = delta[{current, c}];
}

void Automaton::query(const std::string &s)
{
	current = initial;
	for (auto &c : s) next(c);
}

bool Automaton::accepts(const std::string &s)
{
	query(s);
	return accepting.count[current->getlabel()];		// Returns 1 if present (True), 0 if absent (False).
}

void Automaton::addstate(State *st)
{
	states[st->getlabel()] = st;
}

void Automaton::addtransition(const std::string& s1, char c, const std::string& s2)
{
	delta[{states[s1], c}] = states[s2];
}

