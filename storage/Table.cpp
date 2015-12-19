#include "Table.h"

#include <cstring>

using namespace std;

RC Table::createTable(vector<Type> &col_type, vector<string> &col_names, string name)
{
	if (col_type.size() != col_names.size()) return RETURN_FAILED;
	
	tableName = name;
	columnNum = col_type.size();
	/*columnTypes = new Type[col_num];
	columnNames = new string[col_num];
	for (int i = 0; i < col_num; i++) {
		columnTypes[i] = col_type[i];
		columnNames[i] = col_names[i];
	}*/
	columnNames = col_names;
	columnTypes = col_type;
	
	
	fm = new FileManager();
	bpm = new BufPageManager(fm);
	fm -> createFile(name.c_str());
	fm -> openFile(name.c_str(), fid);
	
	record_num = 0;
	first_page_available = 1;
	start_page = 1;

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
	fm -> openFile(name.c_str(), fid);
	tableName = name;
	int index;
	BufType b = bpm->getPage(fid, 0, index);
	//read header
	columnNum = b[COLUMN_NUM];
	//columnTypes = new Type[columnNum];
	//columnNames = new string[columnNum];
	columnTypes = vector<Type>(3, Type());
	columnNames = vector<string>(3, string(""));
	
	int col_settings_pos = COLUMN_TYPES + columnNum;
	int col_name_pos = col_settings_pos + columnNum;
	int rec_per_page_pos = col_name_pos + columnNum * MAX_COL_NAME_LEN / 4;
	int rec_num_pos = rec_per_page_pos + 1;
	int first_free_pagen_pos = rec_num_pos + 1;
	int start_page_pos = first_free_pagen_pos + 1;
	
	for (int i = 0; i < columnNum; i++) {
		columnTypes[i].type = b[COLUMN_TYPES + i];
		columnTypes[i].setting = b[col_settings_pos + i]; //TODO
		char *temp_str = new char[MAX_COL_NAME_LEN];
		memcpy(temp_str, &b[col_name_pos + i * MAX_COL_NAME_LEN / 4], MAX_COL_NAME_LEN);
		columnNames[i] = string(temp_str);
		delete []temp_str;
	}
	record_per_page = b[rec_per_page_pos];
	page_header_size = (record_per_page + 31)/32 + PAGE_BITMAP_POS;

	record_num = b[rec_num_pos];
	first_page_available = b[first_free_pagen_pos];
	start_page = b[start_page_pos];
}


void Table::make_header(BufType b)
{
	//find the positions of the items in the header
	int col_settings_pos = COLUMN_TYPES + columnNum;
	int col_name_pos = col_settings_pos + columnNum;
	int rec_per_page_pos = col_name_pos + columnNum * MAX_COL_NAME_LEN / 4;
	int rec_num_pos = rec_per_page_pos + 1;
	int first_free_pagen_pos = rec_num_pos + 1;
	int start_page_pos = first_free_pagen_pos + 1;
	
	b[COLUMN_NUM] = columnNum;
	for (int i = 0; i < columnNum; i++) {
		b[COLUMN_TYPES + i] = columnTypes[i].type;
		b[col_settings_pos + i] = columnTypes[i].setting;
		
		char *c_str = new char[columnNames[i].length() + 1];
		strcpy(c_str, columnNames[i].c_str());
		memcpy(b + (col_name_pos + i * MAX_COL_NAME_LEN / 4), c_str, strlen(c_str) + 1);
		delete []c_str;
	}
	
	record_per_page = 32 * (PAGE_INT_NUM)/ (32 * Record::getRecordSize(columnNum, columnTypes) + 1);
	page_header_size = (record_per_page + 31)/32 + PAGE_BITMAP_POS;

	b[rec_per_page_pos] = record_per_page;
	b[rec_num_pos] = record_num;
	b[first_free_pagen_pos] = first_page_available;
	b[start_page_pos] = start_page;
}

RC Table::getRecord(int rid, Record &rec)
{
	//if (rid > record_num)
	
	int pageid = rid / record_per_page + start_page;
	int pos_in_page = rid % record_per_page + page_header_size;
	int recordsize = Record::getRecordSize(columnNum, columnTypes);
	int index;
	BufType b = bpm->getPage(fid, pageid, index);
	if (!is_record_buf(pos_in_page, b))
		return RETURN_FAILED;
	unsigned int *recdata = new unsigned int[recordsize];
	memcpy(recdata, &b[pos_in_page * recordsize], recordsize * sizeof(unsigned int));
	rec = Record(columnTypes/*, recdata*/);
	rec.fromData(recdata);
	return RETURN_SUCCEED;
}

int Table::insertRecord(Record &r)
{
	//NB
	int pageid = first_page_available;
	int index;
	
	BufType b = bpm->getPage(fid, pageid, index);
	
	check_page_validity(b);
	
	int pos_in_page = page_header_size;
	for (; pos_in_page < record_per_page; pos_in_page++) {
		if (!is_record_buf(pos_in_page, b))
			break;
	}
	int recordsize = Record::getRecordSize(columnNum, columnTypes);
	
	memcpy(b + pos_in_page * recordsize, r.getData(), recordsize * sizeof(unsigned int));
	
	b[pos_in_page / 32 + PAGE_BITMAP_POS] = b[pos_in_page / 32 + PAGE_BITMAP_POS] | (1 << (pos_in_page % 32)); //Set free space flag
	
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
	record_num++;
	bpm -> markDirty(index);
	return pos_in_page - page_header_size + record_per_page * (pageid - start_page);
}

int Table::updateRecord(int rid, Record &r, Record &mask)
{
	//if (rid > record_num)
	
	int pageid = rid / record_per_page + start_page;
	int pos_in_page = rid % record_per_page + page_header_size;
	int recordsize = Record::getRecordSize(columnNum, columnTypes);
	int index;
	BufType b = bpm->getPage(fid, pageid, index);
	
	if (!is_record_buf(pos_in_page, b))
		return RETURN_FAILED; //Error! Record doesn't exist
	
	for (int i = 0; i < Record::getRecordSize(columnNum, columnTypes); i++) {
		if (mask.getData()[i] != 0) 
			b[pos_in_page + i + PAGE_BITMAP_POS] = r.getData()[i];
	}
	//return Record(columnNum, columnType, b + pos_in_page * recordsize);
	return RETURN_SUCCEED;
}

int Table::deleteRecord(int rid)
{
	int pageid = rid / record_per_page + start_page;
	int pos_in_page = rid % record_per_page + page_header_size;
	int recordsize = Record::getRecordSize(columnNum, columnTypes);
	int index;
	BufType b = bpm->getPage(fid, pageid, index);
	
	if (!is_record_buf(pos_in_page, b))
		return RETURN_FAILED; //Error! Record doesn't exist
	
	b[pos_in_page / 32 + PAGE_BITMAP_POS] = b[pos_in_page / 32 + PAGE_BITMAP_POS] & ~(1 << (pos_in_page % 32));
	
	record_num--;

	return RETURN_SUCCEED;
	//TODO: when shall we delete pages?
}

bool Table::isRecord(int rid)
{
	int pageid = rid / record_per_page + start_page;
	int pos_in_page = rid % record_per_page + page_header_size;
	int recordsize = Record::getRecordSize(columnNum, columnTypes);
	int index;
	BufType b = bpm->getPage(fid, pageid, index);
	return is_record_buf(pos_in_page, b);
}

bool Table::is_record_buf(int pos_in_page, BufType b)
{
	return (b[pos_in_page / 32 + PAGE_BITMAP_POS] & (1 << (pos_in_page % 32))) >> (pos_in_page % 32);
}

void Table::check_page_validity(BufType b, int pgnum)
{
	//check if the page exists, and make the page header if not.
	if (b[PAGE_HEADER_VALID] == PAGE_VALID)
		return;
		
	//for (int i = 0; i < (record_per_page + 7) / 8; i++) {
	for (int i = 0; i < PAGE_INT_NUM; i++) {
		b[i] = 0;
	}
	
	b[NEXT_PAGE_AVAILABLE] = pgnum + 1;
	b[PAGE_HEADER_VALID] = PAGE_VALID;
	//Others...
}

Table::~Table()
{
	int index;
	make_header(bpm->getPage(fid, 0, index));
	bpm -> markDirty(index);
	bpm -> close();
	delete bpm;
	delete fm;

}