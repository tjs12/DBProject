#ifndef QLM_H
#define QLM_H

#include <vector>
#include <iostream>
#include "../common/rc.h"
#include "../common/Condition.h"
#include "../storage/Record.h"

using namespace std;

class QL_Manager {
 public:
                                              // Constructor
      /*QL_Manager (
		 // SM_Manager &smm, IX_Manager &ixm, RM_Manager &rmm
		  );
      ~QL_Manager ();                         // Destructor*/
    RC Select (int           nSelAttrs,        // # attrs in Select clause
              const RelAttr selAttrs[],       // attrs in Select clause
              int           nRelations,       // # relations in From clause
              const char * const relations[], // relations in From clause
              int           nConditions,      // # conditions in Where clause
              /*const*/ Condition conditions[]);  // conditions in Where clause
    RC Insert (const char  *relName,           // relation to insert into
              int         nValues,            // # values to insert
              const Value values[]);          // values to insert
    RC Delete (const char *relName,            // relation to delete from
              int        nConditions,         // # conditions in Where clause
              /*const*/ Condition conditions[]);  // conditions in Where clause
    RC Update (const char *relName,            // relation to update
              const RelAttr &updAttr,         // attribute to update
              const int bIsValue,             // 0/1 if RHS of = is attribute/value
              const RelAttr &rhsRelAttr,      // attr on RHS of =
              const Value &rhsValue,          // value on RHS of =
              int   nConditions,              // # conditions in Where clause
              /*const*/ Condition conditions[]);  // conditions in Where clause

	static QL_Manager *getInst() {return &inst;}

private:
	QL_Manager() {}
	QL_Manager(const QL_Manager &val) {}
	static QL_Manager inst;

	RC select_all_attr(
              int           nRelations,       // # relations in From clause
              const char * const relations[], // relations in From clause
              int           nConditions,      // # conditions in Where clause
              Condition conditions[]);  // conditions in Where clause

	vector<Record> res;
	vector<int> res_rid;
	vector<string> attr_names;
};

#endif
