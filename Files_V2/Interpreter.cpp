#include <iostream>
#include "Set.h"
#include "Int.h"
#include "Logical.h"
#include "String.h"
#include "Char.h"

using std::cout;
using std::endl;

int main() 
{
	string x = "1234";
	string y = x;
	x[2] = '5';
	cout << x << " " << y << endl;
	return 0;
}