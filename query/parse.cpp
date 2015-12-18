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

int parse_sql_delete(char &*cmd) {
	if (!strcnt(" FROM "))
		return ;
	char *relName = str;
	while (*str != ' ' && *str)
		str++;
	if (!*str)
		return ;
	*str = '\0';
	if (!strcnt(++str, "WHERE "))
		return ;
	vector<Condition> conditions(0);
	sstream sin(str);
	for (Condition cond; sin >> cond; conditions.push_back(cond));
	Delete(relName, conditions.size(), conditions.)
}