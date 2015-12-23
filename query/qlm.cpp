#include "qlm.h"
#include "../storage/TableIterator.h"

RC QL_Manager::select_all_attr (
              int           nRelations,       // # relations in From clause
              const char * const relations[], // relations in From clause
              int           nConditions,      // # conditions in Where clause
              /*const*/ Condition conditions[])

{
	if (nRelations == 1) {
		
		Table t;
		if (t.openTable(string(relations[0])) == RETURN_FAILED) return RETURN_FAILED;

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
	}

	else if (nRelations == 2) {
		Table t1, t2;
		if (t1.openTable(string(relations[0])) == RETURN_FAILED) return RETURN_FAILED;
		if (t2.openTable(string(relations[1])) == RETURN_FAILED) return RETURN_FAILED;

		res.clear();
		res_rid.clear();


		for (TableIterator i(&t1); !i.isEnd(); i.gotoNext()) {
			for (TableIterator j(&t2); !j.isEnd(); j.gotoNext()) {

				//TODO change left value of conditions from attribute to value;

				Record current = i.current();
				bool flag = true;
				for (int k = 0; k < nConditions; k++) {
					if (!current.judgeCondition(conditions[k], t1.columnNames)) {
						flag = false;
						break;
					}
				}
				if (flag) {
					res.push_back(current);
					res_rid.push_back(i.getRID());
				}
			}
		}
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
	if (t.openTable(string(relName)) == RETURN_FAILED) return RETURN_FAILED;

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
	if (t.openTable(string(relName)) == RETURN_FAILED) return RETURN_FAILED;


	for (int i = 0; i < res_rid.size(); i++) {
		t.deleteRecord(res_rid[i]);
	}

	return RETURN_SUCCEED;
}