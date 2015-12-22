#ifndef TABLEITERATOR_H
#define TABLEITERATOR_H

#include "Table.h"

class TableIterator
{
public:
	TableIterator(Table *table) {
		t = table;
		num = t -> page_header_size;
		gotoBegin();
	}

	Record begin() {
		int i = 0;
		while (i < t -> max_rid) {
			if (t -> isRecord(i)) break;
			i++;
		}
		Record r;
		t -> getRecord(i, r);
		return r;
	}

	Record current()
	{
		if (!t -> isRecord(num)) {
			do{
				num++;
				if (t -> isRecord(num)) break;
			}
			while (num < t -> max_rid);
		}
		Record r;
		t -> getRecord(num, r);
		return r;
	}

	void gotoNext()
	{
		if (num == t -> max_rid) return;
		do {
			num++;
			if (t -> isRecord(num)) break;
		}
		while (num < t -> max_rid);
	}

	void gotoBegin()
	{
		num = t->page_header_size;
		while (num < t -> max_rid) {
			if (t -> isRecord(num)) break;
			num++;
		}
	}

	bool isEnd()
	{
		return num == t->max_rid;
	}
private:
	int num;
	Table *t;
};

#endif
