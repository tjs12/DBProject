char *strcnt(char &*str, char *key) {	// contrast key word
	int i = 0;
	while (key[i]) {
		if (*str != key[i])
			return NULL;
		i++;
	}

	return str += i;
}

// cmd should point to a writable memory
int parse_sql_statement(char *cmd) {
	int ret = -1;
	if (ret = parse_sql_action(cmd))
		return ret;
}

int parse_sql_action(char &*cmd) {
	if (strcnt(cmd, "SELECT"))
		parse_sql_select(cmd);
	else if (strcnt(cmd, "INSERT"))
		parse_sql_insert(cmd);
	else if (strcnt(cmd, "DELETE"))
		parse_sql_delete(cmd);
	else if (strcnt(cmd, "UPDATE"))
		parse_sql_update(cmd);
}

char *parse_sql_relName(char &*cmd) {
	char *relName = cmd;
	cmd = strchr(cmd, ' ');
	if (!cmd)
		return NULL;
	*cmd = '\0';
	cmd++;
	return relName;
}

vector<Condition> parse_sql_conditions(char &*cmd) {
	vector<Condition> conditions(0);
	sstream sin(cmd);
	for (Condition cond; sin >> cond; conditions.push_back(cond));
	return conditions;	// use C++11 to optimize
}

int parse_sql_delete(char &*cmd) {
	if (!strcnt(cmd, " FROM "))
		return ;
	char *relName = parse_sql_relName(cmd);
	if (!relName)
		return ;
	if (!strcnt(cmd, "WHERE "))
		return ;
	vector<Condition> conditions = parse_sql_conditions(cmd);	// use C+11 to optimize
	Delete(relName, conditions.size(), &conditions.front());
}

int parse_sql_update(char &*cmd) {
	if (!strcnt(cmd, " "))
		return ;
	char *relName = parse_sql_relName(cmd);
	if (!relName)
		return ;
	if (!strcnt(cmd, "SET "))
		return ;
	RelAttr updAttr = parse_sql_relAttr(cmd);
	int bIsValue = -1;
	for (char *str = cmd; *str && bIsValue < 0; str++)
		if (*str == '.')
			bIsValue = 0;
		else if (*str == '=')
			bIsValue = 1;
	if (bIsValue < 0)
		return ;
    RelAttr rhsRelAttr;
    Value rhsValue;
	sstream sin(cmd);
	if (bIsValue)
		sin >> rhsValue;
	else
		sin >> rhsRelAttr;
	vector<Condition> conditions = parse_sql_conditions(cmd);	// use C+11 to optimize
	Update(relName, updAttr, bIsValue, rhsRelAttr, rhsValue, conditions.size(), &conditions.front());
}

int parse_sql_insert() {
	if (!strcnt(cmd, " INTO "))
		return ;
	char *relName = parse_sql_relName(cmd);
	if (!relName)
		return ;
	vector<Value> values = parse_sql_values(cmd);
	Insert(relName, values.size(), &values.front());
}

int parse_sql_select() {
	
}