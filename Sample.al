# Sample program.

automaton_desc (M) {				# Define automata M.

	sigma = {'1', '2'}				# Alphabet.

	states = {state("Label1"), state("Label2")}	# States by labels.

	delta : states x sigma --> states {		# Transition function.
	
		("Label1", '1') --> "Label2"
		
		("Label1", '2') --> "Label1"

		("Label2", '1') --> "Label2"

		("Label2", '2') --> "Label2"
		
	}

	accepting = {states["Label2"]}			# Accepting states.

	M = (sigma, states, states["Label1"], delta, accepting)
}

>> M.accepts("1212")				# Commands.

>> M.accepts("2222")

# Sample output.

# >> 1

# >> 0