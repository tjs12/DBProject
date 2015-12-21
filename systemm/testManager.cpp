#include "dbManager.h"
using namespace std; 
int main(){
	DbManager::getInstance()->createDb("");
	cout<<DbManager::getInstance()->showCurDbPath();
}
