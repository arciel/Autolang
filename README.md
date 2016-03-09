# An experimental language.

This semester, I'm learning about programming languages and automata theory. It only made sense to make this language. 

The Automata Language is a small programming language that lets you describe automata, and run queries on them.

For instance, an automaton *M* can be described with the following syntax

```perl
automata_desc (M) {
	
	sigma = { '1', '2' }
	
	states = { state("Label1"), state("Label2") }

	delta : states x sigma --> states { 

		("Label1", '1') --> "Label2"	
		
		("Label1", '2') --> "Label1"

		("Label2", '1') --> "Label2"

		("Label2", '2') --> "Label2"
	}

	accepting = { states["Label2"] }

	M = (sigma, states, states["Label"], delta, accepting)
}
```

Queries have the following syntax

```perl
>> M.accepts("1212")

>> M.accepts("2222")
```

The result of interpreting this code will be 

```perl
>> 1

>> 0
```

where 1 represents True, and 0 represents False. 

Comments are preceded by a '#', and can span a single line.

The Automata Language will eventually support if statements, for loops, functions not bound to automata, and sets, amongst other things. More features to come as I learn more!



