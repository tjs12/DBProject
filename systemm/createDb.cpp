#include "createDb.h"

int m;

string DbCreator::dbName = "";
string DbCreator::dbPath ="";
bool DbCreator::exsist(string dir,const string name){
	dir +="*.*";
	_finddata_t fileDir;
	bool result = false;
	long lfDir;
	if((lfDir = _findfirst((dir).c_str(),&fileDir))==-1) {
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
	if(exsist(rootpath,name)){
		cout<<"Database already existed,create failed!"<<endl;
		return 0;
	}
	dbName =name;
	dbPath = rootpath+name;
	_mkdir((string(rootpath)+name).c_str());
	return 1;
} 