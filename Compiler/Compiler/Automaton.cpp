/* Implementations for the methods in the class 'Automaton'. */

#include "Automaton.h"

void Automaton::next(char c) 
{
	
	current = delta[{current, c}];
}

void Automaton::query(std::string &s)
{
	current = initial;
	for (auto &c : s) next(c);
}

bool Automaton::accepts(std::string &s)
{
	query(s);
	return (accepting.count(s) == 1);
}

void Automaton::addstate(State *st)
{
	states[st->getlabel()] = st;
}

void Automaton::addtransition(std::string& s1, char c, std::string& s2)
{
	delta.addtransition({ states[s1], c }, states[s2]);
}

