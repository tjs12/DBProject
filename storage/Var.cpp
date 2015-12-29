#include "Var.h"

Var *Var::fromBuf(unsigned int *buf, Type type)
{
	if (type.type == TYPE_INT) {
		IntVar *i = new IntVar(*buf);
		return i;
	}
	else if (type.type == TYPE_REAL) {
		RealVar *i = new RealVar(*((float*)(*buf)));
		return i;
	}
	else if (type.type == TYPE_CHAR) {
		char *b1 = (char*) buf;
		VarCharVar *c = new VarCharVar(b1, strlen(b1));
		return c;
	}
	else {
		return 0;
	}
}