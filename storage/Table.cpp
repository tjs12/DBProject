#include "Table.h"
using namespace std;

void Table::createTable(int col_num, Type *col_type, string *col_names, string name)
{
	tableName = name;
	columnNum = col_num;
	columnTypes = new Type[col_num];
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
	
	//PAGE_HEADER_SIZE = PAGE_HEADER_FIXED_LEN + ((PAGE_INT_NUM + columnTypes.size() - 1) / columnTypes.size() + 31) / 32;
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
	columnTypes = new Type[columnNum];
	columnNames = new string[columnNum];
	
	int col_settings_pos = COLUMN_TYPES + columnNum;
	int col_name_pos = col_settings_pos + columnNum;
	int rec_per_page_pos = col_name_pos + columnNum * MAX_STRING_LEN / 4;
	int rec_num_pos = rec_per_page_pos + 1;
	
	for (int i = 0; i < col_num; i++) {
		columnTypes[i].type = b[COLUMN_TYPES + i];
		columnType[i].setting = b[col_settings_pos + i]; //TODO
		columnNames[i] = new string((char*)b[col_name_pos + i * MAX_STRING_LEN / 4]);
	}
	record_per_page = b[rec_per_page_pos];
	record_num = b[rec_num_pos];
	
}


Table::make_header(BufType b)
{
	//find the positions of the items in the header
	int col_settings_pos = COLUMN_TYPES + columnNum;
	int col_name_pos = COLUMN_TYPES + columnNum;
	int rec_per_page_pos = col_name_pos + columnNum * MAX_STRING_LEN / 4;
	int rec_num_pos = rec_per_page_pos + 1;
	int first_free_pagen_pos = rec_num_pos + 1;
	
	b[COLUMN_NUM] = columnNum;
	for (int i = 0; i < col_num; i++) {
		b[COLUMN_TYPES + i] = columnTypes[i].type;
		b[col_settings_pos + i] = columnType[i].setting;
		
		char *c_str = columnNames[i].c_str();
		memcpy(b[col_name_pos + i * MAX_STRING_LEN / 4], c_str, strlen(c_str) + 1);
		delete []c_str;
	}
	
	record_per_page = (PAGE_INT_NUM - PAGE_HEADER_SIZE)/ Record::getRecordSize(columnNum, columnTypes);
	b[rec_per_page_pos] = record_per_page;
	b[rec_num_pos] = 0;
	b[first_free_pagen_pos] = 1;
}

Record Table::getRecord(int rid)
{
	//if (rid > record_num)
	
	int pageid = rid / record_per_page + start_page;
	int pos_in_page = rid % record_per_page + PAGE_HEADER_SIZE;
	int recordsize = Record::getRecordSize(columnNum, columnTypes);
	int index;
	BufType b = bpm->getPage(fid, pageid, index);
	return Record(columnNum, columnType, b + pos_in_page * recordsize);
}

int Table::insertRecord(Record &r)
{
	//NB
	int pageid = first_page_available;
	int index;
	
	BufType b = bpm->getPage(fid, pageid, index);
	
	check_page_validity(b);
	
	int pos_in_page = 0;
	for (; pos_in_page < record_per_page; pos_in_page++) {
		if (!is_record_buf(pos_in_page, b))
			break;
	}
	int recordsize = Record::getRecordSize(columnNum, columnTypes);
	
	memcpy(b + pos_in_page * recordsize, r.data, recordsize);
	
	b[pos_in_page / 32] = b[pos_in_page / 32] | (1 << (pos_in_page % 32)); //Set free space flag
	
	bool still_free = false;
	for (int i = pos_in_page + 1; i < record_per_page; i++)
		if (!is_record_buf(i, b)) {
			still_free = true;
			break;
		}
	
	if (!still_free) {
		first_page_available = b[NEXT_PAGE_AVAILABLE];
		BufType b_header = bpm->getPage(fid, 0, index);
		make_header(b_header);//save_header(); TODO
	}
	
}

int Table::updateRecord(int rid, Record &r, Record &mask)
{
	//if (rid > record_num)
	
	int pageid = rid / record_per_page + start_page;
	int pos_in_page = rid % record_per_page + PAGE_HEADER_SIZE;
	int recordsize = Record::getRecordSize(columnNum, columnTypes);
	int index;
	BufType b = bpm->getPage(fid, pageid, index);
	
	if (!is_record_buf(pos_in_page, b))
		return RETURN_FAILED; //Error! Record doesn't exist
	
	for (int i = 0; i < Record::getRecordSize(columnNum, columnTypes); i++) {
		if (mask.data[i] != 0) 
			b[pos_in_page + i] = r.data[i];
	}
	//return Record(columnNum, columnType, b + pos_in_page * recordsize);
	return RETURN_SUCCEED;
}

int Table::deleteRecord(int rid)
{
	int pageid = rid / record_per_page + start_page;
	int pos_in_page = rid % record_per_page + PAGE_HEADER_SIZE;
	int recordsize = Record::getRecordSize(columnNum, columnTypes);
	int index;
	BufType b = bpm->getPage(fid, pageid, index);
	
	if (!is_record_buf(pos_in_page, b))
		return RETURN_FAILED; //Error! Record doesn't exist
	
	b[pos_in_page / 32] = b[pos_in_page / 32] & ~(1 << (pos_in_page % 32));
	
	return RETURN_SUCCEED;
	//TODO: when shall we delete pages?
}

bool Table::isRecord(int rid)
{
	int pageid = rid / record_per_page + start_page;
	int pos_in_page = rid % record_per_page + PAGE_HEADER_SIZE;
	int recordsize = Record::getRecordSize(columnNum, columnTypes);
	int index;
	BufType b = bpm->getPage(fid, pageid, index);
	return is_record_buf(pos_in_page, b);
}

bool Table::is_record_buf(int pos_in_page, BufType b)
{
	return (b[pos_in_page / 32] & (1 << (pos_in_page % 32))) >> (pos_in_page % 32);
}

void Table::check_page_validity(BufType b, int pgnum = 1)
{
	//check if the page exists, and make the page header if not.
	if (b[PAGE_HEADER_VALID] == PAGE_VALID)
		return;
		
	for (int i = 0; i < (record_per_page + 7) / 8; i++) {
		b[i] = 0;
	}
	
	b[NEXT_PAGE_AVAILABLE] = pgnum + 1;
	b[PAGE_HEADER_VALID] = PAGE_VALID
	//Others...
}