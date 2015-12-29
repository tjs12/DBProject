#include "../query/parse.h"

#include "../storage/Table.h"
#include "../storage/TableIterator.h"
#include "../systemm/dbManager.h"

#include <fstream>
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#define bufferSize 1024
using namespace std;
vector<string> split(string cmds,char sp){
	vector<string>ans;
	int start=0;
	for(int i = 0;i<cmds.length();i++){
		if(cmds[i]==sp){
			ans.push_back(cmds.substr(start,i-start+1));
			start=i+1;
		}
	}
	return ans;
}
//int main(){
//	int m;
//	ifstream fin;
//	fin.open("./test/create.sql");
//	string s;
//	char tmp[bufferSize];
//	while(fin.getline(tmp,bufferSize)){
//		s+=string(tmp); 
//	}
//
//	vector<string>ans=split(s,';');
//	for(int i=0;i<ans.size();i++){
//		cout<<ans[i]<<endl;
//	}
//	parse_entrance()
//	fin.close(); 
//	system("pause");
//	return 0;
//} 

void test_createDb(string dbname){
	DbManager::getInstance()->createDb(dbname);
}
void test_createTable(string table_name){
	Table *t1 = new Table();
	Type coltypes[3] = {Type(TYPE_CHAR),Type(TYPE_CHAR),Type(TYPE_CHAR)};
	string names[3]={string ("id"),string("n"),string("a")};
	vector<Type>vt;
	vector<string>vn;
	for(int i = 0;i<3;i++){
		vt.push_back(coltypes[i]);
		vn.push_back(names[i]);
	}
	t1->createTable(vt,vn,table_name);
	//t1->createTable(vt, vn, string("t1"));
}
void test_insert_string(Record r,string table_name){
	//insertRecord(Record &r);
}
int main() {
	test_createDb("mydb");
	test_createTable("mytable");
	string();
	
}