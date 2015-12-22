#include "dbManager.h"
using namespace std; 
int main(){
	DbManager* m = DbManager::getInstance();
	m->useDb("hgee");
	m->showTables();
	Type coltypes[3] = {Type(TYPE_INT), Type(TYPE_INT), Type(TYPE_INT)};
	string colnames[3] = {string("id"), string("n"), string("a")};
	vector<Type>types;
	vector<string>names;
	for (int i = 0; i < 3; i++) {
		types.push_back(coltypes[i]);
		names.push_back(colnames[i]);
	}
	m->createTable(types,names,"customer",1);
	int k;cin>>k;
}
