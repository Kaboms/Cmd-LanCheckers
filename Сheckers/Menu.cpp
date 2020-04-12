#include "Menu.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include "Gui.h"
#include "Key.h"
#include "Server.h"
#include "Client.h"
//----------------------------------------------------------------------
using namespace std;
using namespace Gui;
//----------------------------------------------------------------------
#pragma comment(lib, "ws2_32.lib")
//----------------------------------------------------------------------
#define DEFAULT 15
#define SELECTED 240
//----------------------------------------------------------------------

void Menu::ShowMain()
{
	string items[3] = { "Create game", "Connect to player", "Exit" };

	auto show_main = [&](int selected)
	{
		SetConsoleTextColor(Color::White, Color::Black);
		system("cls");

		cout << items[0] << endl;
		cout << items[1] << endl;
		cout << items[2];

		SetConsoleTextColor(Color::Black, Color::White);

		COORD pos;
		pos.X = 0;
		pos.Y = selected;
		SetConsoleCursorPosition(Gui::GetOutputHandle(), pos);

		cout << items[selected];
	};

	int selected = 0;

	while (true)
	{
		show_main(selected);

		int key = Key::GetKey();

		if (key == Key::DownArrow && selected < 2)
		{
			selected++;
		}
		else if (key == Key::UpArrow && selected > 0)
		{
			selected--;
		}
		else if (key == Key::Space || key == Key::Enter)
		{
			switch (selected)
			{
			case 0: ShowStartGameMenu(); break;
			case 1: ShowConnectGameMenu();  break;
			case 2: return;
			}
		}
	}
}
//----------------------------------------------------------------------

void Menu::ShowStartGameMenu()
{
	SetConsoleTextColor(Color::White, Color::Black);

	system("cls");

	cout << "You can play with players only LOCAL NETWORK.\n";

	string name = "";

	cout << "Enter your name: ";
	cin >> name;

	WSADATA WSAData;

	// Initialize winsock dll

	if (WSAStartup(MAKEWORD(1, 0), &WSAData))
	{
		cout << "Socket error";
		return;
	}

	char szHostName[128] = "";

	//get the standard host name of the machine
	if (gethostname(szHostName, sizeof(szHostName)))
	{
		cout << "Standart host not found";
		return;
	}

	struct sockaddr_in SocketAddress;
	struct hostent* pHost = 0;

	// Get local IP addresses
	pHost = gethostbyname(szHostName);

	if (!pHost)
	{
		cout << "Failed to get IP addresses";
		return;
	}

	char aszIPAddresses[10][16];

	for (int i = 0; i < 10; ++i)
	{
		strcpy_s(aszIPAddresses[i], "");
	}

	int nCount;

	for (nCount = 0; ((pHost->h_addr_list[nCount]) && (nCount < 10)); ++nCount)
	{
		memcpy(&SocketAddress.sin_addr, pHost->h_addr_list[nCount], pHost->h_length);
		strcpy_s(aszIPAddresses[nCount], inet_ntoa(SocketAddress.sin_addr));
	}

	WSACleanup();
	
	int selected = 0;

	if (nCount >= 1)
	{
		cout << "\nYour local addresses: \n";
		for (int i = 0; i < nCount; ++i)
		{
			string address = aszIPAddresses[i];

			if (address.empty())
			{
				break;
			}

			cout << i + 1 << ". " << address << endl;
		}

		bool waitServer = true;
		int key = -1;

		do
		{
			cout << "Enter the number of the address where you would like to open the server (-1 for exit): ";

			int key;

			cin >> key;

			if (key <= nCount && key > 0)
			{
				selected = key - 1;
				waitServer = false;
			}
			else if (key == -1)
			{
				return;
			}
			else
			{
				cout << "\nERROR. You enter incorrect number!\n";
			}

		} while (waitServer);

	}
	else
	{
		cout << "\nYour local address: " << aszIPAddresses[selected] << endl;
	}

	cout << "Would you like create server " << aszIPAddresses[selected] << "? (press ENTER to continue)\n";

	if (Key::GetKey() != Key::Enter)
		return;

	Server::CreateGame(aszIPAddresses[selected], name);

	Key::GetKey();
}
//----------------------------------------------------------------------

void Menu::ShowConnectGameMenu()
{
	SetConsoleTextColor(Color::White, Color::Black);

	system("cls");

	cout << "You can play with players only LOCAL NETWORK. Press ENTER to continue.\n";

	if (Key::GetKey() != Key::Enter)
		return;

	string name = "";

	cout << "Enter your name: ";
	cin >> name;

	string server = "";

	cout << "Enter server ip: ";
	cin >> server;

	Client::ConnectToServer(server, name);
}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
