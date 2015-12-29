//	int parse_sql_statement(char *cmd) is used to parse query command
//	accept all
//	return -1 when failed, otherwise the cmd's running result
//	the mem poined by cmd should be writable
#include "qlm.h"


#define TEST 0
#if TEST
#include <cstdio>
using namespace std;
#define createDb(str) printf("createDb(%s)\n", str);
#define createTable printf("createTable()\n");
#define dropDb(str) printf("dropDb(%s)\n", str);
#define dropTable(str) printf("dropTable(%s)\n", str);
#define useDb(str) printf("useDb(%s)\n", str);
#define showTables() printf("showTables()\n");
#define Delete printf("Delete()\n");
#define Update printf("Update()\n");
#define Insert printf("Insert()\n");
#define Select printf("Select()\n");
#else
#include "../systemm/dbManager.h"
#include "../common/IO.h"

RC createDb(string str) {DbManager* m = DbManager::getInstance(); return m -> createDb(str);}
//RC createTable printf("createTable()\n");
RC dropDb(string str) {DbManager* m = DbManager::getInstance(); return m -> dropDb(str);}
RC dropTable(string str) {DbManager* m = DbManager::getInstance(); return m -> dropTable(str);}
RC useDb(string str) {DbManager* m = DbManager::getInstance(); return m -> useDb(str);};
void showTables() {DbManager* m = DbManager::getInstance(); m -> showTables();}

#endif

#define NO_COMMAND -2

bool parse_sql_keyword(char *&cmd, char *kw) {	// contrast key word
	int i = 0;
	while (kw[i]) {
		if (cmd[i] != kw[i])
			return false;
		i++;
	}
	cmd += i;
	return true;
}

int parse_sql_name(char *&cmd) {
	while (('a' <= *cmd && *cmd <= 'z')
		|| ('A' <= *cmd && *cmd <= 'Z'))
		cmd++;
	int end = *cmd;
	*cmd = '\0';
	cmd++;
	return end;
}

int parse_sql_relAttr(char *&cmd, RelAttr &relAttr) {
	int end;
	relAttr.relName = NULL;
	relAttr.attrName = cmd;
	end = parse_sql_name(cmd);
	if (end == '.') {
		relAttr.relName = relAttr.attrName;
		relAttr.attrName = cmd;
		end = parse_sql_name(cmd);
	}
	return end;
}

int parse_sql_relAttrs(char *&cmd, vector<RelAttr> &relAttrs) {
	int end;
	relAttrs.clear();
	if (parse_sql_keyword(cmd, "* "))
		return ' ';
	RelAttr relAttr;
	do {
		end = parse_sql_relAttr(cmd, relAttr);
		relAttrs.push_back(relAttr);
	} while (end == ',');
	return end;
}

int parse_sql_int(char *&cmd, int &data) {
	int end;
	for (data = 0; '0' <= *cmd && *cmd <= '9'; cmd++)
		data = data * 10 + (*cmd - '0');
	end = *cmd;
	*cmd = '\0';
	cmd++;
	return end;
}

int parse_sql_float(char *&cmd, float &data) {
	int end;
	for (data = 0; '0' <= *cmd && *cmd <= '9'; cmd++)
		data = data * 10 + (*cmd - '0');
	end = *cmd;
	*cmd = '\0';
	cmd++;
	if (end == '.') {
		for (float e = 1; '0' <= *cmd && *cmd <= '9'; cmd++) {
			e *= 0.1;
			data += (*cmd - '0') * e;
		}
		end = *cmd;
		*cmd = '\0';
		cmd++;
	}
	return end;
}

int parse_sql_char(char *&cmd, char *&data, int &length) {
	int end;
	data = cmd;
	cmd = strchr(cmd, '\'');
	if (!cmd)
		return -1;
	length = (cmd - data) / sizeof(char);
	*cmd = '\0';
	cmd++;
	end = *cmd;
	cmd++;
	return end;
}

int parse_sql_type(char *&cmd, Type &type) {
	int end;
	if (parse_sql_keyword(cmd, "int("))
		type.type = TYPE_INT;
	else if (parse_sql_keyword(cmd, "varchar("))
		type.type = TYPE_CHAR;
	else if (parse_sql_keyword(cmd, "float("))
		type.type = TYPE_FLOAT;
	else
		return -1;
	end = parse_sql_int(cmd, type.setting);
	return end;
}

int parse_sql_value(char *&cmd, Value &value) {
	int end;
	if (parse_sql_keyword(cmd, "'")) {
		value.type.type = TYPE_CHAR;
		char *data;
		end = parse_sql_char(cmd, data, value.type.setting);
		value.data = data;
	} else if ('0' <= *cmd && *cmd <= '9') {
		bool Int = true;
		for (char *str = cmd; *str && Int; str++)
			if (*str == '.')
				Int = false;
			else if (!('0' <= *str && *str <= '9'))
				break;
		if (Int) {
			value.type.type = TYPE_INT;
			value.type.setting = 0;
			int *data = new int;
			end = parse_sql_int(cmd, *data);
			value.data = data;
		} else {
			value.type.type = TYPE_FLOAT;
			value.type.setting = 0;
			float *data = new float;
			end = parse_sql_float(cmd, *data);
			value.data = data;
		}
	}
	return end;
}

int parse_sql_values(char *&cmd, vector<Value> &values) {
	int end;
	if (!parse_sql_keyword(cmd, "VALUES ("))
		return -1;
	values.clear();
	Value value;
	do {
		end = parse_sql_value(cmd, value);
		values.push_back(value);
	} while (end == ',');
	return end;
}

int parse_sql_condition(char *&cmd, Condition &condition) {
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

int parse_sql_conditions(char *&cmd, vector<Condition> &conditions) {
	int end;
	conditions.clear();
	Condition cond;
	do {
		end = parse_sql_condition(cmd, cond);
		conditions.push_back(cond);
		if (end == ' ' && !parse_sql_keyword(cmd, "AND "))
			return -1;
	} while (end == ' ');
	return end;
}

int parse_sql_create(char *&cmd) {
	if (parse_sql_keyword(cmd, " DATABASE ")) {
		char *dbName = cmd;
		parse_sql_name(cmd);
		return createDb(dbName);
	}
	if (parse_sql_keyword(cmd, " TABLE ")) {
		char *tbName = cmd;
		if (parse_sql_name(cmd) != ' ')
			return -1;
		if (!parse_sql_keyword(cmd, "("))
			return -1;
		vector<Type> types(0);
		vector<string> names(0);
		while (!parse_sql_keyword(cmd, "PRIMARY KEY(")) {
			char *id = cmd;
			parse_sql_name(cmd);
			names.push_back(id);
			Type type;
			parse_sql_type(cmd, type);
			types.push_back(type);
			parse_sql_keyword(cmd, " NOT NULL");
			if (!parse_sql_keyword(cmd, ","))
				return -1;
		}
		char *pkName = cmd;
		parse_sql_name(cmd);
		int priKey = distance(names.begin(), find(names.begin(), names.end(), pkName));
#if TEST == 0
		return DbManager::getInstance() -> createTable(types, names, tbName, priKey);
#else
		return createTable(types, names, tbName, priKey);
#endif
	}
	return -1;
}

int parse_sql_drop(char *&cmd) {
	if (parse_sql_keyword(cmd, " DATABASE ")) {
		char *dbName = cmd;
		parse_sql_name(cmd);
		return dropDb(dbName);
	}
	if (parse_sql_keyword(cmd, " TABLE ")) {
		char *tbName = cmd;
		parse_sql_name(cmd);
		return dropTable(tbName);
	}
	return -1;
}

int parse_sql_use(char *&cmd) {
	if (!parse_sql_keyword(cmd, " "))
		return -1;
	char *dbName = cmd;
	parse_sql_name(cmd);
	return useDb(dbName);
}

int parse_sql_show(char *&cmd) {
	if (!parse_sql_keyword(cmd, " TABLES"))
		return -1;
	/*return */showTables();
}

int parse_sql_desc(char *&cmd) {
	if (!parse_sql_keyword(cmd, " "))
		return -1;
	char *tbName = cmd;
	parse_sql_name(cmd);
	return 0;(tbName);
}

int parse_sql_delete(char *&cmd) {
	if (!parse_sql_keyword(cmd, " FROM "))
		return -1;
	char *relName = cmd;
	if (parse_sql_name(cmd) != ' ')
		return -1;
	if (!parse_sql_keyword(cmd, "WHERE "))
		return -1;
	vector<Condition> conditions(0);
    parse_sql_conditions(cmd, conditions);	// use C+11 to optimize
#if test
	return Delete(relName, conditions.size(), &conditions.front());
#else
	return QL_Manager::getInst() -> Delete(relName, conditions.size(), &conditions.front());
#endif
}

int parse_sql_update(char *&cmd) {
	if (!parse_sql_keyword(cmd, " "))
		return -1;
	char *relName = cmd;
	if (parse_sql_name(cmd) != ' ')
		return -1;
	if (!parse_sql_keyword(cmd, "SET "))
		return -1;
	Condition updCond;
	if (parse_sql_condition(cmd, updCond) != ' ')
		return -1;
	if (!parse_sql_keyword(cmd, "WHERE "))
		return -1;
	vector<Condition> conditions(0);
	parse_sql_conditions(cmd, conditions);
#if test
	return Update(relName,
		updCond.lhsAttr, updCond.bRhsIsAttr, updCond.rhsAttr, updCond.rhsValue,
		conditions.size(), &conditions.front());
#else
	return QL_Manager::getInst() -> Update(relName,
		updCond.lhsAttr, updCond.bRhsIsAttr, updCond.rhsAttr, updCond.rhsValue,
		conditions.size(), &conditions.front());
#endif
	
}

int parse_sql_insert(char *&cmd) {
	if (!parse_sql_keyword(cmd, " INTO "))
		return -1;
	char *relName = cmd;
	if (parse_sql_name(cmd) != ' ')
		return -1;
	vector<Value> values(0);
	parse_sql_values(cmd, values);
	return QL_Manager::getInst() -> Insert(relName, values.size(), &values.front());
}

int parse_sql_select(char *&cmd) {
	int end;
	vector<RelAttr> selAttrs(0);
	if (!parse_sql_keyword(cmd, " "))
	   return -1;
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
	return QL_Manager::getInst() -> Select(selAttrs.size(), selAttrs.size()==0? 0: &selAttrs.front(), 
		relations.size(), &relations.front(),
		conditions.size(), conditions.size()==0 ? 0 : &conditions.front());
}

// cmd should point to a writable memory
int parse_sql_statement(char *cmd) {
	if (parse_sql_keyword(cmd, "CREATE"))
		return parse_sql_create(cmd);
	if (parse_sql_keyword(cmd, "DROP"))
		return parse_sql_drop(cmd);
	if (parse_sql_keyword(cmd, "USE"))
		return parse_sql_use(cmd);
	if (parse_sql_keyword(cmd, "SHOW"))
		return parse_sql_show(cmd);
	if (parse_sql_keyword(cmd, "DESC"))
		return parse_sql_desc(cmd);
	if (parse_sql_keyword(cmd, "SELECT"))
		return parse_sql_select(cmd);
	if (parse_sql_keyword(cmd, "INSERT"))
		return parse_sql_insert(cmd);
	if (parse_sql_keyword(cmd, "DELETE"))
		return parse_sql_delete(cmd);
	if (parse_sql_keyword(cmd, "UPDATE"))
		return parse_sql_update(cmd);
	return -1;
}


int parse_entrance(char c) {
	static string cmd = "";
	int ret = NO_COMMAND;	// not run the cmd
	char *temp;
	switch (c) {
		case ';':
			cmd += c;
			temp = new char[cmd.length()+1];
			strcpy(temp, cmd.c_str());
			ret = parse_sql_statement(temp);
			cmd = "";
			delete temp;
			return ret;
		case ' ':
		case '\t':
		case '\n':
			if (cmd == "" || cmd.back() == ' ' || cmd.back() == '(' || cmd.back() == ',')
				return ret;
			cmd += ' ';
			return ret;
		default:
			cmd += c;
			return ret;
	}
}

int main() {
	char *cmd = new char[1 << 8];
	SocketIO io;
	QL_Manager::getInst() -> setIO(&io);
	char temp;
	int ret;
	while (temp = io.getchar()) {
		//printf("cmd's running result: %d\n------------------------\n", parse_entrance(temp));
		ret = parse_entrance(temp);
		if (ret != NO_COMMAND) io.print("\n" + to_string(ret) + "#end");
	}
	delete []cmd;
}

