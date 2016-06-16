# Makes the interpreter for Autolang Version 2.

# MACROS

SOURCE = Files_V2/Source\ Files/
HEADER = Files_V2/Header\ Files/
CC = g++
FLAGS = -std=c++1y -w
OBJS = Interpreter.o Auto.o ExpressionTree.o Logical.o Map.o Set.o Tuple.o

# TARGETS

auto : $(OBJS)
	$(CC) -o auto $(OBJS)

Set.o : $(SOURCE)Set.cpp $(HEADER)Set.h $(HEADER)ExpressionTree.h 
	$(CC) -c $(FLAGS) $(SOURCE)Set.cpp

Tuple.o : $(SOURCE)Tuple.cpp $(HEADER)Tuple.h $(HEADER)Set.h $(HEADER)ExpressionTree.h
	$(CC) -c $(FLAGS) $(SOURCE)Tuple.cpp

Logical.o : $(SOURCE)Logical.cpp $(HEADER)Logical.h
	$(CC) -c $(FLAGS) $(SOURCE)Logical.cpp

Map.o : $(SOURCE)Map.cpp $(HEADER)Map.h
	$(CC) -c $(FLAGS) $(SOURCE)Map.cpp

Auto.o : $(SOURCE)Auto.cpp $(HEADER)Auto.h
	$(CC) -c $(FLAGS) $(SOURCE)Auto.cpp

ExpressionTree.o : $(SOURCE)ExpressionTree.cpp $(HEADER)ExpressionTree.h
	$(CC) -c $(FLAGS) $(SOURCE)ExpressionTree.cpp

Interpreter.o : $(SOURCE)Interpreter.cpp $(HEADER)ExpressionTree.h
	$(CC) -c $(FLAGS) $(SOURCE)Interpreter.cpp

clean : 
	rm *.o auto
