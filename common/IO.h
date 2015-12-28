#ifndef IO_H
#define IO_H

#include <iostream>
#include <string>

class IO
{
public:
	void virtual print(std::string) = 0;
	char virtual getchar() = 0;
};

class StdIO : public IO
{
public:
	void print(std::string str) {
		std::cout << str;
	}
	
	char getchar() {
		if (pos == buffer.length())
		{
			pos = 0;
			cin >> buffer;
		}
		return buffer[pos]++;
	}
	
private:
	string buffer;
	int pos;
};

class SocketIO : public IO
{
public:
	void print(std::string str) {
		;
	}
};

#endif