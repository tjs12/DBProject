#include "qlm.h"
#include "../storage/TableIterator.h"
#include "../systemm/dbManager.h"


QL_Manager QL_Manager::inst;

RC QL_Manager::select_all_attr (
              int           nRelations,       // # relations in From clause
              const char * const relations[], // relations in From clause
              int           nConditions,      // # conditions in Where clause
              /*const*/ Condition conditions[])

{
	DbManager* m = DbManager::getInstance();

	if (nRelations == 1) {
		Table t;
		string tablepath = m->getCurDbPath(), tablename = string(relations[0]);
		tablepath = tablepath.append("\\").append(tablename);
		if (t.openTable(tablepath) == RETURN_FAILED) return RETURN_FAILED;

		res.clear();
		res_rid.clear();

		for (TableIterator i(&t); !i.isEnd(); i.gotoNext()) {
			Record current = i.current();
			bool flag = true;
			for (int j = 0; j < nConditions; j++) {
				if (!current.judgeCondition(conditions[j], t.columnNames)) {
					flag = false;
					break;
				}
			}
			if (flag) {
				res.push_back(current);
				res_rid.push_back(i.getRID());
			}

		}

		attr_names = t.columnNames;
	}

	else if (nRelations == 2) {
		Table t1, t2;
		string tablepath1 = DbManager::getInstance()->getCurDbPath(), tablename1 = relations[0];
		tablepath1 = tablepath1.append("\\").append(tablename1);
		string tablepath2 = DbManager::getInstance()->getCurDbPath(), tablename2 = relations[1];
		tablepath2 = tablepath2.append("\\").append(tablename2);
		if (t1.openTable(tablepath1) == RETURN_FAILED) return RETURN_FAILED;
		if (t2.openTable(tablepath2) == RETURN_FAILED) return RETURN_FAILED;

		res.clear();
		res_rid.clear();

		vector<string> joined_colname;

		for (int i = 0; i < t1.columnNum; i++) {
			joined_colname.push_back(string(relations[0]).append(string(".")).append(t1.columnNames[i]));
		}

		for (TableIterator i(&t1); !i.isEnd(); i.gotoNext()) {
			for (TableIterator j(&t2); !j.isEnd(); j.gotoNext()) {

				//TODO change left value of conditions from attribute to value;

				Record current1 = i.current(), current2 = j.current();
				Record joined = Record::join(current1, current2);
				bool flag = true;
				for (int k = 0; k < nConditions; k++) {
					if (!joined.judgeCondition(conditions[k], joined_colname)) {
						flag = false;
						break;
					}
				}
				if (flag) {
					res.push_back(joined);
				}
			}
		}

		attr_names = joined_colname;
	}

	
	
	return RETURN_SUCCEED;
}

RC QL_Manager::Insert (const char  *relName,           // relation to insert into
              int         nValues,            // # values to insert
              const Value values[])          // values to insert
{
	vector<Type> types;
	for (int i = 0; i < nValues; i++) {
		types.push_back(values[i].type);
	}
	Record rec(types);
	for (int i = 0; i < nValues; i++) {
		rec.addVar(Var::fromBuf((unsigned int*)values[i].data, values[i].type), i);
	}

	Table t;  
	string tablepath = DbManager::getInstance()->getCurDbPath(), tablename = relName;
	tablepath = tablepath.append("\\").append(tablename);
	if (t.openTable(tablepath) == RETURN_FAILED) return RETURN_FAILED;

	if (t.primaryKey != -1) {
		for (TableIterator i(&t); !i.isEnd(); i.gotoNext()) {
			Record current = i.current();
			if ((rec.getVar(t.primaryKey))->equal(current.getVar(t.primaryKey)))
				return RETURN_FAILED;
		}
	}

	t.insertRecord(rec);

	return RETURN_SUCCEED;
}

RC QL_Manager::Delete (const char *relName,            // relation to delete from
              int        nConditions,         // # conditions in Where clause
              Condition conditions[])  // conditions in Where clause
{
	select_all_attr(1, &relName, nConditions, conditions);

	Table t;  
	string tablepath = DbManager::getInstance()->getCurDbPath(), tablename = relName;
	tablepath = tablepath.append("\\").append(tablename);
	if (t.openTable(tablepath) == RETURN_FAILED) return RETURN_FAILED;


	for (int i = 0; i < res_rid.size(); i++) {
		t.deleteRecord(res_rid[i]);
	}

	return RETURN_SUCCEED;
}

 RC QL_Manager::Update (const char *relName,            // relation to update
              const RelAttr &updAttr,         // attribute to update
              const int bIsValue,             // 0/1 if RHS of = is attribute/value
              const RelAttr &rhsRelAttr,      // attr on RHS of =
              const Value &rhsValue,          // value on RHS of =
              int   nConditions,              // # conditions in Where clause
              Condition conditions[])  // conditions in Where clause
 {
	select_all_attr(1, &relName, nConditions, conditions);
	Table t;  
	string tablepath = DbManager::getInstance()->getCurDbPath(), tablename = relName;
	tablepath = tablepath.append("\\").append(tablename);
	if (t.openTable(tablepath) == RETURN_FAILED) return RETURN_FAILED;

	Record mask = Record(t.columnTypes);
	unsigned int *maskdata = new unsigned int[mask.size()];
	memset(maskdata, 0, sizeof(unsigned int)*mask.size());

	for (int i = 0; i < res.size(); i++) {
		int j;
		for (j = 0; j < t.columnNum; j++) 
			if (t.columnNames[j] == updAttr.attrName) break;
		if (bIsValue)
			res[i].addVar(Var::fromBuf((unsigned int*)rhsValue.data, rhsValue.type), j);
		else {
			int r_attr_pos = 0;
			for (; r_attr_pos < t.columnNum; r_attr_pos++) 
				if (t.columnNames[r_attr_pos] == rhsRelAttr.attrName)
					break;
			res[i].addVar(res[i].getVar(r_attr_pos), j);
		}
		
		mask.fromData(maskdata);
		t.updateRecord(res_rid[i], res[i], mask);
	}

	delete []maskdata;

	return RETURN_SUCCEED;
 }


RC QL_Manager::Select (int           nSelAttrs,        // # attrs in Select clause
              const RelAttr selAttrs[],       // attrs in Select clause
              int           nRelations,       // # relations in From clause
              const char * const relations[], // relations in From clause
              int           nConditions,      // # conditions in Where clause
              Condition conditions[])  // conditions in Where clause
{
	RC r = select_all_attr(nRelations, relations, nConditions, conditions);
	if (r != RETURN_SUCCEED) return r;

	//Table t;  
	
	for (int i = 0; i < res.size(); i++) {
		for (int j = 0; j < attr_names.size(); j++) {
			for (int k = 0; k < nSelAttrs; k++) {
				if (attr_names[j].compare(selAttrs[k].attrName) == 0 || 
					(selAttrs[k].relName!=0 && attr_names[j].compare(string(selAttrs[k].relName).append(".").append(selAttrs[k].attrName)) == 0) ) //*
				{
					cout << res[i].getVar(j)->toString() << ' ';
				}
			}
			if (nSelAttrs == 0) cout << res[i].getVar(j)->toString() << ' ';
		}
		cout << endl;
	}
	return RETURN_SUCCEED;
}