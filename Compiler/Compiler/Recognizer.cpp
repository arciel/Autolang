/*
 * A syntax analyzer and compiler for the Automata Language.
 * Experimental, work in progress. 
 * 
 * - Tushar
 */

#include <iostream>
#include <fstream>
#include "Automaton.h"

// The list of keywords in the Automata Language.
std::string keywords[] { "sigma", "states", "delta", "automata_desc", "accepting", "state" };

// The lexeme classes and their corresponding tokens.
std::unordered_map<std::string, int> tokens
{
	{ "automata_desc", 1 }, { "sigma", 2 },
	{ "{", 3 }, { "}", 4 }, { "[", 5 }, { "]", 6 },
	{ "-->", 7 }, { "delta", 8 }, { "accepting", 9 },
	{ "=", 10 }, { "'", 11 }, { "state", 12 }, { "states", 13 },
	{ "x", 14 }, { "\"", 15 }, { ">>", 16 } 
};

std::string lexeme;		// The current lexeme under inspection.
int token;				// The token for the current lexeme.

void lex();

void getnonblank();		//

int main(int argc, char *argv[])
{
	std::fstream prog;	
	prog.open((argc > 1) ? argv[1] : "program.al");		// Source code to be compiled.
	
}