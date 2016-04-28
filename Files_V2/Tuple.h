#ifndef TUPLE_H
#define TUPLE_H

#include "Elem.h"
#include <vector>

using std::vector;
using std::string;

class Tuple : public Elem
{
public:
	vector<Elem *> *elems;
	
	Tuple() : Elem(TUPLE)                          // Default constructor, empty tuple.
	{
		elems = new vector < Elem * >;
	}

	Tuple(vector<Elem *> *elems) : Elem(TUPLE)     // Tuple-ize an existing vector of element_pointers.
	{
		this->elems = elems;
	}

	int size()
	{
		return elems->size();
	}

	string to_string()
	{
		string representation{ "( " };
		int i{ 0 };
		for (auto &elem_p : *elems)
		{
			representation += elem_p->to_string();
			if (i != elems->size() - 1)
				representation += ", ";
		}
		return representation + " )";
	}
};

#endif