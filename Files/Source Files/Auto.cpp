#define _CRT_SECURE_NO_WARNINGS

#include "../Header Files/Auto.h"
#include <cstdlib>
#include <iostream>

using std::cout;
using std::endl;

/* Implementations for the methods in the class Auto. */

void display_error(string message)
{
	cout << "The requested automaton could not be properly constructed, because: " << message;
	cout << " Use at your own risk." << endl;
}

Auto::Auto(Set *states, Set *sigma, Elem *start, Map *delta, Set *accepting) : Elem(AUTO)			// Constructor (Copy elements).
{
	if (!states->homoset() || !sigma->homoset())				// The set of states and the alphabet must be homogeneous.
	{
		display_error("The set of states or the alphabet is not homogeneous.");
		return;								
	}
	if (sigma->homotype() != CHAR)						// forall x in sigma x.type = CHAR  
	{
		display_error("The alphabet is not a homoset set of characters.");
		return;
	}
	if (!accepting->subset_of(*states))					// The set of accepting states must be a subset of states.
	{
		display_error("The set of accepting states is not a subset of the set of states.");
		return;
	}
	if (!delta->domain()->subset_of(*states->cartesian_product(*sigma)) ||	// If the domain of the map is not a subset of states X sigma...
		!delta->codomain()->subset_of(*states))				// ... or if the codomain is not a subset of states ...
	{
		display_error("The transition function has improper domain or codomain.");
		return;								// ... don't make the automaton.
	}
	this->states	= (  Set  *) states->deep_copy();
	this->sigma	= (  Set  *) sigma->deep_copy();
	this->start	=	     start->deep_copy();
	this->delta	= (  Map  *) delta->deep_copy();
	this->accepting = (  Set  *) accepting->deep_copy();
}

Auto::Auto(Set *states, Set *sigma, Elem *start, Map *delta, Set *accepting, int dir_assign) : Elem(AUTO)      // Constructor (Direct assign).
{
	if (!states->homoset() || !sigma->homoset())				// The set of states and the alphabet must be homogeneous.
	{
		display_error("The set of states or the alphabet is not homogeneous.");
		return;
	}
	if (sigma->homotype() != CHAR)						// forall x in sigma x.type = CHAR  
	{
		display_error("The alphabet is not a homoset set of characters.");
		return;
	}
	if (!accepting->subset_of(*states))					// The set of accepting states must be a subset of states.
	{
		display_error("The set of accepting states is not a subset of the set of states.");
		return;
	}
	if (!delta->domain()->subset_of(*states->cartesian_product(*sigma)) ||	// If the domain of the map is not a subset of states X sigma...
		!delta->codomain()->subset_of(*states))				// ... or if the codomain is not a subset of states ...
	{
		display_error("The transition function has improper domain or codomain.");
		return;								// ... don't make the automaton.
	}						

	this->states = states;
	this->sigma = sigma;
	this->start = start;
	this->delta = delta;
	this->accepting = accepting;
}

Logical * Auto::operator[](String &query)
{
	Elem *current_state = start;			// Initially, the current_state is the starting state 'start'.
	for (auto &character : query.elem)		// For every character in the query string ...
	{	
		Tuple t(new vector<Elem *> {				// Make a tuple t ...
			current_state->deep_copy(), new Char(character) // ... that contains pointers to the deep_copies of the current_state...
		}, DIRECT_ASSIGN);					// ... and a Char object constructed with the current character.
		if (((*delta)[t]) == nullptr) 		// If there does not exist any mapping from the current (state, char) pair ...
			return new Logical(false);	// ... return false because we're going to a dump state.

		current_state = (Elem *)(*delta)[t];	    // Use this tuple as input to the transition map 'delta', and update the current_state.
	}						    // Finally we'll have current_state as the final state of the query.
	return new Logical(accepting->has(*current_state)); // If the final state is in the set of accepting states, return True, else return False.
}

const Logical * Auto::operator[](String &query) const
{
	Elem *current_state = start;			// Initially, the current_state is the starting state 'start'.
	for (auto &character : query.elem)		// For every character in the query string ...
	{
		Tuple t(new vector<Elem *>{				// Make a tuple t ...
			current_state->deep_copy(), new Char(character) // ... that contains pointers to the deep_copies of the current_state...
		}, DIRECT_ASSIGN);					// ... and a Char object constructed with the current character.
		if (((*delta)[t]) == nullptr) 		// If there does not exist any mapping from the current (state, char) pair ...
			return new Logical(false);	// ... return false because we're going to a dump state.

		current_state = (Elem *)(*delta)[t];	    // Use this tuple as input to the transition map 'delta', and update the current_state.
	}						    // Finally we'll have current_state as the final state of the query.
	return new Logical(accepting->has(*current_state)); // If the final state is in the set of accepting states, return True, else return False.
}

mytuple * Auto::make_super_automata(Auto *other)	// Makes the states = Q1 x Q2, and M((q1, q2), c) = (M1(q1, c), M2(q2, c)).
{
	if (!(*other->sigma == *this->sigma)) 		// If the alphabets of the two automata are different ...
		return nullptr;				// ... the we cannot make the unified automaton.

	Set * new_states = this->states->cartesian_product(*other->states);	// The new set of states is a set of 2-tuples of states ...
										// ... from the two automata.
	Set * new_delta_domain = new_states->cartesian_product(*this->sigma);	// The domain of new delta is a set of 2-tuples of new states ...
										// ... and input alphabet characters.
	Set * new_delta_codomain = new_states;					// The codomain of new delta is just the set of new states.

	Tuple * new_start = new Tuple(						// We'll now make the Tuple representing the starting state ... 
		new vector <Elem *> { this->start, other->start }		// ... of the new automaton, (q_01, q_02).
	);

	Map * new_delta = new Map(new_delta_domain, new_delta_codomain);	// Let's make the map, and then we'll add the mappings.

	for (auto &pre_image : *new_delta_domain->elems)			// Now we'll add mappings M((q1, q2), c) = (M1(q1, c), M2(q2, c)) 
	{
		Tuple * preimage = (Tuple *)pre_image;				// *preimage = ((q1, q2), c)
		Tuple * state_pair = (Tuple *)(*preimage)[0];			// *state_pair = (q1, q2)
		Char  * sigma = (Char  *)(*preimage)[1];			// *sigma = c 

		Tuple first_state_char_pair (					// first_state_char_pair = (q1, c)
			new vector < Elem * > {
				(*state_pair)[0]->deep_copy(),			// Here we're making a deep-copy of q1.
				sigma->deep_copy()				// And here one of sigma. 
			},							// So now the tuple is (q1, c),
			DIRECT_ASSIGN						// Because we want to use this vector here, and not a copy.
		);

		// Note that this tuple is not dynamic, because it will only be used for lookup in this->delta,
		// and we actually want it to be destroyed (hence the deep copies of q1 and c).

		Tuple * mapped_first_state_char_pair = (Tuple *)(*this->delta)[first_state_char_pair];   // M1(q1, c)

		Tuple second_state_char_pair(					// second_state_char_pair = (q2, c)
			new vector < Elem * >{
				(*state_pair)[1]->deep_copy(),			// Here we're making a deep-copy of q2. 
				sigma->deep_copy()				// And here one of sigma.
			},							// So now the tuple is (q2, c)
			DIRECT_ASSIGN						// Because we want to use this vector here, and not a copy.
		);

		Tuple *mapped_second_state_char_pair = (Tuple *)(*other->delta)[second_state_char_pair]; // M2(q2, c)

		Tuple image(							// Now we're making the tuple (M1(q1, c), M2(q2, c))
			new vector < Elem * >{					// So we'll make a tuple object initialized with a vector ...
				mapped_first_state_char_pair->deep_copy(),	// ... containing the deep_copy of M1(q1, c), and ...
				mapped_second_state_char_pair->deep_copy()	// ... containing the deep_copy of M2(q2, c).
			},							// Since we only need this tuple for lookup and it will be ...
			DIRECT_ASSIGN						// ... destroyed, we direct assign the vector of deep copies ...
		);								// ... so that it is destroyed along with the tuple.
		new_delta->add_maping(*preimage, image);			// Now finally, add the mapping.
	}
	return new mytuple{ new_states, this->sigma, new_start, new_delta };
	// Return the components of the super automaton, without the accepting set of states.
}

Auto * Auto::accepts_union(Auto * other)	// Returns an automaton that accepts the union of the languages of this and other automaton.
{
	Set * new_states, * new_alphabet;
	Elem * new_start; Map * new_delta;

	mytuple * components = this->make_super_automata(other);		// We make the super automaton's components ...
	std::tie(new_states, new_alphabet, new_start, new_delta) = *components;	// ... put them all in their rightful place ...
	delete components;							// ... and then delete the tuple with the pointers to them.

	Set * new_accepting = new Set;				// Make an empty set of accepting states for now.
	for (auto &state_pair : *new_states->elems)		// For every state in the set of states of the union(ized) automaton ...
	{
		Tuple * statepair = (Tuple *)state_pair;        // ... cast it into (q1, q2) for C++, because it's too dumb to do it itself.
		if (this->accepting->has(*(*statepair)[0]) ||   // Now, if the q1 is in the set of accepting states of this automaton, or if ...
		    other->accepting->has(*(*statepair)[1]))	// ... q2 is in the accepting states of the other automaton ...
		{
		    new_accepting->elems->push_back(statepair); // ... then (q1, q2) ought to be in the set of accepting states of the union. 
		}
	}
	return new Auto(new_states, new_alphabet, new_start, new_delta, new_accepting, DIRECT_ASSIGN);	// Return the unionized automaton.
}

Auto * Auto::accepts_intersection(Auto * other)	// Returns an automaton accepting the intersection of the languages of this and other automaton.
{
	Set * new_states, *new_alphabet;
	Elem * new_start; Map * new_delta;

	mytuple * components = this->make_super_automata(other);		// We make the super automaton's components ...
	std::tie(new_states, new_alphabet, new_start, new_delta) = *components;	// ... put them all in their rightful place ...
	delete components;							// ... and then delete the tuple with the pointers to them.

	Set * new_accepting = new Set;				// Make an empty set of accepting states for now.
	for (auto &state_pair : *new_states->elems)		// For every state in the set of states of the union(ized) automaton ...
	{
		Tuple * statepair = (Tuple *)state_pair;        // ... cast it into (q1, q2) for C++, because it's too dumb to do it itself.
		if (this->accepting->has(*(*statepair)[0]) &&   // Now, if the q1 is in the set of accepting states of this automaton, and ...
		    other->accepting->has(*(*statepair)[1]))	// ... q2 is in the accepting states of the other automaton ...
		{
		    new_accepting->elems->push_back(statepair); // ... then (q1, q2) ought to be in the set of accepting states of the union. 
		}
	}
	return new Auto(new_states, new_alphabet, new_start, new_delta, new_accepting, DIRECT_ASSIGN); // Return the intersect automaton.
}

Auto * Auto::accepts_exclusively(Auto * other)	// Returns an automaton that accepts all string in L(this) that are not in L(other).
{
	Set * new_states, *new_alphabet;
	Elem * new_start; Map * new_delta;

	mytuple * components = this->make_super_automata(other);		// We make the super automaton's components ...
	std::tie(new_states, new_alphabet, new_start, new_delta) = *components;	// ... put them all in their rightful place ...
	delete components;							// ... and then delete the tuple with the pointers to them.

	Set * new_accepting = new Set;				// Make an empty set of accepting states for now.
	for (auto &state_pair : *new_states->elems)		// For every state in the set of states of the union(ized) automaton ...
	{
		Tuple * statepair = (Tuple *)state_pair;        // ... cast it into (q1, q2) for C++, because it's too dumb to do it itself.
		if (this->accepting->has(*(*statepair)[0]) &&   // Now, if the q1 is in the set of accepting states of this automaton, and ...
		   (!other->accepting->has(*(*statepair)[1])))	// ... q2 is NOT in the accepting states of the other automaton ...
		{
		    new_accepting->elems->push_back(statepair); // ... then (q1, q2) ought to be in the set of accepting states of the union. 
		}
	}
	return new Auto(new_states, new_alphabet, new_start, new_delta, new_accepting, DIRECT_ASSIGN); // Return the exclusive automaton.
}

Elem * Auto::deep_copy()					// Return a deep_copy() of this automaton.
{
	return new Auto(					// Simply just construct a new automaton ... 
		(Set *)this->states->deep_copy(),		// ... with deep_copies of all its components ...
		(Set *)this->sigma->deep_copy(), 
		this->start->deep_copy(),
		(Map *)this->delta->deep_copy(),
		(Set *)this->accepting->deep_copy(),
		DIRECT_ASSIGN					// ... directly assign to its corresponding fields.
	);
}

bool Auto::operator==(Elem &other)				// The overloaded == operator for Automata.
{
	if (other.type != AUTO)					// If the other element is not an automaton, it  won't be equal to this.
		return false;	
	Auto * that = (Auto *) & other;				// We can introduce a pointer to other, called that, with an Auto interface.
	return							// Now return the result of this expression.
	(
		(this->states == that->states) &&		// Between you and me, this probably won't ever be used. But well, why not.
		( this->sigma == that->sigma ) &&
		( this->start == that->start ) &&
		( this->delta == that->delta ) &&
		(this->accepting == that->accepting)
	);
}

string Auto::to_string()					// Returns a string representation of this automaton.
{
	return "Cannot print automata.";
}

Auto::~Auto()
{
	if (states != nullptr) delete states;
	if (sigma != nullptr) delete sigma;
	if (delta != nullptr) delete delta;
	if (start != nullptr) delete start;
	if (accepting != nullptr) delete accepting;
}
