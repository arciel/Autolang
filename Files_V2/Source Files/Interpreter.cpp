#include "../Tests/Map_test.h"
#include <unordered_map>
#include <vector>

using std::cout;
using std::vector;
using std::endl;
using std::unordered_map;
using std::string;/*

unordered_map<string, Elem *> elem;		// Elements stored against their names (identifiers).
unordered_map<string, Type> type		// Types stored against typedef keywords in Autolang.
{
	{ "auto", AUTO }, { "char", CHAR },
	{ "int", INT }, { "logical", LOGICAL },
	{ "map", MAP }, { "set", SET }, 
	{ "string", STRING }, { "tuple", TUPLE }
};
unordered_map<string, int> tokens		// Token values against lexemes.
{
	{ "data_type", 1 }, 
	{ "",8 }, 
};
*/
int main() 
{	
	Map_test::test_all();
}
