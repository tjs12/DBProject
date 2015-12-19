//	TODO: set init values
char *parse_sql_keyword(char &*cmd, char *kw) {	// contrast key word
	int i = 0;
	while (kw[i]) {
		if (cmd[i] != kw[i])
			return NULL;
		i++;
	}
	return cmd += i;
}

int parse_sql_name(char &*cmd) {
	while (('a' <= *cmd && *cmd <= 'z')
		|| ('A' <= *cmd && *cmd <= 'Z'))
		cmd++;
	int end = *cmd;
	*cmd = '\0';
	cmd++;
	return end;
}

int parse_sql_relAttr(char &*cmd, RelAttr &relAttr) {
	int end;
	relAttr.relName = cmd;
	end = parse_sql_name(cmd);
	if (end == '.') {
		relAttr.attrName = cmd;
		end = parse_sql_relName(cmd);
	}
	return end;
}

int parse_sql_relAttrs(char &*cmd, vector<RelAttr> &relAttrs) {
	int end;
	RelAttr relAttr;
	do {
		end = parse_sql_relAttr(cmd, relAttr);
		relAttrs.push_back(relAttr);
	} while (end == ',');
	return end;
}

int parse_sql_int(char &*cmd, int &data) {
	int end;
	data = 0;
	while ('0' <= *cmd && *cmd <= '9')
		data = data * 10 + (*cmd - '0');
	end = *cmd;
	*cmd = '\0';
	cmd++;
	return end;
}

int parse_sql_char(char &*cmd, char &*data, int &length) {
	int end;
	parse_sql_keyword(cmd, "'");
	data = cmd;
	cmd = strchr(cmd, '\'');
	if (!cmd)
		return -1;
	end = *cmd;
	length = (cmd - data) / sizeof(char);
	*cmd = '\0';
	cmd++;
	return end;
}

int parse_sql_value(char &*cmd, Value &value) {
	int end;
	if (*cmd = '\'') {
		value.type.type = TYPE_CHAR;
		char *data;
		end = parse_sql_char(cmd, data, value.type.setting);
		value.data = data;
	} else {
		value.type.type = TYPE_INT;
		int *data = new int;
		end = parse_sql_int(cmd, *data);
		value.data = data;
	}
	return end;
}

int parse_sql_values(char &*cmd, vector<Value> &values) {
	int end;
	if (!parse_sql_keyword(cmd, "VALUES ("))
		return -1;
	Value value;
	do {
		end = parse_sql_value(cmd, value);
		values.push_back(value)
	} while (end == ',');
	return end;
}

int parse_sql_condition(char &*cmd, Condition &condition) {
	int end;
	condition.op = parse_sql_relAttr(cmd, condition.lhsAttr);
	if (('a' <= *cmd && *cmd <= 'z')
		|| ('A' <= *cmd && *cmd <= 'Z')) {
		condition.bRhsIsAttr = 1;
		end = parse_sql_relAttr(cmd, condition.rhsAttr);
	} else {
		condition.bRhsIsAttr = 0;
		end = parse_sql_value(cmd, condition.rhsValue);
	}
	return end;
}

int parse_sql_conditions(char &*cmd, vector<Condition> &conditions) {
	int end;
	for (Condition cond; (end = parse_sql_condition(cmd, cond)) == ' '; conditions.push_back(cond))
		if (!parse_sql_keyword(cmd, "AND ")))
			return -1;
	return end;
}

// cmd should point to a writable memory
int parse_sql_statement(char *cmd) {
	return parse_sql_action(cmd);
}

int parse_sql_action(char &*cmd) {
	if (parse_sql_keyword(cmd, "SELECT"))
		return parse_sql_select(cmd);
	if (parse_sql_keyword(cmd, "INSERT"))
		return parse_sql_insert(cmd);
	if (parse_sql_keyword(cmd, "DELETE"))
		return parse_sql_delete(cmd);
	if (parse_sql_keyword(cmd, "UPDATE"))
		return parse_sql_update(cmd);
}

int parse_sql_delete(char &*cmd) {
	if (!parse_sql_keyword(cmd, " FROM "))
		return ;
	char *relName = cmd;
	if (parse_sql_name(cmd) != ' ')
		return ;
	if (!parse_sql_keyword(cmd, "WHERE "))
		return ;
	vector<Condition> conditions = parse_sql_conditions(cmd);	// use C+11 to optimize
	return Delete(relName, conditions.size(), &conditions.front());
}

int parse_sql_update(char &*cmd) {
	if (!parse_sql_keyword(cmd, " "))
		return ;
	char *relName = cmd;
	if (parse_sql_name(cmd) != ' ')
		return ;
	if (!parse_sql_keyword(cmd, "SET "))
		return ;
	Condition updCond;
	if (parse_sql_condition(cmd, updCond) != ' ')
		return ;
	if (!parse_sql_keyword(cmd, "WHERE "))
		return ;
	vector<Condition> conditions;
	parse_sql_conditions(cmd, conditions);
	return Update(relName,
		updCond.lhsAttr, updCond.bRhsIsAttr, updCond.rhsAttr, updCond.rhsValue,
		conditions.size(), &conditions.front());
}

int parse_sql_insert(char &*cmd) {
	if (!parse_sql_keyword(cmd, " INTO "))
		return ;
	char *relName = cmd;
	if (parse_sql_name(cmd) != ' ')
		return ;
	vector<Value> values;
	parse_sql_values(cmd, values);
	return Insert(relName, values.size(), &values.front());
}

int parse_sql_select(char &*cmd) {
	int end;
	vector<RelAttr> selAttrs;
	parse_sql_relAttrs(cmd, selAttrs);
	if (!parse_sql_keyword(cmd, "FROM "))
		return -1;
	vector<char *> relations(0);
	do {
		relations.push_back(cmd);
		end = parse_sql_name(cmd);
	} while (end == ',');
	if (!parse_sql_keyword(cmd, "WHERE "))
		return -1;
	vector<Condition> conditions;
	end = parse_sql_conditions(cmd, conditions);
	return Select(selAttr.size(), &selAttr.front(), 
		relations.size(), &relations.front(),
		conditions.size(), &conditions.front());
}