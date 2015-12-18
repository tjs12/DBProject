#include "qlm.h"
RC Select (int nSelAttrs,        // # attrs in Select clause
              const RelAttr selAttrs[],       // attrs in Select clause
              int           nRelations,       // # relations in From clause
              const char * const relations[], // relations in From clause
              int           nConditions,      // # conditions in Where clause
              const Condition conditions[])

{
	//parseRelations()
	//parseConditions(nConditions, conditions, )
	
	return RETURN_SUCCEED;
}

RC Insert (const char  *relName,           // relation to insert into
              int         nValues,            // # values to insert
              const Value values[])          // values to insert
{
	return RETURN_SUCCEED;
}

RC Delete (const char *relName,            // relation to delete from
              int        nConditions,         // # conditions in Where clause
              const Condition conditions[])  // conditions in Where clause
{
	return RETURN_SUCCEED;
}