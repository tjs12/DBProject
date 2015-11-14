#ifndef TABLE_H
#define TABLE_H

#include <string>

#include "filesystem/bufmanager/BufPageManager.h"
#include "filesystem/fileio/FileManager.h"
#include "filesystem/utils/pagedef.h"

#include "Record.h"

/*
Data format:
Page 0: Header Page
	COLUMN_NUM: Number of columns(1)
	COLUMN_TYPES: Data types of the columns(1 for each column)
	col_type_settings: Settings of the data types(1 for each column)
	col_name : Names of the columns(MAX_STR for each column)

*/

class Table {
public:
	string tableName
	int columnNum;
	Type *columnTypes;
	std::string *columnNames;
	
	Record *select(string *col_names, Condition cond);
	void deleteTable();
	void insert();
	Record getRecord(int rid);
	int updateRecord(int rid, Record &r, Record &mask);
	int insertRecord(Record &r);
	int deleteRecord(int rid);
	int getRecordNum() {return record_num;}
	bool isRecord(int rid);
	
	void createTable(int col_num, Type *col_type, std::string *col_names, std::string name);
	void openTable(string name);
	
	~Table();
private:
	FileManager *fm;
	BufPageManager* bpm;
	int fid;
	
	int record_per_page;
	int record_num;
	int start_page;
	int first_page_available;
	int page_header_size;
	
	make_header(Buf_Type b);
	bool is_record_buf(int pos_in_page, BufType b);
	void check_page_validity(BufType b);
}
	
#endif