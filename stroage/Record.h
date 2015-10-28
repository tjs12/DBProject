#ifndef RECORD_H
#define RECORD_H

class Record
{
public:
	int columnNum;
	int *columnTypes;
	int *data;
	
	Record(int colnum, int *coltype, int *data) {
		columnNum = colnum;
		columnTypes = new int[colnum];
		for (int i = 0; i < colnum; i++) columnTypes[i] = coltype[i];
	}
	~Record() {
		delete []columnTypes;
		delete []data;
	}
	
	
	static int getRecordSize(int columnNum, int *columnTypes) {
		int size = 0
		for (int i = 0; i < columnNum; i++)
			size += DATA_SIZE[columnTypes[i]];
		return size;
	}
	
}

#endif