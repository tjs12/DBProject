#ifndef SOCKET_H
#define SOCKET_H

#ifdef _WIN32

#include <Windows.h>
#pragma comment(lib, "ws2_32.lib")

#include "rc.h"

#define  PORT 4000
#define  IP_ADDRESS "127.0.0.1"

class Socket
{
public:
	static RC Start();
	static RC Send(char*);
	static void Receive(char*);
private:
	static SOCKET ServerSocket, CientSocket;
	//static char *RecvBuffer;
	
	//static DWORD WINAPI ClientThread(LPVOID lpParameter);
};


#else


#endif

#endif
