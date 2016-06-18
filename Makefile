# Makes the interpreter for Autolang Version 2.

# MACROS

SOURCE = Files_V2/Source\ Files/
HEADER = Files_V2/Header\ Files/
FLAGS = -std=c++14 -w
OBJS = Interpreter.o Auto.o ExpressionTree.o Logical.o Map.o Set.o Tuple.o

# TARGETS

auto : $(OBJS)
	$(CXX) -o auto $(OBJS)

Set.o : $(SOURCE)Set.cpp $(HEADER)Set.h $(HEADER)ExpressionTree.h 
	$(CXX) -c $(FLAGS) $(SOURCE)Set.cpp

Tuple.o : $(SOURCE)Tuple.cpp $(HEADER)Tuple.h $(HEADER)Set.h $(HEADER)ExpressionTree.h
	$(CXX) -c $(FLAGS) $(SOURCE)Tuple.cpp

Logical.o : $(SOURCE)Logical.cpp $(HEADER)Logical.h
	$(CXX) -c $(FLAGS) $(SOURCE)Logical.cpp

Map.o : $(SOURCE)Map.cpp $(HEADER)Map.h
	$(CXX) -c $(FLAGS) $(SOURCE)Map.cpp

Auto.o : $(SOURCE)Auto.cpp $(HEADER)Auto.h
	$(CXX) -c $(FLAGS) $(SOURCE)Auto.cpp

ExpressionTree.o : $(SOURCE)ExpressionTree.cpp $(HEADER)ExpressionTree.h
	$(CXX) -c $(FLAGS) $(SOURCE)ExpressionTree.cpp

Interpreter.o : $(SOURCE)Interpreter.cpp $(HEADER)ExpressionTree.h
	$(CXX) -c $(FLAGS) $(SOURCE)Interpreter.cpp

clean : 
	rm $(OBJS)
