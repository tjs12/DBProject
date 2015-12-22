#include "Var.h"

Var *Var::fromBuf(unsigned int *buf, Type type)
{
	if (type.type == TYPE_INT) {
		IntVar *i = new IntVar(*buf);
		return i;
	}
	else if (type.type == TYPE_CHAR) {
		//VarCharVar *c = new VarCharVar();
	}
	else {
		return 0;
	}
}