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
		vector<string> dbNames; 			//ά���������ݿ������ 
		int createDb(const string name);	//����һ�����ݿ⣬�ɹ�����1��ʧ�ܷ���0 
		int createTable(vector<Type>types,vector<string>names,string name,int pri_key); //������
		int useDb(const string name);		//�л����ݿ⣬�ɹ�����1��ʧ�ܷ���0
		int dropDb(const string name);		//ɾ�����ݿ⣬�ɹ�����1��ʧ�ܷ���0 
		void showTables();					//��ʾ��ǰ���ݿ��µ����б�
		int dropTable(const string name);	//ɾ�����ɹ�����1��ʧ��0
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



#endif
