#include "Gui.h"
#include <iostream>
//----------------------------------------------------------------------
using namespace std;
//----------------------------------------------------------------------

HANDLE Gui::GetOutputHandle()
{
	return GetStdHandle(STD_OUTPUT_HANDLE);
}
//----------------------------------------------------------------------

void Gui::ShowColorTable()
{
	// you can loop k higher to see more color choices
	for (int k = 0; k < 255; ++k)
	{
		// pick the colorattribute k you want
		SetConsoleTextAttribute(Gui::GetOutputHandle(), k);
		cout << k << " I want to be nice today!" << endl;
	}
}
//----------------------------------------------------------------------

void Gui::SetConsoleSize(int width, int height)
{
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);

	MoveWindow(console, r.left, r.top, 800, 600, TRUE); 
}
//----------------------------------------------------------------------

void Gui::SetConsoleTextColor(Color foreground, Color background)
{
	SetConsoleTextAttribute(Gui::GetOutputHandle(), foreground + (background * 16));
}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------

