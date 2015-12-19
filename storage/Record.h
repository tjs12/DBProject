#ifndef RECORD_H
#define RECORD_H

#include "Var.h"
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
	
	~Record() {
		//if (columnTypes != 0) delete []columnTypes;
		if (data != 0) delete []data;
		
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
