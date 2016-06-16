#include "../Header Files/ExpressionTree.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>

using std::cout;
using std::cin;
using std::endl;
using std::istream;
using std::queue;
using std::ios;

unordered_map<string, Elem *> * program_vars::identify = new unordered_map<string, Elem *> { { "__prompt__", new String(">>> ") } };	
// Identifiers mapped to their objects.

using program_vars::identify;

Token current_token;		// The current token we're looking at. 
queue<Token> tokens;		// A queue of tokens to help the get_next_token stream. 
istream * program;		// Stream of text input for the interpreter.
int line_num = 1;		// Line number that we're looking at right now.

Token get_next_token();		// Both of these together ...
Token peek_next_token();	// ... make up the lexer.
void parse_program();		// Parse the program.
void parse_statement();		// Parse a statement.
void parse_declaration();	// Parse a declaration.
void parse_initialization();	// Parse an initialization.
void parse_assignment();	// Parse an assignment.
void parse_input();		// Parse an input command.
void parse_print();		// Parse a print command.
void parse_mapping(string &);	// Parse a single mapping.
void parse_if();		// Parse an if condition.
void parse_while();		// Parse a while loop.
void parse_delete();		// Parse a delete command.
void parse_delete_elems();	// Parse a delete_elems command.
void parse_expression();	// Parse an expression.
void trim(string &);		// Trim a string (*sigh*).
void remove_comment(string&);   // Removes the comment at the end of the line.
bool identifier(string &);	// Returns true if a string is an identifier.
void raise_error(const char *);	// Raises an error and quits the program.
bool all_spaces(string &);	// Returns true if a string is full of spaces.
void print_info();		// Prints the license and other info.

vector<string> &split(string &, vector<string> &);	// Functions to split strings.
vector<string> split(string &);				// Functions to split strings.


int main(int argc, char **argv) 
{ 
	if (argc == 1) 
	{ 
		program = &cin;
		print_info();
	}
	else program = new std::ifstream(argv[1]);
	parse_program();
}

Token get_next_token()
{
	Token next = peek_next_token();
	if (!tokens.empty()) tokens.pop();
	return next;
}

Token peek_next_token()						// The lexer.
{
	if (tokens.empty() && program->eof()) return{ "", {END} };
	if (!tokens.empty()) return tokens.front();
	string line;
	do {
		if (program == &std::cin) cout << (*identify)["__prompt__"]->to_string();
		getline(*program, line);			// Get the entire next line.
		line_num++;
		remove_comment(line);
		trim(line);					// Finally trim the string to get the usable lexeme.
	} while (all_spaces(line));

	vector<string> parts = split(line);			// We'll split the line by spaces.

	for (auto &lexeme : parts) {
		if (lexeme != "") {	
			//cout << "Pushing: " << lexeme << endl;
			if	(lexeme == "--quit--")	tokens.push(Token{lexeme, {QUIT}});		// The quit token, so quit the program.
			else if (lexeme == "delete")	tokens.push(Token{lexeme, {DELETE}});		// The delete token.
			else if (lexeme == "delete_elems") tokens.push(Token{lexeme, {DELETE_ELEMS}});	// The delete elems token.
			else if (lexeme == "declare")	tokens.push(Token{ lexeme, { DECLARE } });

			else if	(lexeme == "set" || lexeme == "string" || lexeme == "int" ||		// If it's a data_type token.
				 lexeme == "char" || lexeme == "tuple" || lexeme == "map" ||
		       		 lexeme == "logical" || lexeme == "auto")
				tokens.push(Token{ lexeme, {TYPE}});

			else if (lexeme == "=")		tokens.push(Token{ lexeme, {EQUAL_SIGN}});
			else if (lexeme == "-->")	tokens.push(Token{ lexeme, {MAPPING_SYMBOL}});
			else if (lexeme == ":")		tokens.push(Token{ lexeme, {COLON}});
			else if (lexeme == "input")	tokens.push(Token{ lexeme, {INPUT}});
			else if (lexeme == "print")	tokens.push(Token{ lexeme, {PRINT}});
			else if (lexeme == "{")		tokens.push(Token{ lexeme, {L_BRACE}});
			else if (lexeme == "}")		tokens.push(Token{ lexeme, {R_BRACE}});
			else if (lexeme == "while")	tokens.push(Token{ lexeme, {WHILE}});
			else if (lexeme == "if")	tokens.push(Token{ lexeme, {IF}});
			else if (lexeme == "else")	tokens.push(Token{ lexeme, {ELSE}});
			else if (lexeme == "endw")	tokens.push(Token{ lexeme, {END_WHILE}});
			else if (lexeme == "endif")	tokens.push(Token{ lexeme, {END_IF}});
			else if (identifier(lexeme))	tokens.push(Token{ lexeme, {IDENTIFIER}});   
			else				tokens.push(Token{ lexeme, { EXPR } });
		}
	}	
	return tokens.front();
}

void parse_program()
{
	do {
		parse_statement();
	} while (current_token.types[0] != END);
}

void parse_statement()
{
	current_token = get_next_token();
	if (current_token.types[0] == QUIT || current_token.types[0] == END) {cout << "\n"; exit(0); }				// Exit.
	else if (current_token.types[0] == DELETE) parse_delete();
	else if (current_token.types[0] == DELETE_ELEMS) parse_delete();
	else if (current_token.types[0] == DECLARE) parse_declaration();
	else if (current_token.types[0] == TYPE) parse_initialization();
	else if (current_token.types[0] == WHILE) parse_while();
	else if (current_token.types[0] == IF) parse_if();
	else if (current_token.types[0] == PRINT) parse_print();
	else parse_assignment();
}

void parse_assignment()
{
	Token &update = current_token;

	Token eq_sign = get_next_token();

	if (eq_sign.types[0] != EQUAL_SIGN) raise_error("Missing operator \"=\".");

	Token expression = get_next_token();

	ExpressionTree expr(expression.lexeme, ROOT);

	(*identify)[update.lexeme] = expr.evaluate();
}

void parse_print()
{
	Token print = get_next_token();
	ExpressionTree expr(print.lexeme, ROOT);
	Elem * to_be_printed = expr.evaluate();

	cout << to_be_printed->to_string();
	if (program == &cin) cout << endl << endl;
}

void parse_delete()		// Parse a delete command.
{
	Token delete_this = get_next_token();
	if (delete_this.types[0] == IDENTIFIER && (*identify)[delete_this.lexeme] != nullptr)	// If it's an identifier ...
		delete (*identify)[delete_this.lexeme];						// ... then delete the object it maps to.
}

void parse_delete_elems()	// Parse a delete_elems command.
{
	Token delete_elemsof_this = get_next_token();
	if (delete_elemsof_this.types[0] == IDENTIFIER && (*identify)[delete_elemsof_this.lexeme] != nullptr)
	{
		Elem * /*delete_elements*/of_this = (*identify)[delete_elemsof_this.lexeme];	// Get a pointer to the object.

		if (of_this->type == SET) ((Set  *)of_this)->delete_elems();			// Cast it appropriately ...
		else if (of_this->type == TUPLE) ((Tuple*)of_this)->delete_elems();		// ... and call the method needed.
		else if (of_this->type == MAP) ((Map  *)of_this)->delete_elems();
	}
}

void parse_declaration()	// Parse a declaration.
{
	Token data_type = get_next_token();

	if (data_type.types[0] != TYPE) raise_error("Data type not supported.");

	Token new_identifier = get_next_token();

	if (new_identifier.types[0] != IDENTIFIER)	   raise_error("Please use a valid name for the identifier.");
	if ((*identify)[new_identifier.lexeme] != nullptr) raise_error("Identifier already in use. Cannot re-declare.");

	if (data_type.lexeme == "set") (*identify)[new_identifier.lexeme] = new Set();
	else if (data_type.lexeme == "tuple")(*identify)[new_identifier.lexeme] = new Tuple();
	else if (data_type.lexeme == "map")
	{
		Token colon = get_next_token();
		if (colon.types[0] != COLON) raise_error("A map identifier must be followed by a \":\".");

		Token domain = get_next_token();
		Token mapsymb = get_next_token();
		Token codomain = get_next_token();

		Elem * map_domain = nullptr, *map_codomain = nullptr;	// Actual pointers that will be used in the map's constructor.

		if (mapsymb.types[0] != MAPPING_SYMBOL) raise_error("Missing mapping operator \"-->\".");

		if (domain.types[0] == IDENTIFIER)
			if ((*identify)[domain.lexeme] == nullptr)
				raise_error("The domain identifier doesn't refer to any object.");
			else if ((*identify)[domain.lexeme]->type != SET)
				raise_error("The domain of a map must be a set.");
			else
				map_domain = (*identify)[domain.lexeme];

		else if (domain.types[0] == EXPR)
		{
			ExpressionTree domain_expr(domain.lexeme, ROOT);
			map_domain = domain_expr.evaluate();
			if (map_domain->type != SET)
				raise_error("The domain of a map must be a set.");
		}

		if (codomain.types[0] == IDENTIFIER)
			if ((*identify)[codomain.lexeme] == nullptr)
				raise_error("The codomain identifier doesn't refer to any object.");
			else if ((*identify)[codomain.lexeme]->type != SET)
				raise_error("The codomain of a map must be a set.");
			else
				map_codomain = (*identify)[codomain.lexeme];

		else if (codomain.types[0] == EXPR)
		{
			ExpressionTree codomain_expr(codomain.lexeme, ROOT);
			map_codomain = codomain_expr.evaluate();
			if (map_codomain->type != SET)
				raise_error("The codomain of a map must be a set.");
		}
		(*identify)[new_identifier.lexeme] = new Map((Set *)map_domain, (Set *)map_codomain);
	}
	else if (data_type.lexeme == "int") (*identify)[new_identifier.lexeme] = new Int();
	else if (data_type.lexeme == "char") (*identify)[new_identifier.lexeme] = new Char();
	else if (data_type.lexeme == "string") (*identify)[new_identifier.lexeme] = new String();
	else if (data_type.lexeme == "logical") (*identify)[new_identifier.lexeme] = new Logical();

}

void parse_initialization()
{
	Token & data_type = current_token;

	if (data_type.types[0] != TYPE) raise_error("Data type not supported.");

	Token new_identifier = get_next_token();

	if (new_identifier.types[0] != IDENTIFIER)	   raise_error("Please use a valid name for the identifier.");
	if ((*identify)[new_identifier.lexeme] != nullptr) raise_error("Identifier already in use. Cannot re-declare.");

	if (data_type.lexeme == "set")  
	{
		Token equal_sign = get_next_token();

		if (equal_sign.types[0] != EQUAL_SIGN) raise_error("Missing operator \"=\".");

		Token value = get_next_token();			// The value to be assigned to the identifier.

		ExpressionTree value_expr(value.lexeme, ROOT);

		Elem * val = value_expr.evaluate();

		if (val->type != SET) raise_error("Cannot assign a non-set object to a set identifier.");

		(*identify)[new_identifier.lexeme] = val;
	}
	else if (data_type.lexeme == "tuple")
	{
		Token equal_sign = get_next_token();

		if (equal_sign.types[0] != EQUAL_SIGN) raise_error("Missing operator \"=\".");

		Token value = get_next_token();			// The value to be assigned to the identifier.
		
		ExpressionTree value_expr(value.lexeme, ROOT);

		Elem * val = value_expr.evaluate();

		if (val->type != TUPLE) raise_error("Cannot assign a non-tuple object to a tuple identifier.");

		(*identify)[new_identifier.lexeme] = val;
	}
	else if (data_type.lexeme == "map")
	{
		Token colon = get_next_token();
		if (colon.types[0] != COLON) raise_error("A map identifier must be followed by a \":\".");

		Token domain = get_next_token();
		Token mapsymb = get_next_token();
		Token codomain = get_next_token();

		Elem * map_domain = nullptr, *map_codomain = nullptr;	// Actual pointers that will be used in the map's constructor.

		if (mapsymb.types[0] != MAPPING_SYMBOL) raise_error("Missing mapping operator \"-->\".");

		if (domain.types[0] == IDENTIFIER)
			if ((*identify)[domain.lexeme] == nullptr)
				raise_error("The domain identifier doesn't refer to any object.");
			else if ((*identify)[domain.lexeme]->type != SET)
				raise_error("The domain of a map must be a set.");
			else
				map_domain = (*identify)[domain.lexeme];

		else if (domain.types[0] == EXPR)
		{
			ExpressionTree domain_expr(domain.lexeme, ROOT);
			map_domain = domain_expr.evaluate();
			if (map_domain->type != SET)
				raise_error("The domain of a map must be a set.");
		}

		if (codomain.types[0] == IDENTIFIER)
			if ((*identify)[codomain.lexeme] == nullptr)
				raise_error("The codomain identifier doesn't refer to any object.");
			else if ((*identify)[codomain.lexeme]->type != SET)
				raise_error("The codomain of a map must be a set.");
			else
				map_codomain = (*identify)[codomain.lexeme];

		else if (codomain.types[0] == EXPR)
		{
			ExpressionTree codomain_expr(codomain.lexeme, ROOT);
			map_codomain = codomain_expr.evaluate();
			if (map_codomain->type != SET)
				raise_error("The codomain of a map must be a set.");
		}
		(*identify)[new_identifier.lexeme] = new Map((Set *)map_domain, (Set *)map_codomain);
	}
	else if (data_type.lexeme == "int") 
	{
		Token equal_sign = get_next_token();
		
		if (equal_sign.types[0] != EQUAL_SIGN) raise_error("Missing operator \"=\".");
		
		Token int_expression = get_next_token();

		ExpressionTree int_expr(int_expression.lexeme, ROOT);

		Elem * val = int_expr.evaluate();

		if (val->type != INT) raise_error("Cannot assign a non-int value to an int identifier.");

		(*identify)[new_identifier.lexeme] = val;
	}
	else if (data_type.lexeme == "char")
	{
		Token equal_sign = get_next_token();

		if (equal_sign.types[0] != EQUAL_SIGN) raise_error("Missing operator \"=\".");

		Token char_expression = get_next_token();

		ExpressionTree char_expr(char_expression.lexeme, ROOT);

		Elem * val = char_expr.evaluate();

		if (val->type != CHAR) raise_error("Cannot assign a non-char value to a char identifier.");

		(*identify)[new_identifier.lexeme] = val;
	}
	else if (data_type.lexeme == "string") 
	{
		Token equal_sign = get_next_token();

		if (equal_sign.types[0] != EQUAL_SIGN) raise_error("Missing operator \"=\".");

		Token str_expression = get_next_token();

		ExpressionTree str_expr(str_expression.lexeme, ROOT);

		Elem * val = str_expr.evaluate();

		if (val->type != STRING) raise_error("Cannot assign a non-string value to a string identifier.");

		(*identify)[new_identifier.lexeme] = val;
	}
	else if (data_type.lexeme == "logical")
	{
		Token equal_sign = get_next_token();

		if (equal_sign.types[0] != EQUAL_SIGN) raise_error("Missing operator \"=\".");

		Token logic_expression = get_next_token();

		ExpressionTree logic_expr(logic_expression.lexeme, ROOT);

		Elem * val = logic_expr.evaluate();

		if (val->type != LOGICAL) raise_error("Cannot assign a non-logical value to a logical identifier.");

		(*identify)[new_identifier.lexeme] = val;
	}
	else if (data_type.lexeme == "auto")
	{
		Token equal_sign = get_next_token();

		if (equal_sign.types[0] != EQUAL_SIGN) raise_error("Missing operator \"=\".");

		Token tuple_expression = get_next_token();

		ExpressionTree tuple_expr(tuple_expression.lexeme, ROOT);

		Elem * val = tuple_expr.evaluate();

		if (val->type != TUPLE) raise_error("Initializing an automaton needs a tuple.");

		Tuple * val_ = (Tuple *)val;

		if (val_->size() != 5) raise_error("Initializing an automaton needs a 5-tuple.");

		(*identify)[new_identifier.lexeme] = new Auto(	// Make a new automaton object.
			(Set *)(*val_)[0], (Set *)(*val_)[1], (*val_)[2], (Map *)(*val_)[3], (Set *)(*val_)[4], DIRECT_ASSIGN
		);
	}
}

void parse_while()
{
	Token condition = get_next_token();

	if (condition.types[0] != EXPR) raise_error("Expected an expression.");

	Token starting_brace = get_next_token();

	if (starting_brace.types[0] != L_BRACE) raise_error("Missing '{'.");

	int loop_from = program->tellg();		// We will restart the stream of tokens from this point.
	int restore_line = line_num;
	
	ExpressionTree * logical_condition = new ExpressionTree(condition.lexeme, ROOT);
	Elem * do_or_not = logical_condition->evaluate();

	if (do_or_not->type != LOGICAL) raise_error("Expected a logical expression.");

	while (((Logical *)do_or_not)->elem)
	{
		if (logical_condition != nullptr) delete logical_condition;	// We have no use for the old parse tree of the condition ...
		if (do_or_not != nullptr) delete do_or_not;			// ... or this value, because the identifiers may have changed.

		while (peek_next_token().types[0] != R_BRACE)			// As long as you don't see the end of the while loop ...
		{
			parse_statement();					// ... keep parsing statements.
		}
		Token r_brace = get_next_token();				// Should be an R_BRACE.
		if (r_brace.types[0] != R_BRACE) raise_error("Expected ending brace");

		program->seekg(loop_from, ios::beg);				// Go back to the beginning of statements when you're done.
		line_num = restore_line;

		logical_condition = new ExpressionTree(condition.lexeme, ROOT);	// Re-parse the condition ...
		do_or_not = logical_condition->evaluate();			// ... and re-evaluate it.

		if (do_or_not->type != LOGICAL) raise_error("Expected a logical expression (non-logical sometime during the iterations).");
	}		
	
	delete logical_condition;	// Finally once you're done with the loop ... 
	delete do_or_not;		// ... you can delete these objects.
	
	int level = 0;			// Now we're literally going to keep reading and tossing tokens out ...
	Token end_while;		// .. until we get the endw we're looking for Let's see if the next thing we get is an end_while.

	while (true) 
	{
		end_while = get_next_token();
		//cout << "Seeing (and possibly throwing): " << end_while.to_string() << endl;
		if (level == 0 && end_while.types[0] == END) raise_error("While block unterminated at the end of program.");
		else if (end_while.types[0] == WHILE) level++;
		else if (end_while.types[0] == END_WHILE) 
		{
			if (level == 0) { /*cout << "So we're breaking."*/; break; }
			else level--;
		}
	}
}

void parse_if()		// Parsing if statements.
{
	Token condition = get_next_token();
	if (condition.types[0] != EXPR) raise_error("Expected an expression.");
	Token starting_brace = get_next_token();
	if (starting_brace.types[0] != L_BRACE) raise_error("Missing '{'.");

	ExpressionTree * logical_condition = new ExpressionTree(condition.lexeme, ROOT);
	Elem * do_or_not = logical_condition->evaluate();

	if (do_or_not->type != LOGICAL) raise_error("Expected a logical expression.");

	if (((Logical *)do_or_not)->elem)
	{
		while (peek_next_token().types[0] != R_BRACE)			// As long as you don't see the end of the if block ...
			parse_statement();					// ... keep parsing statements.

		Token r_brace = get_next_token();				// Should be an R_BRACE.
		if (r_brace.types[0] != R_BRACE) raise_error("Expected ending brace");
		Token else_ = get_next_token();
		if (else_.types[0] != ELSE) raise_error("else block expected.");

		// But we have to do nothing with the else block. We just need to ignore it.

		int level = 0;		// Now we're literally going to keep reading and tossing tokens out ...
		Token end_if;		// .. until we get the endif we're looking for. Let's see if the next thing we get is an end_if.
		while (true)
		{
			end_if = get_next_token();
			if (level == 0 && end_if.types[0] == END) raise_error("else block unterminated at the end of program.");
			if (end_if.types[0] == IF) level++;
			else if (end_if.types[0] == END_IF)
			{
				if (level == 0) break;
				else level--;
			}
		}
	}
	else
	{
		int level = 0;		// Now we're literally going to keep reading and tossing tokens out ...
		Token else_;		// .. until we get the else we're looking for. Let's see if the next thing we get is an end_if.
		while (true)
		{
			else_ = get_next_token();
			if (level == 0 && else_.types[0] == END) raise_error("if block unterminated at the end of program.");
			if (else_.types[0] == IF) level++;
			else if (else_.types[0] == ELSE)
			{
				if (level == 0) break;
				else level--;
			}
		}
		Token l_brace = get_next_token();
		if (l_brace.types[0] != L_BRACE) raise_error("'{' expected.");	
			
		while (peek_next_token().types[0] != R_BRACE)			// As long as you don't see the end of the else block ...
			parse_statement();					// ... keep parsing statements.

		Token r_brace = get_next_token();				// Should be an R_BRACE.
		if (r_brace.types[0] != R_BRACE) raise_error("Expected ending brace");
		Token endif = get_next_token();
		if (endif.types[0] != END_IF) raise_error("\"endif\" expected.");
	}
	delete logical_condition;	// Finally once you're done with the loop ... 
	delete do_or_not;
}

void trim(string &str)
{
	if (all_spaces(str)) return;
	int start = 0, end = str.size() - 1;
	while (isspace(str[start])) start++;
	while (isspace(str[ end ]))  end-- ;
	if (start >= end) str == "";
	str = str.substr(start, end - start + 1);
}

vector<string> &split(string &x, vector<string> &elements) 
{
	int level = 0, start = 0;
	bool in_string = false;
	for (int i = start; i < x.size(); i++)
	{
		if (isspace(x[i]) && level == 0)		// If we find a space that delimits an elements representation ...
		{
			elements.push_back(x.substr(start, i - start));	// Push it to the vector of representations
			start = i + 1;					// The next element's representation will usually start from i + 1.
		}
		if (((x[i] == '`' && !in_string) || x[i] == '{' || x[i] == '(' || x[i] == '[')
			&& (i == 0 || x[i - 1] != '\\')) {
			level++;
			if (x[i] == '`' && !in_string) in_string = true;
		}
		else if (((x[i] == '`' && in_string) || x[i] == '}' || x[i] == ')' || x[i] == ']')
			&& (i == 0 || x[i - 1] != '\\')) {
			level--;
			if (x[i] == '`' && in_string) in_string = false;
		}
	}
	elements.push_back(x.substr(start, x.size() - start));
	return elements;
}

vector<string> split(string &s) 
{
	vector<string> elems;
	split(s, elems);
	return elems;
}

bool identifier(string &s)
{
	if (s[0] == '_' || isalpha(s[0])) {
		for (int i = 1; i < s.size(); i++)
			if (!isalnum(s[i]) && s[i] != '_')
				return false;
		return true;
	}
	else return false;
}

void raise_error(const char * message)
{
	cout << "ERROR: Line " << line_num << ": " << message << endl;
	exit(0);
}

void remove_comment(string &x)
{
	if (all_spaces(x)) return;
	int level = 0, i;
	bool in_string = false;
	for (i = 0; i < x.size(); i++)
	{
		if ((x[i] == '`' && !in_string) && (i == 0 || x[i - 1] != '\\')) {
			level++;
			if (x[i] == '`' && !in_string) in_string = true;
		}
		else if ((x[i] == '`' && in_string) && (i == 0 || x[i - 1] != '\\')) {
			level--;
			if (x[i] == '`' && in_string) in_string = false;
		}
		else if (x[i] == '#' && level == 0)		
			break;
	}
	x = x.substr(0, i);
}

bool all_spaces(string& x)
{
	for (auto& c : x)
		if (!isspace(c))
			return false;
	return true;
}

void print_info()
{
	cout << "Autolang, Version 2.0 \nCopyright (c) 2016 Tushar Rakheja (The MIT License)." << endl << endl;
	cout << "Please contribute to Autolang if you find it useful." << endl;
	cout << "For more info, visit https://github.com/TusharRakheja/Autolang." << endl << endl;
	cout << "To change the prompt, use the env. variable \"__prompt__\"." << endl << endl;	
}