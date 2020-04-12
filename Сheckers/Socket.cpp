#include "Socket.h"
#include <stdio.h>
#include <iostream>
//----------------------------------------------------------------------
using namespace std;
//----------------------------------------------------------------------
// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
//----------------------------------------------------------------------
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
//----------------------------------------------------------------------

Socket::Socket()
{
    state = State::NotInit;

    _socket = INVALID_SOCKET;
	result = NULL;
	ZeroMemory(&hints, sizeof(hints));
}
//----------------------------------------------------------------------

void Socket::Init()
{
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
}
//----------------------------------------------------------------------

void Socket::InitServer(PCSTR serverIP, PCSTR port/* = 27015*/)
{
	Socket::Init();

    SOCKET ListenSocket = INVALID_SOCKET;

	hints.ai_flags = AI_PASSIVE;

    int iResult;

    // Resolve the server address and port
    iResult = getaddrinfo(serverIP, DEFAULT_PORT, &hints, &result);
    if (iResult != 0)
    {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return;
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET)
    {
        printf("Listen socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return;
    }

    // Setup the TCP listening socket
    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR)
    {
        printf("bind listen socket failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR)
    {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return;
    }

    state = State::Wait;

    cout << "Wait client...\n";

    // Accept a client socket
    _socket = accept(ListenSocket, NULL, NULL);
    if (_socket == INVALID_SOCKET)
    {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return;
    }

    // No longer need server socket
    closesocket(ListenSocket);
}
//----------------------------------------------------------------------

void Socket::InitClient(PCSTR serverIP, PCSTR port/* = 27015*/)
{
    int iResult;

    // Resolve the server address and port
    // Resolve the server address and port
    iResult = getaddrinfo(serverIP, DEFAULT_PORT, &hints, &result);
    if (iResult != 0)
    {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return;
    }

    // Attempt to connect to an address until one succeeds
    struct addrinfo* ptr = NULL;

    for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
    {
        // Create a SOCKET for connecting to server
        _socket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);

        if (_socket == INVALID_SOCKET)
        {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            state = State::Error;
            WSACleanup();
            return;
        }

        // Connect to server.
        iResult = connect(_socket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR)
        {
            closesocket(_socket);
            _socket = INVALID_SOCKET;
            state = State::Error;
            continue;
        }
        break;
    }

    state = State::Connected;

    freeaddrinfo(result);

    if (_socket == INVALID_SOCKET)
    {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return;
    }
}
//----------------------------------------------------------------------

int Socket::Send(char* buffer, int length /* = 512 */)
{
    int _state = send(_socket, buffer, length, 0);

    if (_state == State::Error)
    {
        state = State::Error;
        closesocket(_socket);
    }
    else if (_state == State::Disconnected)
    {
        state = State::Disconnected;
        closesocket(_socket);
    }

    return state;
}
//----------------------------------------------------------------------

int Socket::Receive(char* data, int length /* = 512 */)
{
    int _state = recv(_socket, data, length, 0);
    
    if (_state == State::Error)
    {
        state = State::Error;
        closesocket(_socket);
    }
    else if (_state == State::Disconnected)
    {
        state = State::Disconnected;
        closesocket(_socket);
    }

    return _state;
}
//----------------------------------------------------------------------

void Socket::Close()
{
    // shutdown the connection since we're done
    if (shutdown(_socket, SD_SEND) == SOCKET_ERROR)
    {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(_socket);
        WSACleanup();
        return;
    }

    // cleanup
    closesocket(_socket);
}
//----------------------------------------------------------------------

int Socket::GetState()
{
    return state;
}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
