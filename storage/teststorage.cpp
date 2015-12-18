#include <iostream>

#include "Table.h"

using namespace std;

int main()
{
	Table *t1 = new Table();
	Type coltypes[3] = {Type(TYPE_INT,4), Type(TYPE_CHAR,2), Type(TYPE_CHAR,2)};
	string colnames[3] = {string("id"), string("n"), string("a")};
	t1->createTable(3, coltypes, colnames, string("t1"));
	delete t1;
	//Table *t2 = new Table();
	//t2 -> openTable(string("t1"));
	//delete t2;
	return 0;
}
