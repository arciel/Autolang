# An experimental language.

In spring 2016, I was learning about programming languages and automata theory. In order to gain a better understanding of language design, I decided to make *Autolang*. You've somehow stumbled upon Version 1 of it, which I no longer maintain. Thanks for checking it out anyway!

#### Version 1

Version 1 of Autolang is a small, relatively uninteresting language that lets you describe *Deterministic Finite Automata* (DFA), and run queries on them.

For instance, an automaton *M* in *Autolang* can be described with the following syntax

```perl
automaton_desc (M) {
	
	sigma = { '1', '2' }
	
	states = { state("Label1"), state("Label2") }

	delta : states x sigma --> states { 

		("Label1", '1') --> "Label2"	
		
		("Label1", '2') --> "Label1"

		("Label2", '1') --> "Label2"

		("Label2", '2') --> "Label2"
	}

	accepting = { states["Label2"] }

	M = (sigma, states, states["Label2"], delta, accepting)
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

V1 was really an experiment, I just wanted to get my feet wet with syntax analysis and interpretation. For the current face of Autolang, checkout Version 2.
