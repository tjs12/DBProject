#ifndef CREATEDB_H
#define CREATEDB_H
#include <direct.h>
#include <io.h>
#include <windows.h>
#include <stdio.h>
#include <string>
#include <iostream>
#define rootpath ".\\dbs\\"
using namespace std;

class DbCreator{
	public:
		static bool exsist(string dir,const string name);
		static int createFolder(const string name);
		static string dbName;
		static string dbPath;
};

#endif
