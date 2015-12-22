#ifndef TABLE_H
#define TABLE_H

#include <string>
#include <vector>

#include "../filesystem/bufmanager/BufPageManager.h"
//#include "../filesystem/fileio/FileManager.h"
//#include "../filesystem/utils/pagedef.h"
#include "../common/rc.h"

#include "Record.h"

#define MAX_COL_NAME_LEN 255


/*
Data format:
Page 0: Header Page
	COLUMN_NUM: Number of columns(1)
	COLUMN_TYPES: Data types of the columns(1 for each column)
	col_type_settings: Settings of the data types(1 for each column)
	col_name : Names of the columns(MAX_STR for each column)

Other Pages:
	PAGE_HEADER_VALID: the page is valid if value = PAGE_VALID
	NEXT_PAGE_AVAILABLE: number of next page with free space

*/


#define COLUMN_NUM 0
#define PRIMARY_KEY 1
#define COLUMN_TYPES 2


#define PAGE_HEADER_VALID 0
#define NEXT_PAGE_AVAILABLE 1
#define PAGE_VALID 255 //NB
//#define PAGE_HEADER_SIZE  //TODO
#define PAGE_BITMAP_POS 3


class Table {
friend class TableIterator;
public:
	string tableName;
	int columnNum;
	int primaryKey;
	std::vector<Type> columnTypes;
	std::vector<std::string> columnNames;
	
	//Record *select(string *col_names, Condition cond);
	void deleteTable();
	void insert();
	RC Table::getRecord(int rid, Record &rec);
	int updateRecord(int rid, Record &r, Record &mask);
	int insertRecord(Record &r);
	int deleteRecord(int rid);
	int getRecordNum() {return record_num;}
	bool isRecord(int rid);
	
	RC createTable(vector<Type> &col_type, vector<string> &col_names, string name, int pri_key = -1);
	void openTable(string name);
	
	Table() {primaryKey = -1;}
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
	
	void make_header(BufType b);
	bool is_record_buf(int pos_in_page, BufType b);
	void check_page_validity(BufType b, int pgnum = 1);

	int max_rid;

};
	
#endif
