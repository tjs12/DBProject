#include "dbManager.h"
using namespace std; 
int main(){
	DbManager* m = DbManager::getInstance();
	m->useDb("hgee");
	m->showTables();
	int k;cin>>k;
}
