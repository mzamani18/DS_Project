all: BTree.h hash.h QueryParse.cpp
	g++ BTree.h -o BTree
	g++ hash.h -o hash
	g++ -std=c++14 -o QueryParse QueryParse.cpp -lpthread -I.
# g++ -std=c++14 -o BTree BTree.cpp -lpthread -I.
# g++ -std=c++14 -o hash hash.cpp -lpthread -I.