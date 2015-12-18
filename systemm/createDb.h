#include <direct.h>
#include <io.h>
#include <stdio.h>
#include <string>
#include <iostream>
#define rootpath "E:/DBProject/dbs/"
using namespace std;
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
	if((lfDir = _findfirst("E:\\DBProject\\dbs\\*.*",&fileDir))==-1) {
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

//int main(){
////	 _finddata_t fileDir;
////   // char* dir="e:\\DBProject\\*.*";
////    long lfDir;
////     
////    if((lfDir = _findfirst("e:\\DBProject\\dbs\\*.*",&fileDir))==-1l)
////        printf("No file is found\n");
////    else{
////        printf("file list:\n");
////        do{
////            printf("%s\n",fileDir.name);
//// 
////        }while( _findnext( lfDir, &fileDir ) == 0 );
////    }
////    _findclose(lfDir);
// 	DbCreator::createFolder("hehe");
//	return 0;
//}
