# An experimental language.

This semester, I'm learning about programming languages and automata theory. In order to gain a better understanding of language design, I decided to make *Autolang*.

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

Comments are preceded by a '#', and can span a single line (though they're not supported as of this commit).

V1 was really an experiment, I just wanted to get my feet wet with syntax analysis and interpretation. I'll put up a Makefile soon, but all files pertaining to it are in the *Files* directory, should be easy to compile the interpreter.

#### Version 2 (wip)

Autolang V2 will be a major departure from V1, moving towards more conventional syntax, and will be decently powerful and definitely Turing complete. It will allow low-level, 'first-order' programs to be written in it.

*Files_V2/Grammar_V2.bnf* is a draft grammar for V2. It covers nearly all of the features, and reading it will give you a good insight into the language's future plans (also helps to work on the implementation better).