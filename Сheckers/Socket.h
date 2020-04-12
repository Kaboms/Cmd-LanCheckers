#pragma once
//----------------------------------------------------------------------
#include <winsock2.h>
#include <ws2tcpip.h>
#include <winsock.h>
//----------------------------------------------------------------------
#define SOCKET_CLIENT 0
#define SOCKET_SERVER 1

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"
//----------------------------------------------------------------------

class Socket
{
public:
	Socket();
	
	void InitServer(PCSTR serverIP, PCSTR port = DEFAULT_PORT);
	void InitClient(PCSTR serverIP, PCSTR port = DEFAULT_PORT);
	int Send(char* buffer, int length = DEFAULT_BUFLEN);
	int Receive(char* buffer, int length = DEFAULT_BUFLEN);
	void Close();
	int GetState();

	enum State
	{
		Error = -1,
		Disconnected = 0,
		Connected = 2,
		NotInit = 3,
		Wait = 4
	};

private:
	int state;

	SOCKET _socket;
	struct addrinfo* result;
	struct addrinfo hints;

	void Init();
};
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
