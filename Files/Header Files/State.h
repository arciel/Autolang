#ifndef STATE_H
#define STATE_H

#include <string>

class State 
{
	std::string label;			// Label for the state.
public:
	State() 
	{
		this->label = "";
	}
	State(std::string label)
	{
		this->label = label;
	} 
	std::string getlabel()
	{
		return this->label;
	}
};

#endif