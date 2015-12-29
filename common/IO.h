#ifndef IO_H
#define IO_H

#include <iostream>
#include <string>
#include "socket.h"

using namespace std;

class IO
{
public:
	void virtual print(std::string) = 0;
	char virtual getchar() = 0;
};

class StdIO : public IO
{
public:
	StdIO()
	{
		pos = 0;
	}
	void print(std::string str) {
		std::cout << str;
	}
	
	char getchar() {
		if (pos == Buffer.length())
		{
			pos = 0;
			char *temp = new char[MAX_PATH];
			std::cin.getline(temp, MAX_PATH);
			Buffer = temp;
			delete []temp;
		}
		return Buffer[pos++];
	}
	
private:
	string Buffer;
	int pos;
};


class SocketIO : public IO
{
public:
	SocketIO() {
		socket.Start();
		buf = new char[MAX_PATH];
		memset(buf, 0, sizeof(char) * MAX_PATH);
		pos = 0;
	}
	void print(std::string str) {
		char *temp = new char[str.length() + 1];
		strcpy(temp, str.c_str());
		cout << "Print:\n    \"" << temp << "\"" << endl;
		socket.Send(temp);
		delete []temp;
	}
	char getchar() {
		if (pos == strlen(buf)) {
			pos = 0;
			socket.Receive(buf);
		}
		cout << "Received: " << pos << "\n    \"" <<buf[pos] << "\"" << endl;
		return buf[pos++];
	}
	private:
	Socket socket;
	char *buf;
	int pos;
};

#endif