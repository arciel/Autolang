#ifndef TUPLE_TEST_H
#define TUPLE_TEST_H

#include "Set_test.h"

namespace Tuple_test
{
	using std::cout;
	using std::endl;

	static void build_AB(Tuple &A, Tuple &B)
	{
		cout << "\n----------- Constructing frequently used Tuples A and B. -------------\n\n";
		Set *t = new Set(new vector<Elem *>{ new String("INTJ") });
		vector<Elem *> *elems = new vector<Elem *>{ new String("Hello"), new Logical(true), t, new Int(27), new Char('J') };
		A.elems = elems;
		cout << "A = " << A.to_string() << endl;

		Set *t2 = new Set(new vector<Elem *>{ new String("Suits") });
		vector<Elem *> *elems2 = new vector<Elem *>{ new String("World"), new Logical(false), t2, new Int(06), new Char('J'), new Int(1996) };
		B.elems = elems2;
		cout << "B = " << B.to_string() << endl << endl;
	}

	static void size(Tuple &A, Tuple &B)
	{
		cout << "\n-------------- Testing the size() method for Tuples. ----------------\n\n";
		cout << "|A| = " << A.size() << endl;
		cout << "|B| = " << B.size() << endl << endl;
	}

	static void contains(Tuple &A)
	{
		cout << "\n-------------- Testing the has() method for Tuples. -----------------\n\n";
		Elem *str1 = new String("Hello"), *str2 = new String("World");
		Elem *set1 = new Set(new vector<Elem *>{ new String("INTJ") });
		Elem *set2 = new Set(new vector<Elem *>{ new String("INTP") });

		cout << A.to_string() << " has " << str1->to_string() << " = " << A.has(*str1) << endl;
		cout << A.to_string() << " has " << str2->to_string() << " = " << A.has(*str2) << endl;
		cout << A.to_string() << " has " << set1->to_string() << " = " << A.has(*set1) << endl;
		cout << A.to_string() << " has " << set2->to_string() << " = " << A.has(*set2) << endl;
	}

	static void equality()
	{
		cout << "\n---------------- Testing the == operator for Sets. --------------\n\n";
		Tuple I(new vector<Elem *>{
			new Int(1),
			new Set(new vector<Elem *>{ new String("Yo") }),
			new Logical(true),
			new String("INTJ"),
			new Char('T')
		});
		Tuple J(new vector<Elem *>{
			new Int(1),
			new Set(new vector<Elem *>{ new String("Yo") }),
			new Logical(true),
			new String("INTJ"),
			new Char('T')
		});
		Tuple K(new vector<Elem *>{
			new Int(1),
			new Set(new vector<Elem *>{ new String("Yp") }),
			new Logical(true),
			new String("INTJ"),
			new Char('T')
		});
		cout << I.to_string() << " == " << J.to_string() << " = " << (I == J) << endl;
		cout << I.to_string() << " == " << K.to_string() << " = " << (I == K) << endl << endl;
	}

	static void deep_copy(Tuple &A)
	{
		cout << "\n------------- Testing the deep_copy() method for Tuples. ------------\n\n";
		cout << "A = " << A.to_string() << endl << endl;

		Tuple *L = (Tuple *)(A.deep_copy());
		cout << "Let L := A. L = " << L->to_string() << ", ";
		cout << "(A == L) = " << (A == *L) << endl << endl;

		Tuple M = A;

		cout << "Let M  = A. M = " << M.to_string() << ", ";
		cout << "(A == M) = " << (A == M) << endl << endl;

		delete (*L)[3];					// Remove the Int 27 from L[3].
		(*L)[3] = new Int(28);				// Put a new Int 28 there.

		delete M[3];
		M[3] = new Int(29);

		cout << "L[3] := 28, L = " << L->to_string() << endl;
		cout << "M[3] := 29, M = " << M.to_string() << endl << endl;

		cout << "A = " << A.to_string() << endl << endl;

		cout << "(A == L) = " << (A == *L) << endl;
		cout << "(A == M) = " << (A == M) << endl << endl;
	}

	static void test_all()
	{
		Tuple A, B;
		build_AB(A, B);	
		size(A, B);
		contains(A), equality();
		deep_copy(A);
	}
}

#endif