#ifndef TABLE_H
#define TABLE_H

#include <string>

#include "filesystem/bufmanager/BufPageManager.h"
#include "filesystem/fileio/FileManager.h"
#include "filesystem/utils/pagedef.h"

#include "Record.h"

class Table {
public:
	string tableName
	int columnNum;
	int *columnTypes;
	std::string *columnNames;
	
	Record *select(string *col_names, Condition cond);
	void deleteTable();
	void insert();
	Record getRecord(int rid);
	void updateRecord(int rid, Record &r, Record &mask);
	void insertRecord(Record &r);
	void deleteRecord(int rid);
	int getRecordNum() {return record_num;}
	
	void createTable(int col_num, int *col_type, std::string name);
	void openTable(string name);
	
	~Table();
private:
	FileManager *fm;
	BufPageManager* bpm;
	int fid;
	
	int record_per_page;
	int record_num;
	int start_page;
	int **page_available;
	
	make_header(Buf_Type b);
}
	
#endif