#ifndef RECORD_H
#define RECORD_H

#include "Var.h"
#include "../common/Condition.h"
#include <cstring>
#include <vector>

using namespace std;

class Record
{
public:
	int columnNum;
	vector<Type> columnTypes;
	
	
	Record(vector<Type> &coltype) {
		columnNum = coltype.size();
		//columnTypes.clear();
		//columnTypes = new Type[colnum];
		for (int i = 0; i < columnNum; i++) {
			//columnTypes.push_back(coltype[i]);
			vars.push_back(0);
		}
		columnTypes = coltype;
		int s = size();
		data = new unsigned int[s];
		
	}

	Record() {
		columnNum = 0;
		//columnTypes = 0;
		data = 0;
	}

	Record(Record &r)
	{
		
		//for (int i = 0; i < r.size(); i++)
		//if (r.data != 0)
		//	memcpy(data, r.data, r.size() * sizeof(unsigned int));
		columnNum = r.columnNum;
		columnTypes = r.columnTypes;
		data = 0;
		vars = vector<Var*>();
		for (int i = 0; i < r.vars.size(); i++) {
			vars.push_back(r.vars[i]->copy());
		}
	}
	
	~Record() {
		//if (columnTypes != 0) delete []columnTypes;
		//if (data != 0) delete []data;
		
	}
	
	int size() {
		int s = 0;
		for (int i = 0; i < columnNum; i++) s += columnTypes[i].size();
		return s;
	}
	
	RC addVar(Var *v, int place)
	{
		if (place <0 || place >= columnNum) return RETURN_FAILED;
		vars[place] = v;
	}

	Var *getVar(int place)
	{
		if (place <0 || place >= columnNum) return 0;
		return vars[place];
	}

	unsigned int *getData() {
		int j = 0;
		if (data == 0) data = new unsigned int[getRecordSize(columnNum, columnTypes)];

		for (int i = 0; i < columnNum; i++) {
			vars[i]->writeToBuf(&data[j]);
			j += columnTypes[i].size();
		}
		return data;
	}

	void fromData(unsigned int *d) {
		int j = 0;
		for (int i = 0; i < columnNum; i++) {
			vars[i] = Var::fromBuf(&d[j], columnTypes[i]);
			j += columnTypes[i].size();
		}
	}



	bool judgeCondition(Condition &c, vector<string> colNames)
	{
		if (c.bRhsIsAttr) return false;
		int i;
		for (i = 0; i < colNames.size(); i++) {
			if (colNames[i].compare(c.lhsAttr.attrName)) {
				break;
			}
		}
		Var *data = Var::fromBuf((unsigned int*)c.rhsValue.data, c.rhsValue.type);
		return data->equal(vars[i]);
	}
	
	static int getRecordSize(int columnNum, vector<Type> columnTypes) {
		int size = 0;
		for (int i = 0; i < columnNum; i++)
			size += columnTypes[i].size();
		return size;
	}
	
private:
	unsigned int *data;
	vector <Var*> vars;

};

#endif
