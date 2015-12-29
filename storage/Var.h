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
	virtual Var *copy() = 0;
	virtual bool equal(Var *v) = 0;
	virtual bool greater_than(Var *v) = 0;
	virtual bool less_than(Var *v) = 0;
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

	Var *copy() {
		return new IntVar(val);
	}

	bool equal(Var *v) {
		if (v->type() == TYPE_INT) {
			if (((IntVar*)v) -> val == val) return true;
		}
		return false;
	}

	bool greater_than(Var *v) {
		if (v->type() == TYPE_INT) {
			if (((IntVar*)v) -> val < val) return true;
		}
		return false;
	}

	bool less_than(Var *v) {
		if (v->type() == TYPE_INT) {
			if (((IntVar*)v) -> val > val) return true;
		}
		return false;
	}

	int val;
	
};

class VarCharVar : public Var
{
public:
	VarCharVar(const char *v, int maxlength) {
		maxlen = maxlength;
		val = new char[maxlen+1];
		//TODO check for leng
		strcpy(val, v);
	}

	std::string toString() {
		return "'" + std::string(val) + "'";
	}

	Type type() {
		return Type(TYPE_CHAR, maxlen);
	}

	void writeToBuf(unsigned int *buf) {
		char *b1 = (char*) buf;
		strcpy(b1, val);
	}

	Var *copy() {
		return new VarCharVar(val, maxlen);
	}

	bool equal(Var *v) {
		if (v->type() == TYPE_CHAR) {
			return strcmp(val, ((VarCharVar*)v)->val) == 0;
		}
		return false;
	}

	bool greater_than(Var *v) {
		if (v->type() == TYPE_CHAR) {
			return false;//TODO
		}
		return false;
	}

	bool less_than(Var *v) {
		if (v->type() == TYPE_CHAR) {
			return false;//TODO
		}
		return false;
	}

	char *val;
	int maxlen;
};

class RealVar : public Var
{
public:
	RealVar(float v) {
		val = v;
	}

	std::string toString() {
		return to_string(val);
	}

	Type type() {
		return Type(TYPE_REAL, 0);
	}

	void writeToBuf(unsigned int *buf) {
		*((float*)buf) = val;
	}

	Var *copy() {
		return new RealVar(val);
	}

	bool equal(Var *v) {
		if (v->type() == Type(TYPE_REAL)) {
			if (((RealVar*)v) -> val == val) return true;
		}
		return false;
	}

	bool greater_than(Var *v) {
		if (v->type() == Type(TYPE_REAL)) {
			if (((IntVar*)v) -> val < val) return true;
		}
		return false;
	}

	bool less_than(Var *v) {
		if (v->type() == Type(TYPE_REAL)) {
			if (((IntVar*)v) -> val > val) return true;
		}
		return false;
	}
	
	float val;
	
};



#endif