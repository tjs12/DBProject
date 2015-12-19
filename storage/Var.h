#ifndef VAR_H
#define VAR_H

#include <string>
#include "Type.h"

using namespace std;

class Var
{
public:
	virtual std::string toString() = 0;
	virtual Type type() = 0;
	virtual void writeToBuf(unsigned int *buf) = 0;

	static Var *fromBuf(unsigned int *buf, Type type);
	
};

class IntVar : public Var
{
public:
	IntVar(int v) {
		val = v;
	}

	std::string toString() {
		return to_string(val);
	}
	Type type() {
		return Type(TYPE_INT, 0);
	}

	void writeToBuf(unsigned int *buf) {
		*buf = val;
	}

	

	int val;
	
};

class VarCharVar : public Var
{
public:
	VarCharVar(const char *v, int maxlength) {
		maxlen = maxlength;
		val = new char[maxlen];
		strcpy(val, v);
	}

	std::string toString() {
		return std::string(val);
	}
	 Type type() {
		 return Type(TYPE_CHAR, maxlen);
	 }

	char *val;
	int maxlen;
};





#endif