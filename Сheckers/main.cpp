#include <string.h>
#include <conio.h>
#include <stdio.h>
#include <iostream>
#include <locale>
#include "Board.h"
#include "Menu.h"
#include "Gui.h"
//----------------------------------------------------------------------
using namespace std;
//----------------------------------------------------------------------

int main(void)
{
	// Initialize Winsock
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		cout << "WSAStartup failed with error.\n";
	}

	//Gui::SetConsoleSize(800, 600);
	//Gui::ShowColorTable();
	//Menu::ShowMain();
	
	Board* board = new Board();
	board->SetPlayer(YELLOW_PLAYER);
	board->Draw();
	
	while (true)
	{
		if (board->Controle() == 0)
		{
			return 0;
		}
	}

	WSACleanup();
}
//----------------------------------------------------------------------
