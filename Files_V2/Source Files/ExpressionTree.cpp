#include "../Header Files/ExpressionTree.h"
#include <iostream>
#include <cmath>

/* Implementations for methods in the classes Token, Node, and ExpressionTree. */

// -----------------------------------------------------<CLASS TOKEN>------------------------------------------//

namespace program_vars 
{
	std::unordered_map<string, Elem *> * identify = new std::unordered_map<string, Elem *>();
}

string token_name[] = 
{
	"INT_LIT", "LOGICAL_LIT", "CHAR_LIT", "STRING_LIT", "SET_LIT", "TUPLE_LIT", "LITERAL",
	"INDEX", "IDENTIFIER", "INT_OP", "CHAR_OP", "SET_OP", "COPY_OP", "OP", "UNARY",
	"LOGICAL_OP", "STRING_OP", "TUPLE_OP", "MAP_OP", "AUTO_OP", "END", "ERROR", "EXPR" 
};

string Token::to_string()
{
	string rep = "TOKEN(" + lexeme + ", {";
	for (auto &type : types)
		rep += token_name[type] + ", ";
	rep += "})";
	return rep;
}

// -----------------------------------------------------</CLASS TOKEN>------------------------------------------//


// ------------------------------------------------------<CLASS NODE>-------------------------------------------//

Elem * Node::parse_literal()		// Parses the token.lexeme to get a value, if the lexeme is a literal.
{
	if (this->token.types[0] != LITERAL) 
		return nullptr;		// If the token is not a literal, ignore it.
	
	if (this->token.types[1] == INT_LIT)			
		return new Int(token.lexeme);			

	if (this->token.types[1] == CHAR_LIT)			
		return new Char(token.lexeme);

	if (this->token.types[1] == LOGICAL_LIT)
		return new Logical(token.lexeme);
		
	if (this->token.types[1] == STRING_LIT)
		return new String(token.lexeme, 0);		// The 0 means that the string being passed is a representation of the object.
								// (as opposed to its value).
	if (this->token.types[1] == SET_LIT)
	{
		return new Set(token.lexeme);
	}
	if (this->token.types[1] == TUPLE_LIT)
		return new Tuple(token.lexeme);

	return nullptr;
}


// -----------------------------------------------------</CLASS NODE>-------------------------------------------//


// -------------------------------------------------<CLASS EXPRESSIONTREE>--------------------------------------//

void ExpressionTree::skip_whitespace()
{								
	while ((current_index < expr.size()) && (isspace(expr[current_index])))	
	{
		current_index++;				// The char will most definitely NOT be a '\n' character.
	}
}

Elem * ExpressionTree::evaluate()
{
	if (root->value != nullptr) 
		return root->value;				// Will be triggered in case of literals and identifiers.
	
	if (root->token.types[0] == OP)				// If the root is an op.
	{
		if (root->token.types[1] == UNARY)
		{
			if (root->token.lexeme == ".")				// Deep_copy operator.
			{
				root->value = root->left->evaluate()->deep_copy();	// Get the deep_copy of the value of the left tree.
			}
			else if (root->token.lexeme == "()")
			{
				root->value = root->left->evaluate();
			}
			else if (root->token.lexeme == "|")
			{
				Elem * get_size_of = root->left->evaluate();
				if (get_size_of->type == SET)
				{
					Set * get_size_of_set = (Set *)get_size_of;
					root->value = new Int(get_size_of_set->cardinality());
				}
				else if (get_size_of->type == TUPLE)
				{
					Tuple * get_size_of_tuple = (Tuple *)get_size_of;
					root->value = new Int(get_size_of_tuple->size());
				}
			}
			else if (root->token.lexeme == "!")
			{
				Logical * negate = (Logical *)root->left->evaluate();
				root->value = new Logical(!negate->elem);
			}
		}
		else
		{
			if (root->token.lexeme == "V")
			{
				Elem * left = root->left->evaluate();
				Elem * right = root->right->evaluate();
				if (left->type == INT)
				{
					Int * l_int = (Int *)left;
					if (right->type == INT)
					{
						Int *r_int = (Int *)right;
						root->value = new Int(l_int->elem || r_int->elem);
					}
					else if (right->type == LOGICAL)
					{
						Logical *r_logical = (Logical *)right;
						root->value = new Int(l_int->elem || r_logical->elem);
					}
					else if (right->type == CHAR)
					{
						Char *r_char = (Char *)right;
						root->value = new Int(l_int->elem || r_char->elem);
					}
				}
				else if (left->type == LOGICAL)
				{
					Logical * l_int = (Logical *)left;
					if (right->type == INT)
					{
						Int *r_int = (Int *)right;
						root->value = new Logical(l_int->elem || r_int->elem);
					}
					else if (right->type == LOGICAL)
					{
						Logical *r_logical = (Logical *)right;
						root->value = new Logical(l_int->elem || r_logical->elem);
					}
					else if (right->type == CHAR)
					{
						Char *r_char = (Char *)right;
						root->value = new Logical(l_int->elem || r_char->elem);
					}
				}
				else if (left->type == CHAR)
				{
					Char * l_int = (Char *)left;
					if (right->type == INT)
					{
						Int *r_int = (Int *)right;
						root->value = new Char(l_int->elem || r_int->elem);
					}
					else if (right->type == LOGICAL)
					{
						Logical *r_logical = (Logical *)right;
						root->value = new Char(l_int->elem || r_logical->elem);
					}
					else if (right->type == CHAR)
					{
						Char *r_char = (Char *)right;
						root->value = new Char(l_int->elem || r_char->elem);
					}
				}
			}
			else if (root->token.lexeme == "&")
			{
				Elem * left = root->left->evaluate();
				Elem * right = root->right->evaluate();
				if (left->type == SET && right->type == SET)
				{
					Set * l_set = (Set *)left, *r_set = (Set *)right;
					root->value = l_set->intersection(*r_set);
				}
				else if (left->type == INT)
				{
					Int * l_int = (Int *)left;
					if (right->type == INT)
					{
						Int *r_int = (Int *)right;
						root->value = new Int(l_int->elem && r_int->elem);
					}
					else if (right->type == LOGICAL)
					{
						Logical *r_logical = (Logical *)right;
						root->value = new Int(l_int->elem && r_logical->elem);
					}
					else if (right->type == CHAR)
					{
						Char *r_char = (Char *)right;
						root->value = new Int(l_int->elem && r_char->elem);
					}
				}
				else if (left->type == LOGICAL)
				{
					Logical * l_int = (Logical *)left;
					if (right->type == INT)
					{
						Int *r_int = (Int *)right;
						root->value = new Logical(l_int->elem && r_int->elem);
					}
					else if (right->type == LOGICAL)
					{
						Logical *r_logical = (Logical *)right;
						root->value = new Logical(l_int->elem && r_logical->elem);
					}
					else if (right->type == CHAR)
					{
						Char *r_char = (Char *)right;
						root->value = new Logical(l_int->elem && r_char->elem);
					}
				}
				else if (left->type == CHAR)
				{
					Char * l_int = (Char *)left;
					if (right->type == INT)
					{
						Int *r_int = (Int *)right;
						root->value = new Char(l_int->elem && r_int->elem);
					}
					else if (right->type == LOGICAL)
					{
						Logical *r_logical = (Logical *)right;
						root->value = new Char(l_int->elem && r_logical->elem);
					}
					else if (right->type == CHAR)
					{
						Char *r_char = (Char *)right;
						root->value = new Char(l_int->elem && r_char->elem);
					}
				}
			}
			else if (root->token.lexeme == "==")
			{
				Elem * left = root->left->evaluate();
				Elem * right = root->right->evaluate();
				root->value = new Logical(*left == *right);
			}
			else if (root->token.lexeme == "<")
			{
				Elem * left = root->left->evaluate();
				Elem * right = root->right->evaluate();
				root->value = new Logical(*left < *right);
			}
			else if (root->token.lexeme == "<=")
			{
				Elem * left = root->left->evaluate();
				Elem * right = root->right->evaluate();
				root->value = new Logical((*left < *right) || (*left == *right));
			}
			else if (root->token.lexeme == ">")
			{
				Elem * left = root->left->evaluate();
				Elem * right = root->right->evaluate();
				root->value = new Logical(!(*left < *right) || (*left == *right));
			}
			else if (root->token.lexeme == ">=")
			{
				Elem * left = root->left->evaluate();
				Elem * right = root->right->evaluate();
				root->value = new Logical(!(*left < *right));
			}
			else if (root->token.lexeme == "o")
			{
				Map * f = (Map *) root->left->evaluate();
				Map * g = (Map *)root->right->evaluate();
				root->value = f->composed_with(*g);
			}
			else if (root->token.lexeme == "c")
			{
				Set * left  = (Set *) root->left->evaluate();
				Set * right = (Set *)root->right->evaluate();
				root->value = new Logical(left->subset_of(*right));
			}
			else if (root->token.lexeme == "x")
			{
				Set * left = (Set *)root->left->evaluate();
				Set * right = (Set *)root->right->evaluate();
				root->value = left->cartesian_product(*right);
			}
			else if (root->token.lexeme == "U")
			{
				Set * left = (Set *)root->left->evaluate();
				Set * right = (Set *)root->right->evaluate();
				root->value = left->_union(*right);
			}
			else if (root->token.lexeme == "\\")
			{
				Set * left = (Set *)root->left->evaluate();
				Set * right = (Set *)root->right->evaluate();
				root->value = left->exclusion(*right);
			}
			else if (root->token.lexeme == "[]")
			{
				Elem * elem = root->left->evaluate();
				Elem * query = root->right->evaluate();
				if (elem->type == SET && query->type == INT)
				{
					Set * e = (Set *)elem;
					Int * q = (Int *)query;
					root->value = (*e)[q->elem];
				}
				else if (elem->type == TUPLE && query->type == INT)
				{
					Tuple * e = (Tuple *)elem;
					Int * q = (Int *)query;
					root->value = (*e)[q->elem];
				}
				else if (elem->type == MAP)
				{
					Map * map = (Map *)elem;
					root->value = (*map)[*query];
				}
				else if (elem->type == AUTO && query->type == STRING)
				{
					Auto * auto_ = (Auto *)elem;
					String * q = (String *)query;
					root->value = (*auto_)[*q];
				}
			}
			else if (root->token.lexeme == "+")
			{
				Elem * left = root->left->evaluate();
				Elem * right = root->right->evaluate();
				if (left->type == INT)
				{
					Int * l_int = (Int *)left;
					if (right->type == INT)
					{
						Int *r_int = (Int *)right;
						root->value = new Int(l_int->elem + r_int->elem);
					}
					else if (right->type == LOGICAL)
					{
						Logical *r_logical = (Logical *)right;
						root->value = new Int(l_int->elem + r_logical->elem);
					}
					else if (right->type == CHAR)
					{
						Char *r_char = (Char *)right;
						root->value = new Int(l_int->elem + r_char->elem);
					}
				}
				else if (left->type == LOGICAL)
				{
					Logical * l_int = (Logical *)left;
					if (right->type == INT)
					{
						Int *r_int = (Int *)right;
						root->value = new Logical(l_int->elem + r_int->elem);
					}
					else if (right->type == LOGICAL)
					{
						Logical *r_logical = (Logical *)right;
						root->value = new Logical(l_int->elem + r_logical->elem);
					}
					else if (right->type == CHAR)
					{
						Char *r_char = (Char *)right;
						root->value = new Logical(l_int->elem + r_char->elem);
					}
				}
				else if (left->type == CHAR)
				{
					Char * l_int = (Char *)left;
					if (right->type == INT)
					{
						Int *r_int = (Int *)right;
						root->value = new Char(l_int->elem + r_int->elem);
					}
					else if (right->type == LOGICAL)
					{
						Logical *r_logical = (Logical *)right;
						root->value = new Char(l_int->elem + r_logical->elem);
					}
					else if (right->type == CHAR)
					{
						Char *r_char = (Char *)right;
						root->value = new Char(l_int->elem + r_char->elem);
					}
				}
			}
			else if (root->token.lexeme == "-")
			{
				Elem * left = root->left->evaluate();
				Elem * right = root->right->evaluate();
				if (left->type == INT)
				{
					Int * l_int = (Int *)left;
					if (right->type == INT)
					{
						Int *r_int = (Int *)right;
						root->value = new Int(l_int->elem - r_int->elem);
					}
					else if (right->type == LOGICAL)
					{
						Logical *r_logical = (Logical *)right;
						root->value = new Int(l_int->elem - r_logical->elem);
					}
					else if (right->type == CHAR)
					{
						Char *r_char = (Char *)right;
						root->value = new Int(l_int->elem - r_char->elem);
					}
				}
				else if (left->type == LOGICAL)
				{
					Logical * l_int = (Logical *)left;
					if (right->type == INT)
					{
						Int *r_int = (Int *)right;
						root->value = new Logical(l_int->elem - r_int->elem);
					}
					else if (right->type == LOGICAL)
					{
						Logical *r_logical = (Logical *)right;
						root->value = new Logical(l_int->elem - r_logical->elem);
					}
					else if (right->type == CHAR)
					{
						Char *r_char = (Char *)right;
						root->value = new Logical(l_int->elem - r_char->elem);
					}
				}
				else if (left->type == CHAR)
				{
					Char * l_int = (Char *)left;
					if (right->type == INT)
					{
						Int *r_int = (Int *)right;
						root->value = new Char(l_int->elem - r_int->elem);
					}
					else if (right->type == LOGICAL)
					{
						Logical *r_logical = (Logical *)right;
						root->value = new Char(l_int->elem - r_logical->elem);
					}
					else if (right->type == CHAR)
					{
						Char *r_char = (Char *)right;
						root->value = new Char(l_int->elem - r_char->elem);
					}
				}
			}
			else if (root->token.lexeme == "*")
			{
				Elem * left = root->left->evaluate();
				Elem * right = root->right->evaluate();
				if (left->type == INT)
				{
					Int * l_int = (Int *)left;
					if (right->type == INT)
					{
						Int *r_int = (Int *)right;
						root->value = new Int(l_int->elem * r_int->elem);
					}
					else if (right->type == LOGICAL)
					{
						Logical *r_logical = (Logical *)right;
						root->value = new Int(l_int->elem * r_logical->elem);
					}
					else if (right->type == CHAR)
					{
						Char *r_char = (Char *)right;
						root->value = new Int(l_int->elem * r_char->elem);
					}
				}
				else if (left->type == LOGICAL)
				{
					Logical * l_int = (Logical *)left;
					if (right->type == INT)
					{
						Int *r_int = (Int *)right;
						root->value = new Logical(l_int->elem * r_int->elem);
					}
					else if (right->type == LOGICAL)
					{
						Logical *r_logical = (Logical *)right;
						root->value = new Logical(l_int->elem * r_logical->elem);
					}
					else if (right->type == CHAR)
					{
						Char *r_char = (Char *)right;
						root->value = new Logical(l_int->elem * r_char->elem);
					}
				}
				else if (left->type == CHAR)
				{
					Char * l_int = (Char *)left;
					if (right->type == INT)
					{
						Int *r_int = (Int *)right;
						root->value = new Char(l_int->elem * r_int->elem);
					}
					else if (right->type == LOGICAL)
					{
						Logical *r_logical = (Logical *)right;
						root->value = new Char(l_int->elem * r_logical->elem);
					}
					else if (right->type == CHAR)
					{
						Char *r_char = (Char *)right;
						root->value = new Char(l_int->elem * r_char->elem);
					}
				}
			}
			else if (root->token.lexeme == "/")
			{
				Elem * left = root->left->evaluate();
				Elem * right = root->right->evaluate();
				if (left->type == INT)
				{
					Int * l_int = (Int *)left;
					if (right->type == INT)
					{
						Int *r_int = (Int *)right;
						root->value = new Int(l_int->elem / r_int->elem);
					}
					else if (right->type == LOGICAL)
					{
						Logical *r_logical = (Logical *)right;
						root->value = new Int(l_int->elem / r_logical->elem);
					}
					else if (right->type == CHAR)
					{
						Char *r_char = (Char *)right;
						root->value = new Int(l_int->elem / r_char->elem);
					}
				}
				else if (left->type == LOGICAL)
				{
					Logical * l_int = (Logical *)left;
					if (right->type == INT)
					{
						Int *r_int = (Int *)right;
						root->value = new Logical(l_int->elem / r_int->elem);
					}
					else if (right->type == LOGICAL)
					{
						Logical *r_logical = (Logical *)right;
						root->value = new Logical(l_int->elem / r_logical->elem);
					}
					else if (right->type == CHAR)
					{
						Char *r_char = (Char *)right;
						root->value = new Logical(l_int->elem / r_char->elem);
					}
				}
				else if (left->type == CHAR)
				{
					Char * l_int = (Char *)left;
					if (right->type == INT)
					{
						Int *r_int = (Int *)right;
						root->value = new Char(l_int->elem / r_int->elem);
					}
					else if (right->type == LOGICAL)
					{
						Logical *r_logical = (Logical *)right;
						root->value = new Char(l_int->elem / r_logical->elem);
					}
					else if (right->type == CHAR)
					{
						Char *r_char = (Char *)right;
						root->value = new Char(l_int->elem / r_char->elem);
					}
				}
			}
			else if (root->token.lexeme == "^")
			{
				Elem * left = root->left->evaluate();
				Elem * right = root->right->evaluate();
				if (left->type == INT)
				{
					Int * l_int = (Int *)left;
					if (right->type == INT)
					{
						Int *r_int = (Int *)right;
						root->value = new Int(pow(l_int->elem, r_int->elem));
					}
					else if (right->type == LOGICAL)
					{
						Logical *r_logical = (Logical *)right;
						root->value = new Int(pow(l_int->elem, r_logical->elem));
					}
					else if (right->type == CHAR)
					{
						Char *r_char = (Char *)right;
						root->value = new Int(pow(l_int->elem, r_char->elem));
					}
				}
				else if (left->type == LOGICAL)
				{
					Logical * l_int = (Logical *)left;
					if (right->type == INT)
					{
						Int *r_int = (Int *)right;
						root->value = new Logical(pow(l_int->elem, r_int->elem));
					}
					else if (right->type == LOGICAL)
					{
						Logical *r_logical = (Logical *)right;
						root->value = new Logical(pow(l_int->elem, r_logical->elem));
					}
					else if (right->type == CHAR)
					{
						Char *r_char = (Char *)right;
						root->value = new Logical(pow(l_int->elem, r_char->elem));
					}
				}
				else if (left->type == CHAR)
				{
					Char * l_int = (Char *)left;
					if (right->type == INT)
					{
						Int *r_int = (Int *)right;
						root->value = new Char(pow(l_int->elem, r_int->elem));
					}
					else if (right->type == LOGICAL)
					{
						Logical *r_logical = (Logical *)right;
						root->value = new Char(pow(l_int->elem, r_logical->elem));
					}
					else if (right->type == CHAR)
					{
						Char *r_char = (Char *)right;
						root->value = new Char(pow(l_int->elem, r_char->elem));
					}
				}
			}
			else if (root->token.lexeme == "%")
			{
				Elem * left = root->left->evaluate();
				Elem * right = root->right->evaluate();
				if (left->type == INT)
				{
					Int * l_int = (Int *)left;
					if (right->type == INT)
					{
						Int *r_int = (Int *)right;
						root->value = new Int(l_int->elem % r_int->elem);
					}
					else if (right->type == LOGICAL)
					{
						Logical *r_logical = (Logical *)right;
						root->value = new Int(l_int->elem % r_logical->elem);
					}
					else if (right->type == CHAR)
					{
						Char *r_char = (Char *)right;
						root->value = new Int(l_int->elem % r_char->elem);
					}
				}
				else if (left->type == LOGICAL)
				{
					Logical * l_int = (Logical *)left;
					if (right->type == INT)
					{
						Int *r_int = (Int *)right;
						root->value = new Logical(l_int->elem % r_int->elem);
					}
					else if (right->type == LOGICAL)
					{
						Logical *r_logical = (Logical *)right;
						root->value = new Logical(l_int->elem % r_logical->elem);
					}
					else if (right->type == CHAR)
					{
						Char *r_char = (Char *)right;
						root->value = new Logical(l_int->elem % r_char->elem);
					}
				}
				else if (left->type == CHAR)
				{
					Char * l_int = (Char *)left;
					if (right->type == INT)
					{
						Int *r_int = (Int *)right;
						root->value = new Char(l_int->elem % r_int->elem);
					}
					else if (right->type == LOGICAL)
					{
						Logical *r_logical = (Logical *)right;
						root->value = new Char(l_int->elem % r_logical->elem);
					}
					else if (right->type == CHAR)
					{
						Char *r_char = (Char *)right;
						root->value = new Char(l_int->elem % r_char->elem);
					}
				}
			}
		}
	}
	return root->value;
}

Token ExpressionTree::get_next_token()
{
	skip_whitespace();					// Skip whitespace, of course.

	if (current_index >= expr.size())			// If the current_index is now beyond the expr's length ...
		return{ "", {END} };				// ... return a token initializer list with a blank lexeme, and type END.

	else if (expr[current_index] == '|')			// Cardinality op.
	{
		current_index++;
		return{ "|", {OP, UNARY, SET_OP, STRING_OP, TUPLE_OP} };
	}

	else if (expr[current_index] == '.')
	{
		current_index++;
		return{ ".", { OP, UNARY, COPY_OP } };
	}

	//-----------------------------------------<LOGICAL OPS>-----------------------------------------------//

	else if (expr[current_index] == '!')
	{
		current_index++;
		return{ "!", { OP, UNARY, LOGICAL_OP } };
	}

	else if (expr[current_index] == 'V')
	{
		current_index++;
		return{ "V", { OP, LOGICAL_OP } };
	}

	else if (expr[current_index] == '=' && ((current_index + 1) < expr.size()) && expr[current_index + 1] == '=')
	{
		current_index += 2;
		return{ "==", { OP, LOGICAL_OP } };
	}

	//----------------------------------------</LOGICAL OPS>-----------------------------------------------//

	//----------------------------------------------<MAP OP>-----------------------------------------------//

	else if (expr[current_index] == 'o')			// Map compose op.
	{
		current_index++;
		return{ "o", { OP, MAP_OP } };
	}

	//---------------------------------------------</MAP OP>-----------------------------------------------//

	//----------------------------------------------<SET OP>-----------------------------------------------//

	else if (expr[current_index] == 'c')			// Subset op.
	{
		current_index++; return{ "c", { OP, SET_OP } };
	}
	else if (expr[current_index] == 'x')			// Cartesian product op.
	{
		current_index++; return{ "x", { OP, SET_OP } };
	}
	else if (expr[current_index] == 'U')			// Union op.
	{
		current_index++; return{ "U", { OP, SET_OP, AUTO_OP } };
	}
	else if (expr[current_index] == '&')			// Intersection (AND) op.
	{
		current_index++; return{ "&", { OP, SET_OP, AUTO_OP, LOGICAL_OP } };
	}
	else if (expr[current_index] == '\\')			// Exclusion op.
	{
		current_index++; return{ "\\", { OP, SET_OP, AUTO_OP } };
	}

	//---------------------------------------------</SET OP>----------------------------------------------//

	//--------------------------------------------<LITERALS>----------------------------------------------//
	
	else if (isdigit(expr[current_index]))				// Int literal.
	{                                         
		int i = current_index + 1;				// Start looking one space ahead of the current_index ...
		while (i < expr.size() && isdigit(expr[i]))		// ... and while you're finding more digits ...
			i++;						// ... keep looking.
		int j = current_index;					// Store the current_index, because it is about to be updated.
		current_index = i;					// Update the current index.
		return{ expr.substr(j, i-j), { LITERAL, INT_LIT} };     // Return a token initializer list with the int_literal lexeme.
	} 		                          
	
	else if (expr[current_index] == 'T' || expr[current_index] == 'F') // Logical Literals
	{
		if (expr[current_index] == 'T') {
			if (current_index + 3 < expr.size() &&	// Because if expr[i] == 'T', expr[i + 3] == 'e' for a True logical literal.
		           (expr.substr(current_index, 4) == "True"))
			{
				current_index += 4;
				return{ "True", { LITERAL, LOGICAL_LIT} };
			}
		}
		else if (expr[current_index] == 'F') {
			if (current_index + 4 < expr.size() &&  // Because if expr[i] == 'F', expr[i + 4] == 'e' for a False logical literal.
				(expr.substr(current_index, 5) == "False"))
			{
				current_index += 5;
				return{ "False", { LITERAL, LOGICAL_LIT} };
			}
		}									
	}
	else if (expr[current_index] == '\'')			// Char literals.
	{
		if (current_index + 2 < expr.size() && expr[current_index + 2] == '\'')
		{
			int j = current_index;
			current_index += 3;
			return{ expr.substr(j, 3), { LITERAL, CHAR_LIT } };
		}
		else if (current_index + 3 < expr.size() && expr[current_index + 1] == '\\' && expr[current_index + 3] == '\'')
		{
			int j = current_index;		
			current_index += 4;	
			return{ expr.substr(j, 4), { LITERAL, CHAR_LIT } };
		}
		else return{ "", {ERROR} };
	}
	else if (expr[current_index] == '"')			// String literals.
	{
		int i;
		for (i = current_index + 1; i < expr.size(); i++)	// We'll look for the closing quote.
			if (expr[i] == '"' && expr[i - 1] != '\\')	// When we find a closing quote that is not preceded by an escape '\',
				break;					// break.
		int j = current_index;
		current_index = i + 1;
		return{ expr.substr(j, current_index-j), { LITERAL, STRING_LIT } };
	}
	else if (expr[current_index] == '{')			// Set literals.
	{
		int i, level = 0;				// We're going to look for closing brace '}' at level 0.
		bool closingbrace_found = false;
		for (i = current_index + 1; i < expr.size(); i++) 
		{
			if (expr[i] == '}' && level == 0)	// Aha, closing brace found.
			{
				closingbrace_found = true;
				break;
			}
			else if (expr[i] == '(' || expr[i] == '{' || expr[i] == '[') level++;
			else if (expr[i] == ')' || expr[i] == '}' || expr[i] == ']') level--;	
		}
		if (!closingbrace_found) return{ "", { ERROR } };
		else
		{
			int j = current_index;
			current_index = i + 1;	// The next lexeme starts after '}'
			return{ expr.substr(j, i - j + 1), { LITERAL, SET_LIT } };
		}
	}
	else if (expr[current_index] == '(')	  		// Now depending on the current character, we return a token.
	{					  		// It can either be (<expr>) or a tuple literal.
		bool is_tuple = false;		  		// Let us assume it is not a tuple.
		int i, level = 0;			  		// If we find another '(', we increase the level, and decrease if ')'.
		for (i = current_index + 1; i < expr.size(); i++)
			if (expr[i] == '(' || expr[i] == '{' || expr[i] == '[') level++;
			else if (expr[i] == ')' || expr[i] == '}' || expr[i] == ']') level--;
			else if (expr[i] == ',' && level == 0)	// If we find a ',' at level 0, it means that it is outside of any ...
			{					// ... parentheses except the one in the 'case', and its counterpart.
				is_tuple = true;		// Therefore, the lexeme that we're looking at is most certainly a tuple.
				break;				// We can stop looking now.
			}						

		level = 0;					// Now, either way, we need to look for the counterpart ')'.
		bool counterpart_found = false;
		for (i = current_index + 1; i < expr.size(); i++)
		{
			if (expr[i] == ')' && level == 0)	// If we find a ')' at level 0, it has got to be the counterpart ...
			{					// ... because it is outside of any other parentheses.
				counterpart_found = true;	// So found it.
				break;				// We can stop looking now.
			}
			else if (expr[i] == '(' || expr[i] == '{' || expr[i] == '[') level++;
			else if (expr[i] == ')' || expr[i] == '}' || expr[i] == ']') level--;
		}
		if (!counterpart_found) return{ "", {ERROR} };	// If a counterpart is not found, return an ERROR token.
		int j = current_index;
		if (is_tuple) 
		{
			current_index = i + 1;			// The next lexeme starts after ')'            
			return{ expr.substr(j, i - j + 1), {LITERAL, TUPLE_LIT} };
		}
		else
		{
			current_index = i + 1;			// The next lexeme starts after ')'            
			return{ expr.substr(j + 1, i - j - 1), {EXPR} };
		}
	}

	//-------------------------------------------</LITERALS>----------------------------------------------//

	else if (expr[current_index] == '[')
	{
		int i, level = 0;				// We're going to look for closing bracket ']' at level 0.
		bool closingbracket_found = false;
		for (i = current_index + 1; i < expr.size(); i++)
		{
			if (expr[i] == ']' && level == 0)	// Aha, closing bracket found.
			{
				closingbracket_found = true;
				break;
			}
			else if (expr[i] == '[') level++;	// We're inside some other set, which is {an element of}^+ the one we're parsing.
			else if (expr[i] == ']') level--;	// We jump out one level.
		}
		if (!closingbracket_found) return{ "", { ERROR } };
		else
		{
			int j = current_index;
			current_index = i + 1;	// The next lexeme starts after ']'
			return{ expr.substr(j+1, i - j - 1), { OP, INDEX } };
		}
	}
	
	else if (isalpha(expr[current_index]) || expr[current_index] == '_')	// Parsing identifier.
	{
		int i = current_index + 1;					// Going to look for the end of this identifier.
		while (isalnum(expr[i]) || expr[i] == '_') i++;			// Keep going  ...
		int j = current_index;						// Store the current_index ...
		current_index = i;						// ... because it is about to be updated.
		return{ expr.substr(j, i - j), {IDENTIFIER} };			// Return the token initializer list.
	}

	//--------------------------------------------<OPS>-----------------------------------------------//

	else if (expr[current_index] == '+')
	{
		current_index++;
		return{ "+", { OP, INT_OP, CHAR_OP, STRING_OP } };
	}

	else if (expr[current_index] == '-')
	{
		current_index++;
		return{ "-", { OP, INT_OP, CHAR_OP, STRING_OP } };
	}
	else if (expr[current_index] == '*')
	{
		current_index++;
		return{ "*", { OP, INT_OP, CHAR_OP } };
	}
	else if (expr[current_index] == '/')
	{
		current_index++;
		return{ "/", { OP, INT_OP, CHAR_OP } };
	}
	else if (expr[current_index] == '^')
	{
		current_index++;
		return{ "^", { OP, INT_OP, CHAR_OP } };
	}
	else if (expr[current_index] == '%')
	{
		current_index++;
		return{ "%", { OP, INT_OP, CHAR_OP } };
	}
	else if (expr[current_index] == '<')
	{
		if ((current_index + 1) < expr.size() && expr[current_index + 1] == '=')
		{
			current_index += 2;
			return{ "<=", { OP, INT_OP, CHAR_OP, STRING_OP } };
		}
		else
		{
			current_index++;
			return{ "<", { OP, INT_OP, CHAR_OP, STRING_OP } };
		}
	}

	else if (expr[current_index] == '>')
	{
		if ((current_index + 1) < expr.size() && expr[current_index + 1] == '=')
		{
			current_index += 2;
			return{ ">=", { OP, INT_OP, CHAR_OP, STRING_OP } };
		}
		else
		{
			current_index++;
			return{ ">", { OP, INT_OP, CHAR_OP, STRING_OP } };
		}
	}

	//-------------------------------------------</OPS>-----------------------------------------------//

	return{ "", {ERROR} };
}

/*	<expr> -->   (<expr>)
 *		   | !<expr>			# Negation.
 *		   | |<expr>|			# Cardinality.
 *		   | <expr> <op> <expr>		# Every op basically.
 *		   | <expr>[<expr>]		# Set, Tuple, String, Map, Auto query/access.
 *		   | <term>			
 *		
 *	<term> -->   <identifier>
		   | <literal>
 */	

ExpressionTree::ExpressionTree(string &expr)
{
	this->expr = expr;
	Token t1 = get_next_token();
	if (t1.types[0] == END)					// End of recursion.
	{
		root = nullptr;
		return;
	}

	Token t2 = get_next_token();

	if (t2.types[0] == OP)					// If we have <expr> <op> <expr>
	{
		string rest = expr.substr(current_index, expr.size() - current_index);
		root = new Node();
		root->operator_node = true;
		root->token = t2;
		root->left = new ExpressionTree(t1.lexeme);
		root->right = new ExpressionTree(rest);
		return;
	}

	if (t2.types[0] == INDEX)
	{
		string rest = expr.substr(current_index, expr.size() - current_index);
		if (rest == "")
		{
			root = new Node();
			root->operator_node = true;
			root->token = { "[]", { OP, INDEX } };
			root->left = new ExpressionTree(t1.lexeme);
			root->right = new ExpressionTree(t2.lexeme);
		}
		else
		{
			root = new Node();
			root->operator_node = true;
			root->token = { "()", { OP, UNARY, EXPR } };
			root->left = new ExpressionTree((string)"((" + t1.lexeme + ")" + "[" + t2.lexeme + "]" + ")");
		}
		return;
	}

	/*
	 *	<expr> --> !<expr>
	 *	<expr> --> |<expr>|
	 */
	if (t1.types[0] == OP)							// If we've seen an operator.
	{
		if (t1.lexeme == "!")						// A NOT, so this will just precede an expression.
		{
			Token to_be_negated = t2;				// Get the expression to be negated.
			string rest {
				expr.substr(current_index, expr.size() - current_index),// Get the rest of the string.
			};
			if (rest == "") 					// rest == "" implies we're doing a primitive negation.
			{
				root = new Node();					
				root->operator_node = true;
				root->token = t1;
				root->left = new ExpressionTree(to_be_negated.lexeme);
			}
			else 
			{
				root = new Node();					 
				root->operator_node = true;
				root->token = { "()", {OP, UNARY, EXPR} };
				root->left = new ExpressionTree (
					(string)"(!(" + to_be_negated.lexeme + "))" + rest
				);
			}
			return;
		}
		if (t1.lexeme == "|")					// Okay, so we're gonna be checking the size of something.
		{
			Token get_size_of = t2;				// Generate the token whose size you want.
			Token get_second_pipe = get_next_token();
			string rest{
				expr.substr(current_index, expr.size() - current_index),// Get the rest of the string.
			};
			if (rest == "") 				// rest == "" implies we're doing a primitive get_size().
			{
				root = new Node();
				root->operator_node = true;
				root->token = t1;
				root->left = new ExpressionTree(get_size_of.lexeme);
			}
			else
			{
				root = new Node();
				root->operator_node = true;
				root->token = { "()", { OP, UNARY, EXPR } };
				root->left = new ExpressionTree (
					(string)"(|(" + get_size_of.lexeme + ")|)" + rest
				);
			}
		}
		if (t1.lexeme == ".")
		{
			Token to_be_copied = t2;
			string rest{
				expr.substr(current_index, expr.size() - current_index),// Get the rest of the string.
			};
			if (rest == "") 						// rest == "" implies we're doing basic deep_copy().
			{
				root = new Node();
				root->operator_node = true;
				root->token = t1;
				root->left = new ExpressionTree(to_be_copied.lexeme);
			}
			else
			{
				root = new Node();
				root->operator_node = true;
				root->token = { "()", { OP, UNARY, EXPR } };
				root->left = new ExpressionTree(
					(string)"(.(" + to_be_copied.lexeme + "))" + rest
				);
			}
		}
		return;
	}
	/*
	 *	<expr> --> (<expr>)
	 */ 
	if (t1.types[0] == EXPR)							
	{
		root = new Node();				// Make a root node to hold the operator.
		root->operator_node = true;			// Mark it as an operator node.
		root->token = Token{ "()", {OP, UNARY, EXPR} }; // Let it hold the token. 
		root->left = new ExpressionTree(t1.lexeme);	// Let the left parse the <expr> in (<expr>).
		return;
	}

	if (t1.types[0] == IDENTIFIER)
	{
		root = new Node();
		root->operator_node = false;
		root->token = t1;
		root->value = (*program_vars::identify)[t1.lexeme];
		return;
	}

	if (t1.types[0] == LITERAL)
	{
		root = new Node();
		root->operator_node = false;
		root->token = t1;
		root->value = root->parse_literal();
		return;
	}
	
	// By now we've exhausted all the cases where a check was necessary on the first token.
}

// -------------------------------------------------<CLASS EXPRESSIONTREE>--------------------------------------//