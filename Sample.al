# Sample program

automaton_desc (M) {

	sigma = {'1', '2'}

	states = {state("Label1"), state("Label2")}

	delta : states x sigma --> states {
	
		("Label1", '1') --> "Label2"	
		
		("Label1", '2') --> "Label1"

		("Label2", '1') --> "Label2"

		("Label2", '2') --> "Label2"
		
	}

	accepting = {states["Label2"]}

	M = (sigma, states, states["Label1"], delta, accepting)
}

>> M.accepts("1212")

>> M.accepts("2222")

# Sample output

# >> True

# >> False