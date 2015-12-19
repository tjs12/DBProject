#include "createDb.h" 
#include <iostream> 
#include <string>
#include <vector>
#define MAX_DB_AMOUNT
using namespace std;
class DbManager{							
	public:
		vector<string> dbNames; 			//维护所有数据库的名字 
		int createDb(string name);			//创建一个数据库，成功返回1，失败返回0 
		const string showCurDbName();		//返回当前数据库的名称
		const string showCurDbPath();		//返回当前数据库的绝对路径 
		static DbManager* getInstance();	//单例，全局唯一 
	private:
		DbManager(){}
		DbManager(const DbManager&){} 
		DbManager& operator =(const DbManager&){}
		static DbManager* manager;
		string curDbName;
		string curDbPath;
		
};

DbManager* DbManager::manager = new DbManager();

int DbManager::createDb(string name){
	int result = DbCreator::createFolder(name);	
	if(result==1){	//新目录创建成功 
		curDbName = DbCreator::dbName; curDbPath = DbCreator::dbPath;
		dbNames.push_back(curDbName);
	}
	return result;
}

DbManager* DbManager::getInstance(){
	if(manager==NULL){
		manager = new DbManager();
	}
	return manager;
}

const string DbManager::showCurDbName(){
	return curDbName;
}
const string DbManager::showCurDbPath(){
	return curDbPath;
}


//int main(){
//	string s = "789";
//	DbManager::getInstance()->createDb(s);
//	cout<<DbManager::getInstance()->showCurDbPath();
//	return 0;
//}
