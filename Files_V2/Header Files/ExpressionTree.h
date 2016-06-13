#ifndef EXPRESSION_TREE_H            
#define EXPRESSION_TREE_H            
                                     
#include "ProgramVars.h"
#include "Auto.h"
#include <vector>
                                     
using std::string;
using std::vector;

enum Token_type 
{ 
	INT_LIT, LOGICAL_LIT, CHAR_LIT, STRING_LIT, SET_LIT, TUPLE_LIT, LITERAL,
	INDEX, IDENTIFIER, INT_OP, CHAR_OP, SET_OP, COPY_OP, OP, UNARY,
	LOGICAL_OP, STRING_OP, TUPLE_OP, MAP_OP, AUTO_OP, END, ERROR, EXPR
};


class Token
{
public:
	string lexeme;
	vector<Token_type> types;
	string to_string();
};

class ExpressionTree;

class Node
{
public:
	Token token; 
	Elem * value;			// Every node in the expression tree will have a value based on its token.
	bool operator_node;		// To be set to true if the lexeme in the unit is an OP.
	ExpressionTree *left, *right;

	Node() { left = nullptr; right = nullptr; operator_node = false; value = nullptr; }

	Elem * parse_literal();		// Parses the token.lexeme to get a value, if the lexeme is a literal.
	Elem * evaluate();		// Evaluates an expression.
	~Node()
	{
		if (left != nullptr) delete left; 
		if (right != nullptr) delete right;
		if (/*token.types[0] == LITERAL &&*/ value != nullptr) delete value;
	}
};

class ExpressionTree
{
public:
	string expr;			// The expression that the tree is going to parse.
	int current_index;		// The current index into the expression_string.
	Node * root;			// The root tree of the expression.

	void skip_whitespace();		// Skips whitespace in the expression.

	ExpressionTree(string &);	// Construct the tree given the expression.
	Elem * evaluate();		// Will evaluate the expression and return the result (Elem * in the root).
	Token get_next_token();		// The lexical analyzer (lexer) for the expression.
	~ExpressionTree(){delete root;} // Will think about this later.
};


#endif 