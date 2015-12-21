#ifndef DBMANAGER
#define DBMANAGER 
#include "createDb.h" 
#include <iostream> 
#include <string>
#include <vector>

#define MAX_DB_AMOUNT

using namespace std;
class DbManager{							
	public:
		vector<string> dbNames; 			//维护所有数据库的名字 
		int createDb(const string name);			//创建一个数据库，成功返回1，失败返回0 
		int useDb(const string name);		//切换数据库，成功返回1，失败返回0
		int dropDb(const string name);		//删除数据库，成功返回1，失败返回0 
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

DbManager* DbManager::manager = new DbManager();

int DbManager::createDb(const string name){
	int result = DbCreator::createFolder(name);	
	if(result==1){	//新目录创建成功 
		curDbName = DbCreator::dbName; curDbPath = DbCreator::dbPath;
		dbNames.push_back(curDbName);
	}
	return result;
}

int DbManager::useDb(const string name){
	if(DbCreator::exsist(name)){
		curDbName = name;
		curDbPath = rootpath+name;
		return 1;
	}
	cout<<"Database not found!"<<endl;
	return 0;
}

int DbManager::dropDb(const string name){
	if(DbCreator::exsist(name)){
		string dir = rootpath+name;
		_finddata_t fileDir;
		long lfDir;
		if((lfDir=_findfirst((dir+"\\"+"*.*").c_str(),&fileDir))==-1){_findclose(lfDir);}
		else{
			do{
				DeleteFile((dir+"\\"+fileDir.name).c_str());
			}while(_findnext(lfDir,&fileDir)==0);
		}
		_findclose(lfDir);
		rmdir(dir.c_str());

		return 1;
	}
	cout<<"Database does not exsist!"<<endl;
	return 0;
	
} 
DbManager* DbManager::getInstance(){
	if(manager==NULL){
		manager = new DbManager();
	}
	return manager;
}

const string DbManager::getCurDbName(){
	return curDbName;
}
const string DbManager::getCurDbPath(){
	return curDbPath;
}

int main(){
	DbManager* ex = DbManager::getInstance();
	ex->dropDb("orders");
//	cout<<ex->getCurDbPath();
	return 0;
}
#endif
