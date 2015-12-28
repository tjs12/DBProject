#ifndef IO_H
#define IO_H

#include <iostream>
#include <string>
#include "socket.h"

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
	SocketIO() {
		socket.Start();
		buf = new char[MAX_PATH];
		
	}
	void print(std::string str) {
		socket.Send(str.c_str());
	}
	char getchar() {
		if (pos == strlen(buf)) {
			pos = 0;
			socket.Receive(buf);
		}
		return buf[pos++];
	}
	private:
	Socket socket;
	char *buf;
};

#endif