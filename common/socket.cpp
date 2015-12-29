#include "socket.h"

static char *RecvBuffer = 0;


SOCKET Socket::ServerSocket = 0;
SOCKET Socket::CientSocket = 0;

#include <iostream>
using namespace std;

RC Socket::Start()
{
    WSADATA  Ws;
    
    struct sockaddr_in LocalAddr, ClientAddr;
    int Ret = 0;
    int AddrLen = 0;
    HANDLE hThread = NULL;

    //Init Windows Socket
    if ( WSAStartup(MAKEWORD(2,2), &Ws) != 0 )
    {
        //cout<<"Init Windows Socket Failed::"<<GetLastError()<<endl;
		return RETURN_FAILED;
    }
    
    //Create Socket
    ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if ( ServerSocket == INVALID_SOCKET )
    {
        //cout<<"Create Socket Failed::"<<GetLastError()<<endl;
        return RETURN_FAILED;
    }

    LocalAddr.sin_family = AF_INET;
    LocalAddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
    LocalAddr.sin_port = htons(PORT);
    memset(LocalAddr.sin_zero, 0x00, 8);

    //Bind Socket
    Ret = bind(ServerSocket, (struct sockaddr*)&LocalAddr, sizeof(LocalAddr));
    if ( Ret != 0 )
     {
         //cout<<"Bind Socket Failed::"<<GetLastError()<<endl;
         return RETURN_FAILED;
    }

    Ret = listen(ServerSocket, 10);
    if ( Ret != 0 )
    {
        //cout<<"listen Socket Failed::"<<GetLastError()<<endl;
        return RETURN_FAILED;
    }
	
	AddrLen = sizeof(ClientAddr);
    CientSocket = accept(ServerSocket, (struct sockaddr*)&ClientAddr, &AddrLen);
    if ( CientSocket == INVALID_SOCKET )
    {
        //cout<<"Accept Failed::"<<GetLastError()<<endl;
        return RETURN_FAILED;
    }
	
    cout<<"Client connect::"<<inet_ntoa(ClientAddr.sin_addr)<<":"<<ClientAddr.sin_port<<endl;
	return RETURN_SUCCEED;
}

RC Socket::Send(char *SendBuffer)
{
	int Ret = send(CientSocket, SendBuffer, (int)strlen(SendBuffer), 0);
	if ( Ret == SOCKET_ERROR )
    {
        //cout<<"Send Info Error::"<<GetLastError()<<endl;
		return RETURN_FAILED;
    }
	return RETURN_SUCCEED;
}

void Socket::Receive(char* RecvBuffer)
{
	int Ret = 0;
	//char *RecvBuffer = new char[MAX_PATH];
	memset(RecvBuffer, 0, sizeof(char) * MAX_PATH);
	Ret = recv(CientSocket, RecvBuffer, MAX_PATH, 0);
	cout << "recv: " << RecvBuffer << endl;
	if ( Ret == 0 || Ret == SOCKET_ERROR ) 
	{
		/*cout<<"客户端退出!"<<endl;
		break;*/
		return;
	}
	return;
}