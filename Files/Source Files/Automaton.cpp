/* Implementations for the methods in the class 'Automaton'. */

#include "../Header Files/Automaton.h"

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
	return (accepting[s] != nullptr);
}

void Automaton::add_state(std::string &label)
{
	states[label] = new State(label);
}

void Automaton::add_transition(std::string& s1, char c, std::string& s2)
{
	delta.addtransition({ states[s1], c }, states[s2]);
}

void Automaton::set_initial(std::string &label)
{
	initial = states[label];
}

bool Automaton::has_state(std::string &label)
{
	return (states[label] != nullptr);
}

void Automaton::add_acceptingstate(std::string &label)
{
	accepting[label] = states[label];
}

void Automaton::add_char(char &c)
{
	sigma += c;
}

bool Automaton::has_char(char &c)
{
	return (sigma.find(c) != std::string::npos);
}