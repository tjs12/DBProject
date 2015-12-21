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
		vector<string> dbNames; 			//ά���������ݿ������ 
		int createDb(const string name);			//����һ�����ݿ⣬�ɹ�����1��ʧ�ܷ���0 
		int useDb(const string name);		//�л����ݿ⣬�ɹ�����1��ʧ�ܷ���0
		int dropDb(const string name);		//ɾ�����ݿ⣬�ɹ�����1��ʧ�ܷ���0 
		const string getCurDbName();		//���ص�ǰ���ݿ������
		const string getCurDbPath();		//���ص�ǰ���ݿ�ľ���·�� 
		static DbManager* getInstance();	//������ȫ��Ψһ 
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
	if(result==1){	//��Ŀ¼�����ɹ� 
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
