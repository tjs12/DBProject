#include "createDb.h" 
#include <iostream> 
#include <string>
#include <vector>
#define MAX_DB_AMOUNT
using namespace std;
class DbManager{							
	public:
		vector<string> dbNames; 			//ά���������ݿ������ 
		int createDb(string name);			//����һ�����ݿ⣬�ɹ�����1��ʧ�ܷ���0 
		const string showCurDbName();		//���ص�ǰ���ݿ������
		const string showCurDbPath();		//���ص�ǰ���ݿ�ľ���·�� 
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

int DbManager::createDb(string name){
	int result = DbCreator::createFolder(name);	
	if(result==1){	//��Ŀ¼�����ɹ� 
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
