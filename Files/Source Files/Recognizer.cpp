/*
 * A syntax analyzer and compiler for the Automata Language.
 * Experimental, work in progress. 
 * 
 * - Tushar
 */

#include <iostream>
#include <fstream>
#include <cctype>
#include <process.h>
#include "../Header Files/Automaton.h"

#define SYMBOL 0
#define CHAR 1
#define SPACE 2

// The list of keywords - { "sigma", "states", "delta", "automata_desc", "accepting", "state", "accepts" }.

// The lexeme classes and their corresponding tokens.
std::unordered_map<std::string, int> tokens
{
	{ "automaton_desc", 1 }, { "sigma", 2 },
	{ "{", 3 }, { "}", 4 }, { "[", 5 }, { "]", 6 },
	{ "-->", 7 }, { "delta", 8 }, { "accepting", 9 },
	{ "=", 10 }, { "'", 11 }, { "state", 12 },
	{ "states", 13 }, { "x", 14 }, { "\"", 15 },
	{ ">>", 16 }, { "(", 17 }, { ")", 18 },
	{ ":", 20 }, { ",", 21 }, { ".", 22 }, { "accepts", 23 }
};

// The lexemes stored against their tokens.
std::string lexemes[] {
	"", "automaton_desc", "sigma",
	"{", "}", "[", "]",
	"-->", "delta", "accepting",
	"=", "'", "state", "states", 
	"x", "\"", ">>", 
	"(", ")", "", ":", ",", ".", "accepts"
};

// The data members assigned in the language, stored against their types.
std::unordered_map<std::string, std::string> data_type;

/* <Global data.> */

int line_num = 1;                                      // The line that we're currently reading.
std::string lexeme;                                    // The current lexeme under inspection.
int token;                                             // The token for the current lexeme.
int char_class;                                        // Indicates what kind of lexeme we may be reading.
char character;                                        // The next character in the lexeme to be read in.
std::fstream prog;                                     // Will open the source code to be compiled.
std::string auto_name;                                 // The name of the current automata we're looking at.
std::unordered_map<std::string, Automaton *> Automata; // The set of automata that the source file wants to create, against their names. 

/* </Global data.> */


/* Function declarations.*/

void lex();                                                 // Generates the next lexeme and its token.
void error(const std::string&);                             // Error message.
void token_check(int);                                      // Checks the value of the token.
void getnonblank();                                         // Gets the next non-blank character from the source file.
void getcharacter();                                        // Gets a single character from the source file.
void parse_program();                                       // Parses the program in the source file.
void parse_automaton_desc();                                // Parses a single automaton's description header.
void parse_description();                                   // Parses a single automaton's description.
void parse_state_set();                                     // Parses a set of states.
void parse_acceptingstate_set();                            // Parses a set of states, with the additional info that they're accepting.
void parse_char_set();                                      // Parses a set of characters.
void parse_mappings();                                      // Parse the transition function mappings.
void parse_command();                                       // Parses a single command.
void stack_adjust(int);                                     // Just for formatting the testing output.

int main(int argc, char *argv[])
{
	prog.open((argc > 1) ? argv[1] : "Sample.al");		
	if (!prog)
	{
		error("Cannot open file.");
		return 0;
	}
	getcharacter();                                     // Get the first character to start the parsing.
	lex();                                              // Generate the first lexeme and its token.
	parse_program();                                    // Start parsing the first program.
	return 0;
}

void stack_adjust(int n)
{
	for (int i = 0; i < n; i++) std::cout << "\t";
}


// Parses the program in the source file.
void parse_program()
{
	stack_adjust(0);
	std::cout << "<Program>" << std::endl;

	{ // Manual token check.
 		if (token != 1 && token != EOF && token != 16)
		{
			error("Automata descriptions or commands expected, " + lexeme + " found.");
			exit(0);
		}
	}

	stack_adjust(1);
	std::cout << "<Automata_Descs>" << std::endl;
	while (token == 1)                               // While we're reading automata descriptions (tokens["automaton_desc"] == 1).
	{
		parse_automaton_desc();			 // Parse one.
		lex();                                   // And get the next.
	}
	stack_adjust(1);
	std::cout << "</Automata_Descs>" << std::endl;

    { // Manual token check.
		if (token != EOF && token != 16)    	
		{
			error("EOF or commands expected, " + lexeme + " found.");
			exit(0);
		}
	} 
	
	stack_adjust(1);
	std::cout << "<Commands>" << std::endl;
	while (token == 16)                               // While we're reading commands (tokens[">>"] == 16).
	{
		parse_command();                          // Parse one.
		lex();                                    // And get the next.
	}
	stack_adjust(1);
	std::cout << "</Commands>" << std::endl;
	lex();
	{ // Manual token check.
		if (token != EOF)    					
		{
			error("EOF expected, " + lexeme + " found.");
			exit(0);
		}
	}

	stack_adjust(0);
	std::cout << "</Program>" << std::endl;
}

// Parses a single automaton's description header.
void parse_automaton_desc()
{
	stack_adjust(2); 
	std::cout << "<Automaton_Desc>" << std::endl;
	
	lex();                                           // Generate the next lexeme and its token. Should be a "(" (token code 17).
	token_check(17);

	lex();                                           // Now, the lexeme 'should' have the name of this automaton in it.

	{ // Manual token check.
		if (token != NULL)
		{
			error("Unallowed identifier '" + lexeme + "'.");
			exit(0);
		}
		if (Automata[lexeme])                    // If an automaton with the given name already exists, then oops, error.
		{
			error("An automaton with the given name '" + lexeme + "' already exits.");
			exit(0);
		}
	}
	// If we're past this point, it is safe to create an automaton that the source file wants.

	auto_name = lexeme;
	Automata[auto_name] = new Automaton();
	data_type[auto_name] = "Automaton";              // An Automaton object called (auto_name) has been created.

	lex();                                           // Now, the lexeme should have ")" (token code 18).
	token_check(18);

	lex();                                           // The next lexeme we see should be a "{" (token code 3).
	token_check(3);

	parse_description();                             // Inside the braces, we have this automaton's description. We'll parse that.

	lex();                                           // The next lexeme we see should be a "}" (token code 4).
	token_check(4);

	stack_adjust(2); 
	std::cout << "</Automaton_Desc>" << std::endl;
}

// Parses a single automaton's description.
void parse_description()
{
	stack_adjust(3);
	std::cout << "<description>" << std::endl;

	lex();                                         // The next lexeme should be "sigma" (token code 2).
	token_check(2);

	lex();                                         // The next lexeme should be "=" (token code 10).
	token_check(10);

	parse_char_set();                              // Now we should have a character set (the alphabet). Parse that.

	lex();                                         // The next lexeme should be "states" (token code 13).
	token_check(13);

	lex();                                         // The next lexeme should be "=" (token code 10).
	token_check(10);

	parse_state_set();                             // Now we should have a set of states. Parse that.

	lex();                                         // The next lexeme should be "delta" (token code 8).
	token_check(8);

	lex();                                         // The next lexeme should be ":" (token code 20).
	token_check(20);

	lex();                                         // The next lexeme should be "states" (token code 13).
	token_check(13);

	lex();                                         // The next lexeme should be "x" (token code 14).
	token_check(14);

	lex();                                         // The next lexeme should be "sigma" (token code 2).
	token_check(2);

	lex();                                         // The next lexeme should be "-->" (token code 7).
	token_check(7);

	lex();                                         // The next lexeme should be "states" (token code 13).
	token_check(13);

	parse_mappings();                              // Between braces, we should have the transition function mappings. Parse those.

	lex();                                         // The next lexeme should be "accepting" (token code 9).
	token_check(9);

	lex();                                         // The next lexeme should be "=" (token code 10).
	token_check(10);

	parse_acceptingstate_set();                    // Now we should have a set of (accepting) states. Parse that.

	lex();                                         // The next lexeme should again be the name of the automaton.

	{ // Manual token check.
		if (token != NULL || lexeme != auto_name)
		{
			error("The name of the automata in the generation statement doesn't match the name in the header.");
			exit(0);
		}
	}

	lex();							// The next lexeme should be "=" (token code 10).
	token_check(10);

	lex();							// The next lexeme should be "(" (token code 17).
	token_check(17);

	lex();							// The next lexeme should be "sigma" (token code 2).
	token_check(2);

	lex();							// The next lexeme should be "," (token code 21).
	token_check(21);

	lex();							// The next lexeme should be "states" (token code 13).
	token_check(13);

	lex();							// The next lexeme should be "," (token code 21).
	token_check(21);

	lex();							// The next lexeme should be "states" (token code 13).
	token_check(13);

	lex();							// The next lexeme should be "[" (token code 5).
	token_check(5);

	lex();							// The next lexeme should be a double quote (") (token code 15).
	token_check(15);

	lex();							// We should now see the label of the initial state.

	{ // Manual error check.
		if (!Automata[auto_name]->has_state(lexeme))
		{
			error("The automaton " + auto_name + " doesn't contain a state with label " + lexeme + ".");
			exit(0);
		}
	}
	// If we're past this point, it's okay to assign the initial state.

	Automata[auto_name]->set_initial(lexeme);

	lex();							// The next lexeme should be a double quote (") (token code 15).
	token_check(15);

	lex();							// The next lexeme should be "]" (token code 6).
	token_check(6);

	lex();							// The next lexeme should be "," (token code 21).
	token_check(21);

	lex();							// The next lexeme should be "delta" (token code 8).
	token_check(8);

	lex();							// The next lexeme should be "," (token code 21).
	token_check(21);

	lex();							// The next lexeme should be "accepting" (token code 9).
	token_check(9);

	lex();							// The next lexeme should be ")" (token code 18).
	token_check(18);

	stack_adjust(3);
	std::cout << "</description>" << std::endl;
}

// Parses a set of states.
void parse_state_set()
{
	stack_adjust(4);
	std::cout << "<state_set>" << std::endl;

	lex();							// The next lexeme should be "{" (token code 3).
	token_check(3);

	lex(); 
	bool have_states = (token == tokens["state"]);

	if (token != tokens["state"] && token != tokens["}"])
	{
		error("\"state\" or \"}\" expected, \"" + lexeme + "\" found.");
		exit(0);
	}

	while (have_states)                             // While there are states in the state set, parse them.
	{
		lex();					// The next lexeme should be a "(".
		token_check(tokens["("]);

		lex();					// The next lexeme should be a double quote.
		token_check(tokens["\""]);

		lex();					// Now, we should have the label for the current state in lexeme.

		{ // Manual error check.
			if (Automata[auto_name]->has_state(lexeme))
			{
				error("A state with label \"" + lexeme + "\" already exists.");
				exit(0);
			}
		} // If we're past this point, it's okay to add this state to the automaton.
		Automata[auto_name]->add_state(lexeme);
		
		lex();						// The next lexeme should be a double quote.
		token_check(tokens["\""]);

		lex();						// The next lexeme should be a ")".
		token_check(tokens[")"]);

		lex();						// The next lexeme may or may not be ",".
		// Manual token check.
		if (token != tokens[","] && token != tokens["}"])
		{
			error("\",\" or \"}\" expected, \"" + lexeme + "\" found.");
			exit(0);
		}
		else if (token == tokens["}"])	// Done with the states.
		{
			have_states = false;
		}
		else if (token == tokens[","])	// Nope, more left.
		{
			lex();
			token_check(tokens["state"]);	// The next lexeme should be "state".
		}
	}
	stack_adjust(4);
	std::cout << "</state_set>" << std::endl;
}

// Parses a set of states, with the additional info that they're accepting.
void parse_acceptingstate_set()		
{
	stack_adjust(4);
	std::cout << "<acceptingstate_set>" << std::endl;

	lex();							// The next lexeme should be "{" (token code 3).
	token_check(3);

	lex();
	bool have_states = (token == tokens["states"]);
	
	if (token != tokens["states"] && token != tokens["}"])
	{
		error("\"states\" or \"}\" expected, \"" + lexeme + "\" found.");
		exit(0);
	}

	while (have_states)                                 // While there are states in the acceptingstate set, parse them.
	{
		lex();					    // The next lexeme should be a "(".
		token_check(tokens["["]);

		lex();					    // The next lexeme should be a double quote.
		token_check(tokens["\""]);

		lex();					    // Now, we should have the label for the current state in lexeme.

		{ // Manual error check.
			if (!Automata[auto_name]->has_state(lexeme))
			{
				error("A state with label \"" + lexeme + "\" doesn't exist and hence cannot be accepting.");
				exit(0);
			}
		} // If we're past this point, it's okay to add this state to the automaton.

		Automata[auto_name]->add_acceptingstate(lexeme);

		lex();                                  // The next lexeme should be a double quote.
		token_check(tokens["\""]);

		lex();                                  // The next lexeme should be a ")".
		token_check(tokens["]"]);

		lex();                                  // The next lexeme may or may not be ",".
		// Manual token check.
		if (token != tokens[","] && token != tokens["}"])
		{
			error("\",\" or \"}\" expected, \"" + lexeme + "\" found.");
			exit(0);
		}
		else if (token == tokens["}"])          // Done with the states.
		{
			have_states = false;
		}
		else if (token == tokens[","])          // Nope, more left.
		{
			lex();
			token_check(tokens["states"]);  // The next lexeme should be "state".
		}
	}

	stack_adjust(4);
	std::cout << "</acceptingstate_set>" << std::endl;
}

// Parses a set of characters.
void parse_char_set() 
{
	stack_adjust(4);
	std::cout << "<char_set>" << std::endl;

	lex();						// The next lexeme should be "{" (token code 3).
	token_check(3);

	lex();
	bool have_chars = (token == tokens["'"]);

	if (token != tokens["'"] && token != tokens["}"])
	{
		error("\"'\" or \"}\" expected, \"" + lexeme + "\" found.");
		exit(0);
	}

	while (have_chars)                               // While we have characters to add to the alphabet.
	{
		lex();                                   // Now, we should have a character of sigma in the lexeme.
		if (lexeme.size() != 1)
		{
			error("\"sigma\" can only contain characters (multiple characters found in an element)");
			exit(0);
		}
		Automata[auto_name]->add_char(lexeme.at(0));
		
		lex();                                      // The next lexeme should be "'".
		token_check(tokens["'"]);

		lex();                                      // The next lexeme may or may not be ",".

		// Manual token check.
		if (token != tokens[","] && token != tokens["}"])
		{
			error("\",\" or \"}\" expected, \"" + lexeme + "\" found.");
			exit(0);
		}
		else if (token == tokens["}"])             // Done with the states.
		{
			have_chars = false;
		}
		else if (token == tokens[","])             // Nope, more left.
		{
			lex();
			token_check(tokens["'"]);          // The next lexeme should be "'".
		}
	}

	stack_adjust(4);
	std::cout << "</char_set>" << std::endl;
}

// Parse the transition function mappings.
void parse_mappings()
{
	stack_adjust(4);
	std::cout << "<mappings>" << std::endl;

	lex();                                           // The next lexeme should be "{".
	token_check(tokens["{"]);

	lex();
	bool have_mappings = (token == tokens["("]);

	while (have_mappings)                            // While we have mappings, parse them.
	{
		lex();                                   // The next lexeme should be "\"".
		token_check(tokens["\""]);

		lex();                                   // We should now have the label of the starting state in lexeme.

		{ // Manual error check.
			if (!Automata[auto_name]->has_state(lexeme))
			{
				error("There doesn't exist a state labelled \"" + lexeme + "\" and hence a mapping from it cannot be created.");
				exit(0);
			}
		} // If we're past this point, a mapping from states[lexeme] can be created.

		std::string from_state = lexeme;

		lex();							// The next lexeme should be "\"".
		token_check(tokens["\""]);

		lex();							// The next lexeme should be ",".
		token_check(tokens[","]);

		lex();							// The next lexeme should be "'".
		token_check(tokens["'"]);

		lex();							// We should now have the input character in lexeme.

		{ // Manual error check.
			if (lexeme.size() != 1)
			{
				error("\"sigma\" can only contain characters (multiple characters found in an element)");
				exit(0);
			}
			if (!Automata[auto_name]->has_char(lexeme.at(0)))
			{
				error("There doesn't exist a character '" + lexeme + "' and hence a mapping under it cannot be created.");
				exit(0);
			}
		} // If we're past this point, a mapping under the character 'lexeme' can be created.

		char map_under = lexeme.at(0);  // Since it's still stored as a string.

		lex();							// The next lexeme should be "'".
		token_check(tokens["'"]);

		lex();							// The next lexeme should be ")".
		token_check(tokens[")"]);

		lex();							// The next lexeme should be "-->".
		token_check(tokens["-->"]);

		lex();							// The next lexeme should be "\"".
		token_check(tokens["\""]);

		lex();							// We should now have the label of the resulting state in lexeme.

		{ // Manual error check.
			if (!Automata[auto_name]->has_state(lexeme))
			{
				error("There doesn't exist a state labelled \"" + lexeme + "\" and hence a mapping to it cannot be created.");
				exit(0);
			}
		} // If we're past this point, mapping to states[lexeme] can be created.

		Automata[auto_name]->add_transition(from_state, map_under, lexeme/*to_state*/);

		lex();							// The next lexeme should be "\"".
		token_check(tokens["\""]);

		lex();							// The next lexeme may or may not be "(".

		// Manual token check.
		if (token != tokens["("] && token != tokens["}"])
		{
			error("\"(\" or \"}\" expected, " + lexeme + " found.");
			exit(0);
		}
		else if (token == tokens["}"])	// Done with the states.
		{
			have_mappings = false;
		}
		else if (token == tokens["("]);	// Nope, more left.
	}

	stack_adjust(4);
	std::cout << "</mappings>" << std::endl;
}

// Parses a single command.
void parse_command()
{
	lex();                                         // Get the next lexeme. Should be the name of an automaton.
	if (data_type[lexeme] != "Automaton") {
		error("There doesn't exist an automaton called " + lexeme + ".");
		exit(0);
	}
	// If we're past this point, it's okay to run a query on the specified automaton.

	std::string auto_name = lexeme;                // Chill guys, lexical scoping will take care of that. ;)

	lex();                                         // The next lexeme should be a period (".").
	token_check(tokens["."]);

	lex();                                         // The next lexeme should be "accepts".
	token_check(tokens["accepts"]);
	
	lex();                                         // The next lexeme should be "(".
	token_check(tokens["("]);

	lex();                                         // The next lexeme should be "\"".
	token_check(tokens["\""]);

	lex();                                         // Get the next lexeme. This should be the query string.
	
	std::cout << ">> " << Automata[auto_name]->accepts(lexeme) << std::endl;

	lex();
	token_check(tokens["\""]);

	lex();
	token_check(tokens[")"]);
}

// Generates the next lexeme and its token.
void lex() 
{
	lexeme = "";
	getnonblank();
	switch (char_class)
	{
	case CHAR:
		/* Parsing a keyword or an automaton's name. */
		lexeme += character;
		getcharacter();
		while (char_class == CHAR)
		{
			lexeme += character;
			getcharacter();
		}
		token = tokens[lexeme];				// The token will be NULL if it is not a keyword.
		break;
	case SYMBOL:
		/* Parsing other things. */
		lexeme += character;	
		if (character == '-' || character == '>') 
		{						// All symbolic lexemes are one-character long, except "-->" and ">>".
			getcharacter();
			while (char_class == SYMBOL)
			{
				lexeme += character;
				getcharacter();
			}
		}
		else getcharacter();
		token = tokens[lexeme];
		break;
	case EOF: // EOF defined to be -1.
		lexeme = "EOF";
		token = EOF;
		break;
	}
}

// Gets the next non-blank character from the source file.
void getnonblank()
{
	while (isspace(character))				// As long as we're reading whitespace ...
		getcharacter();					// ... ignore and get the next character.
}

// Gets a single character from the source file.
void getcharacter()
{
	if (prog.eof())
	{
		char_class = EOF;
		return;
	}
	prog.get(character);                              // Every alpha-numeric symbol is treated as a character in AL.

	if (character == '#')                             // Comment found.
	{
		while (character != '\n' && !prog.eof())      // Skip comment.
			prog.get(character);

		if (prog.eof())
		{
			char_class = EOF;
			return;
		}
	}

	if (isalnum(character) || character == '_')
		char_class = CHAR;

	else if (isspace(character)) 
	{
		char_class = SPACE;
		if (character == '\n')
			line_num++;
	}

	else
		char_class = SYMBOL;
}

void error(const std::string& message)
{
	std::cout << "error: Line " << line_num << " - " << message << std::endl;
}

void token_check(int expected_token)
{
	if (token != expected_token)
	{
		std::string message = "\"";
		message += lexemes[expected_token];
		message += "\" expected, \"";
		message += lexeme;
		message += "\" found.";
		error(message);
		exit(0);
	}
}
