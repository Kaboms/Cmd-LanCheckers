#include "Server.h"
#include "Key.h"
#include "Socket.h"
#include <iostream>
#include "Board.h"
//----------------------------------------------------------------------
using namespace std;
//----------------------------------------------------------------------
// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
//#pragma comment (lib, "Ws2_32.lib")
//#pragma comment (lib, "Mswsock.lib")
//#pragma comment (lib, "AdvApi32.lib")
//----------------------------------------------------------------------
//#define WIN32_LEAN_AND_MEAN
//#define _WINSOCK_DEPRECATED_NO_WARNINGS
//----------------------------------------------------------------------

void Server::CreateGame(string serverIP, string playerName)
{
    Socket* socket = new Socket();

    socket->InitServer(serverIP.c_str());

    char buffer[DEFAULT_BUFLEN];
    
    // Receive until the peer shuts down the connection
    do 
    {
        //iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);

        if (socket->Receive(buffer) > 0)
        {
            if (socket->GetState() == Socket::Wait)
            {
                string player_name = "";

                for (int i = 0; i < buffer[1]; i++)
                {
                    player_name += buffer[i + 2];
                }

                cout << "Player " << player_name << " connected.";

                socket->Send(buffer);
            }
            else if (socket->GetState() == Socket::Connected)
            {
                system("cls");

                Board* board = new Board();
                board->SetPlayer(YELLOW_PLAYER);
                board->Draw();

                while (socket->GetState() == Socket::Connected)
                {
                    if (board->Controle() != 1)
                    {
                        return;
                    }
                    socket->Send(buffer);
                }
            }
            else
            {
                socket->Send(buffer);
            }
        }
        else if (socket->GetState() == Socket::Disconnected)
        {
            printf("Connection closing...\n");
        }
        else if(socket->GetState() == Socket::Error)
        {
            socket->Close();
            return;
        }

    } while (socket->GetState() > 0);
    

    socket->Close();

    /*
    // shutdown the connection since we're done
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR)
    {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        return;
    }

    // cleanup
    closesocket(ClientSocket);
    */
}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------