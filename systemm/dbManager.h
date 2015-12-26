#ifndef DBMANAGER
#define DBMANAGER 
#include "createDb.h" 
#include "../storage/Table.h"
#include "../storage/TableIterator.h"
#include <iostream> 
#include <string>
#include <vector>

#define MAX_DB_AMOUNT

using namespace std;
class DbManager{							
	public:
		vector<string> dbNames; 			//维护所有数据库的名字 
		int createDb(const string name);	//创建一个数据库，成功返回1，失败返回0 
		int createTable(vector<Type>types,vector<string>names,string name,int pri_key); //创建表
		int useDb(const string name);		//切换数据库，成功返回1，失败返回0
		int dropDb(const string name);		//删除数据库，成功返回1，失败返回0 
		void showTables();					//显示当前数据库下的所有表
		int dropTable(const string name);	//删除表，成功返回1，失败0
		const string getCurDbName();		//返回当前数据库的名称
		const string getCurDbPath();		//返回当前数据库的绝对路径 
		static DbManager* getInstance();	//单例，全局唯一 
	private:
		DbManager(){}
		DbManager(const DbManager&){} 
		DbManager& operator =(const DbManager&){}
		static DbManager* manager;
		string curDbName;
		string curDbPath;		
};



#endif
