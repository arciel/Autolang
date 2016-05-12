#ifndef SET_TEST_H
#define SET_TEST_H

#include <iostream>
#include "../Header Files/Set.h"
#include "../Header Files/Char.h"
#include "../Header Files/Int.h"
#include "../Header Files/Logical.h"
#include "../Header Files/String.h"

namespace Set_test
{
	using std::cout;
	using std::endl;
	
	static void build_AB(Set &A, Set &B)
	{
		cout << "\n----------- Constructing frequently used Sets A and B. -------------\n\n";
		Tuple *t = new Tuple(new vector<Elem *>{new String("INTJ")});
		vector<Elem *> *elems = new vector<Elem *>{ new String("Hello"), new Logical(true), t, new Int(27), new Char('J')};
		A.elems = elems;
		cout << "A = " << A.to_string() << endl;

		Tuple *t2 = new Tuple(new vector<Elem *>{ new String("Suits")});
		vector<Elem *> *elems2 = new vector<Elem *>{ new String("World"), new Logical(false), t2, new Int(06), new Char('J'), new Int(1996)};
		B.elems = elems2;
		cout << "B = " << B.to_string() << endl << endl;
	}

	static void cardinality(Set &A, Set &B)
	{
		cout << "\n----------- Testing the cardinality() method for Sets. -------------\n\n";
		cout << "|A| = " << A.cardinality() << endl;
		cout << "|B| = " << B.cardinality() << endl << endl;
	}

	static void _union(Set &A, Set &B)
	{
		cout << "\n------------- Testing the _union() method for Sets. ----------------\n\n";
		Set C = A._union(B);
		cout << "A U B = " << C.to_string() << endl << endl;
	}

	static void intersection(Set &A, Set &B)
	{
		cout << "\n---------- Testing the intersection() method for Sets. -------------\n\n";
		cout << "A & B = " << A.intersection(B).to_string() << endl << endl;
	}
	
	static void cartesian_product(Set &A, Set &B)
	{
		Set *AxB = &A.cartesian_product(B);
		cout << "\n------- Testing the cartesian_product() method for Sets. -----------\n\n";
		cout << "A x B = " << AxB->to_string() << endl << endl;
	}

	static void exclusion(Set &A, Set &B)
	{
		cout << "\n------------- Testing the exclusion() method for Sets. -------------\n\n";
		cout << "A \\ B = " << A.exclusion(B).to_string() << endl << endl;
	}

	static void access(Set &A, Set &B)
	{
		cout << "\n-------------- Testing the [] operator method for Sets. ------------\n\n";
		cout << "A[0] = " << A[0]->to_string() << endl << endl;
		cout << "B[0] = " << B[0]->to_string() << endl << endl << endl;
	}

	static void homoset()
	{
		cout << "\n--------------- Testing the homoset() method for Sets. -------------\n\n";
		Set C(new vector<Elem *>{ new Int(27), new Int( 6 ), new Int(1996) });
		Set D(new vector<Elem *>{ new Int(27), new Char('6'), new Int(1996) });

		cout << C.to_string();
		if (C.homoset()) 
			cout << " is a homoset with homotype " << C.homotype() << endl; 
		else 
			cout << " is not a homoset" << endl;

		cout << D.to_string();
		if (D.homoset())
			cout << " is a homoset with homotype " << D.homotype() << endl;
		else
			cout << " is not a homoset" << endl << endl;
	}

	static void subset(Set &A, Set &B)
	{
		cout << "A[0, 2] = " << A.subset(0, 2).to_string() << endl;
		cout << "B[1, 4] = " << B.subset(1, 4).to_string() << endl << endl;
	}
	
	static void subset_of()
	{
		cout << "\n------------- Testing the subset_of() method for Sets. -----------\n\n";
		Set E(new vector<Elem *>{ new Int(1), new Int(2), new Char('3'), new Logical(true) });
		Set F(new vector<Elem *>{ new Int(1), new Char('3') });

		Set G(new vector<Elem *>{ new Tuple(new vector<Elem *>{ new Int(1), new Logical(false) }), new Char(('X')) });
		Set H(new vector<Elem *>{ new Tuple(new vector<Elem *>{ new Int(1), new Logical(false) })});

		cout << F.to_string() << " c " << E.to_string() << " = " << F.subset_of(E) << endl;
		cout << G.to_string() << " c " << H.to_string() << " = " << G.subset_of(H) << endl << endl;
	}

	static void equality()
	{
		cout << "\n---------------- Testing the == operator for Sets. --------------\n\n";
		Set I(new vector<Elem *> {
			new Int(1), 
			new Tuple(new vector<Elem *>{ new String("Yo") }),
			new Logical(true),
			new String("INTJ"),
			new Char('T')
		});
		Set J(new vector<Elem *> { 
			new Int(1), 
			new Tuple(new vector<Elem *>{ new String("Yo") }),
			new Logical(true),
			new String("INTJ"),
			new Char('T')
		});
		Set K(new vector<Elem *>{
			new Int(1), 
			new Tuple(new vector<Elem *>{ new String("Yp") }),
			new Logical(true),
			new String("INTJ"),
			new Char('T')
		});
		cout << I.to_string() << " == " << J.to_string() << " = " << (I == J) << endl;
		cout << I.to_string() << " == " << K.to_string() << " = " << (I == K) << endl << endl;
	}

	static void deep_copy(Set &A)
	{
		cout << "\n------------ Testing the deep_copy() method for Sets. ----------\n\n";
		cout << "A = " << A.to_string() << endl << endl;

		Set *L = (Set *)(A.deep_copy());
		cout << "Let L := A. L = " << L->to_string() << ", ";
		cout << "(A == L) = " << (A == *L) << endl << endl; 

		Set M = A;

		cout << "Let M  = A. M = " << M.to_string() << ", ";
		cout << "(A == M) = " << (A ==  M) << endl << endl;

		delete (*L)[3];					// Remove the Int 27 from L[3].
		(*L)[3] = new Int(28);				// Put a new Int 28 there.

		delete M[3];
		M[3] = new Int(29);

		cout << "L[3] := 28, L = " << L->to_string() << endl;
		cout << "M[3] := 29, M = " << M.to_string() << endl << endl;

		cout << "A = " << A.to_string() << endl << endl; 

		cout << "(A == L) = " << (A == *L) << endl;	
		cout << "(A == M) = " << (A ==  M) << endl << endl;
	}
	
	static void test_all()
	{
		Set A, B;
		build_AB(A, B);	cardinality(A, B);
		_union(A, B);	intersection(A, B);
		access(A, B);	homoset();
		subset(A, B);	subset_of();
		equality();	deep_copy(A);
		cartesian_product(A, B);
		exclusion(A, B);
	}
}

#endif