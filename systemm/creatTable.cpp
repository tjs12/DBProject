#include "../storage/Table.h"
#include <iostream>
using namespace std;
class tableCreator{
	static int createTable(const string absolutePath);	//创建一个表,成功返回1，失败返回0
};

int tableCreator::creatTable(const string absolutePath,int colNums,){
	Table* table = new Table();
	table->creatTable()
}
int main(){

	return 0;
}
