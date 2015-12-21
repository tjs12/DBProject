#ifndef CREATEDB_H
#define CREATEDB_H
#include <direct.h>
#include <io.h>
#include <windows.h>
#include <stdio.h>
#include <string>
#include <iostream>
#define rootpath "E:\\DBProject\\dbs\\"
using namespace std;
int m;
class DbCreator{
	public:
		static bool exsist(const string name);
		static int createFolder(const string name);
		static string dbName;
		static string dbPath;
};
string DbCreator::dbName = "";
string DbCreator::dbPath ="";
bool DbCreator::exsist(const string name){
	_finddata_t fileDir;
	bool result = false;
	long lfDir;
	if((lfDir = _findfirst((string(rootpath)+"*.*").c_str(),&fileDir))==-1) {
		  _findclose(lfDir);
		return result;
	}
	else{
		do{
			if(name ==fileDir.name){
				result = true;
				break;
			}
		}while(_findnext(lfDir,&fileDir)==0);
	}
	  _findclose(lfDir);
	return result;
}
int DbCreator::createFolder(const string name){
	if(exsist(name)){
		cout<<"Database already existed,create failed!"<<endl;
		return 0;
	}
	dbName =name;
	dbPath = rootpath+name;
	_mkdir((string(rootpath)+name).c_str());
	return 1;
} 
#endif
