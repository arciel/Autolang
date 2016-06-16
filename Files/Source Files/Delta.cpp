/* Implementation for operators in the class 'Transition' */

#include "../Header Files/Delta.h"

State* Transition::operator[](state_char_pair pair)
{
	int index;
	for (index = 0; index < lhs.size(); index++)
		if (pair.st == lhs[index].st && pair.input == lhs[index].input)
			break;
	return (index < lhs.size()) ? rhs[index] : nullptr;
}

const State* Transition::operator[](state_char_pair pair) const
{
	int index;
	for (index = 0; index < lhs.size(); index++)
		if (pair.st == lhs[index].st && pair.input == lhs[index].input)
			break;
	return (index < lhs.size()) ? rhs[index] : nullptr;
}

void Transition::addtransition(state_char_pair pair, State *st)
{
	lhs.push_back(pair);
	rhs.push_back(st);
}

/*void test()				To test the map.
{
	Transition del;
	State *s1 = new State("Label1"), *s2 = new State("Label2");
	del.addtransition({ s1, '1' }, s2);
	std::cout << del[{s1, '1'}]->getlabel() << std::endl;
	if (del[{s1, '2'}] == nullptr)
	std::cout << "Yep, not found.\n";
	delete s1;
	delete s2;
}*/