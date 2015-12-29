#include "dbManager.h"

DbManager* DbManager::manager = new DbManager();

int DbManager::createDb(const string name){
	int result = DbCreator::createFolder(name);	
	if(result==1){	//新目录创建成功 
		curDbName = DbCreator::dbName; curDbPath = DbCreator::dbPath;
		dbNames.push_back(curDbName);
	}
	return result;
}

int DbManager::createTable(vector<Type>types,vector<string>names,string name,int pri_key){
	string absoluteName = curDbPath+"\\"+name;
	Table* table = new Table();
	table->createTable(types,names,absoluteName,pri_key);
	delete table;
	return 0;
}
int DbManager::useDb(const string name){
	if(DbCreator::exsist(rootpath,name)){
		curDbName = name;
		curDbPath = rootpath+name;
		return 1;
	}
	cout<<"Database not found!"<<endl;
	return 0;
}

int DbManager::dropDb(const string name){
	if(DbCreator::exsist(rootpath,name)){
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
		if(name==curDbName) curDbName = ""; //如果删除的是当前数据库，则当前数据库名置为空
		return 1;
	}
	cout<<"Database does not exsist!"<<endl;
	return 0;
} 

void DbManager::showTables(){
	string dir = curDbPath+"\\";
	_finddata_t fileDir;
	long lfDir;
	if((lfDir=_findfirst((dir+"*.*").c_str(),&fileDir))==-1){_findclose(lfDir);}
		else{
			int m =0;		//前两个目录名字不输出
			do{
				if(m>1) cout<<fileDir.name<<endl;
				m++;
			}while(_findnext(lfDir,&fileDir)==0);
		}
		_findclose(lfDir);
}

int DbManager::dropTable(const string name){
	if(DbCreator::exsist(curDbPath+"\\",name)){
		DeleteFile((curDbPath+"\\"+name).c_str());
		return 1;
	}
	cout<<"table not found!"<<endl;
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