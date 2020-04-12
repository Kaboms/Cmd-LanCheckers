#include "Client.h"
#include "Socket.h"
#include "Board.h"
#include "Key.h"
//#include <windows.h>
//#include <winsock2.h>
//#include <ws2tcpip.h>
//#include <stdlib.h>
//#include <stdio.h>
//#include <conio.h>
#include <iostream>
//----------------------------------------------------------------------
using namespace std;
//----------------------------------------------------------------------
// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
//#pragma comment (lib, "Ws2_32.lib")
//#pragma comment (lib, "Mswsock.lib")
//#pragma comment (lib, "AdvApi32.lib")
//----------------------------------------------------------------------

//#define DEFAULT_BUFLEN 512
//#define DEFAULT_PORT "27015"
//
//#define WIN32_LEAN_AND_MEAN
//#define _WINSOCK_DEPRECATED_NO_WARNINGS
//----------------------------------------------------------------------

void Client::ConnectToServer(string serverIP, string playerName)
{
    Socket* client_socket = new Socket();

    char sendbuf[DEFAULT_BUFLEN];

    fill_n(sendbuf, DEFAULT_BUFLEN, '\0');

    sendbuf[0] = playerName.length();

    for (size_t i = 0; i < playerName.length(); ++i)
    {
        sendbuf[i + 1] = playerName[i];
    }

    char recvbuf[DEFAULT_BUFLEN];

    client_socket->InitClient(serverIP.c_str());

    if (client_socket->GetState() == Socket::State::Connected)
        cout << "Connected.\n";

    if (client_socket->Send(sendbuf, (int)strlen(sendbuf)) == Socket::Error)
    {
        printf("send failed with error: %d\n", WSAGetLastError());
        return;
    }

    
    // Receive until the peer closes the connection
    do 
    {
        if (client_socket->Receive(recvbuf) > 0)
        {
            if (client_socket->GetState() == Socket::Connected)
            {
                string player_name = "";

                for (int i = 0; i < recvbuf[1]; i++)
                {
                    player_name += recvbuf[i + 2];
                }
                cout << "You connect to the " << player_name;

                if (client_socket->Send(sendbuf, (int)strlen(sendbuf)) == SOCKET_ERROR)
                {
                    cout << "send failed with error: %d\n" << WSAGetLastError();
                    return;
                }
            }
            /*else if (client_socket->GetState() == Socket::Connected)
            {
                system("cls");

                Board* board = new Board();
                board->SetPlayer(RED_PLAYER);
                board->Draw();

                while (client_socket->GetState() == Socket::Connected)
                {
                    if (board->Controle() == 0)
                    {
                        return;
                    }
                    else if (board->Controle() == 2)
                    {
                        if (client_socket->Send(sendbuf, (int)strlen(sendbuf)) == SOCKET_ERROR)
                        {
                            cout << "send failed with error: %d\n" << WSAGetLastError();
                            return;
                        }
                    }
                }
            }*/
        }
        else if (client_socket->GetState() == Socket::Disconnected)
        {
            printf("Connection closed\n");
        }
        else
        {
            printf("recv failed with error: %d\n", WSAGetLastError());
        }

    } while (client_socket->GetState() > 0);
    

    client_socket->Close();

    Key::GetKey();

    return;

}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
