# Sample program

automaton_desc (M) {

	sigma = {'1', '2'};

	states = {state("Label1"), state("Label2")};

	delta : states x sigma --> states {
	
		(states["Label1"], sigma['1']) --> states["Label2"];	
		
		(states["Label1"], sigma['2']) --> states["Label1"];

		(states["Label2"], sigma['1']) --> states["Label2"];

		(states["Label2"], sigma['2']) --> states["Label2"];
		
	}

	accepting = {states["Label2"]};

	M = (alpha, states, states["Label1"], delta, accepting);
}

>> M.accepts("1212")

>> M.accepts("2222")

# Sample output

>> True

>> False