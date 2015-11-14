#ifndef RECORD_H
#define RECORD_H

#include "Type.h"

class Record
{
public:
	int columnNum;
	Type *columnTypes;
	int *data;
	
	Record(int colnum, Type *coltype, int *rec_data) {
		columnNum = colnum;
		columnTypes = new Type[colnum];
		for (int i = 0; i < colnum; i++) {
			columnTypes[i] = coltype[i];
		}
		int s = size()
		data = new int[s];
		memcpy(data, rec_data, sizeof(int) * s);
	}
	
	~Record() {
		delete []columnTypes;
		delete []data;
	}
	
	int size() {
		int s = 0;
		for (int i = 0; i < columnNum; i++) s += columnTypes[i].size();
	}
	
	static int getRecordSize(int columnNum, Type *columnTypes) {
		int size = 0
		for (int i = 0; i < columnNum; i++)
			size += columnTypes[i].size();
		return size;
	}
	
}

#endif