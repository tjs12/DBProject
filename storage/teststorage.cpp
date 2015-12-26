#include <iostream>

#include "Table.h"
#include "TableIterator.h"

using namespace std;

int storagemain()
{
	Table *t1 = new Table();
	Type coltypes[3] = {Type(TYPE_INT), Type(TYPE_INT), Type(TYPE_INT)};
	string colnames[3] = {string("id"), string("n"), string("a")};
	vector<Type> vt ;
	vector<string> vn ;

	for (int i = 0; i < 3; i++) {
		vt.push_back(coltypes[i]);
		vn.push_back(colnames[i]);
	}

	//t1->createTable(vt, vn, string("t1"));
	int recdata[3] = {1, 2, 3};
	Record *rec = new Record(vt);
	for (int i = 0; i < 3; i++) rec->addVar(new IntVar(recdata[i]), i);
	/*int rid = t1->insertRecord(*rec);
	int rid2 = t1->insertRecord(*rec);
	delete t1;


	Table *t2 = new Table();
	t2 -> openTable(string("t1"));
	Record r1;
	if (t2->getRecord(rid, r1) == RETURN_SUCCEED) {
		for (int i = 0; i < 3; i++) {
			Var *var = 0;
			string temp = (r1.getVar(i))->toString();
			cout << temp << ' ';
		}
	}
	cout <<endl;
	if (t2->getRecord(rid2, r1) == RETURN_SUCCEED) {
		for (int i = 0; i < 3; i++) {
			Var *var = 0;
			string temp = (r1.getVar(i))->toString();
			cout << temp << ' ';
		}
	}

	t2->deleteRecord(rid2);
	
	cout <<endl;
	if (t2->getRecord(rid2, r1) == RETURN_SUCCEED) {
		cout<<"delete failed"<<endl;
	}

	if (t2->deleteRecord(rid2) == RETURN_SUCCEED) {
		cout<<"delete failed2"<<endl;
	}

	delete t2;*/

	cout<<"write"<<endl;

	Table *t3 = new Table;
	t3 -> createTable(vt, vn, string("t3"));
	for (int i = 0; i < 1000; i++) {
		rec->addVar(&IntVar(i), 0);
		int rid = t3->insertRecord(*rec);
		cout << rid << ' ' << i << endl;
	}
	for (int i = 0; i < 500; i++) {
		t3->deleteRecord(i);
	}
	for (int i = 1000; i < 1500; i++) {
		rec->addVar(&IntVar(i), 0);
		int rid = t3->insertRecord(*rec);
		cout << rid << ' ' << i << endl;
	}
	delete t3;

	cout <<endl <<"read" <<endl;
	//TableIterator ti(t3);
	t3 = new Table();
	t3 -> openTable(string("t3"));
	Record tempr;
	for (TableIterator i(t3); !i.isEnd(); i.gotoNext()) {
		tempr = i.current();
		cout << i.getRID() << ' ' << tempr.getVar(0)->toString() << endl;
		if (tempr.getVar(0)->equal(new IntVar(991))) {
			int abc;
			abc = 10;
		}
	}

	
	return 0;
}
