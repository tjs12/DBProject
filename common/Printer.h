#ifndef PRINTER_H
#define PRINTER_H

#include <iostream>
#include <string>

class Printer
{
public:
	void virtual print(std::string) = 0;
};

class StdPrinter : public Printer
{
public:
	void print(std::string str) {
		std::cout << str;
	}
};

class SocketPrinter : public Printer
{
public:
	void print(std::string str) {
		;
	}
};

#endif