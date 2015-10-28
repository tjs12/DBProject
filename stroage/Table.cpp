#include "Table.h"
using namespace std;

void Table::createTable(int col_num, int *col_type, string *col_names, string name)
{
	tableName = name;
	columnNum = col_num;
	columnTypes = new int[col_num];
	columnNames = new string[col_num];
	for (int i = 0; i < col_num; i++) {
		columnType[i] = col_type[i];
		columnNames[i] = col_names[i];
	}
	
	fm = new FileManager();
	bpm = new BufPageManager(fm);
	fm -> createFile(name);
	fm -> openFile(name, fid);
	
	int index;
	BufType b = bpm -> allocPage(fid, 0, index, false);
	make_header(b);
	bpm -> markDirty(index);
}

void Table::openTable(string name)
{
	fm = new FileManager();
	bpm = new BufPageManager(fm);
	fm -> openFile(name, fid);
	int index;
	BufType b = bpm->getPage(fid, 0, index);
	//read header
	columnNum = b[COLUMN_NUM];
	columnTypes = new int[columnNum];
	columnNames = new string[columnNum];
	
	int col_name_pos = COLUMN_TYPES + columnNum;
	int rec_per_page_pos = col_name_pos + columnNum * MAX_STRING_LEN / 4;
	int rec_num_pos = rec_per_page_pos + 1;
	
	for (int i = 0; i < col_num; i++) {
		columnTypes[i] = b[COLUMN_TYPES + i];
		columnNames[i] = new string((char*)b[col_name_pos + i * MAX_STRING_LEN / 4]);
	}
	record_per_page = b[rec_per_page_pos];
	record_num = b[rec_num_pos];
}


Table::make_header(BufType b)
{
	int col_name_pos = COLUMN_TYPES + columnNum;
	int rec_per_page_pos = col_name_pos + columnNum * MAX_STRING_LEN / 4;
	int rec_num_pos = rec_per_page_pos + 1;
	
	b[COLUMN_NUM] = columnNum;
	for (int i = 0; i < col_num; i++) {
		b[COLUMN_TYPES + i] = columnTypes[i];
		
		char *c_str = columnNames[i].c_str();
		memcpy(b[col_name_pos + i * MAX_STRING_LEN / 4], c_str, strlen(c_str) + 1);
		delete []c_str;
	}
	
	record_per_page = PAGE_SIZE / Record::getRecordSize(columnNum, columnTypes);
	b[rec_per_page_pos] = record_per_page;
	b[rec_num_pos] = 0;
}

Record Table::getRecord(int rid)
{
	//if (rid > record_num)
	
	int pageid = rid / record_per_page + start_page;
	int pos_in_page = rid % record_per_page;
	int recordsize = Record::getRecordSize(columnNum, columnTypes);
	int index;
	BufType b = bpm->getPage(fid, pageid, index);
	return Record(columnNum, columnType, b + pos_in_page * recordsize);
}

void Table::insertRecord(Record &r)
{
	//NB
	int pageid = start_page;
	while (true) {
		int *available = page_available[pageid];
		for (int i = 0; i < 
	}
	
	//int pageid = record_num / record_per_page + start_page;
	//int pos_in_page = record_num % record_per_page;
	int recordsize = Record::getRecordSize(columnNum, columnTypes);
	int index;
	BufType b = bpm->getPage(fid, pageid, index);
	memcpy(b + pos_in_page * recordsize, r.data, recordsize);
}

Record Table::updateRecord(int rid)
{
	//if (rid > record_num)
	
	int pageid = rid / record_per_page + start_page;
	int pos_in_page = rid % record_per_page;
	int recordsize = Record::getRecordSize(columnNum, columnTypes);
	int index;
	BufType b = bpm->getPage(fid, pageid, index);
	//return Record(columnNum, columnType, b + pos_in_page * recordsize);
}

