#ifndef TYPE_H
#define TYPE_H

#define TYPE_INT 0
#define TYPE_CHAR 1

class Type
{
public:
	int type;
	int setting;
	
	int size() {
		switch (type) {
		case TYPE_INT:
			return 1;
		case TYPE_CHAR:
			return (setting + 3) / 4;
		default:
			return 0;
		}
	}
}

#endif