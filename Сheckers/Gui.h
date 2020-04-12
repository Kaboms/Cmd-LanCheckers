#pragma once
//----------------------------------------------------------------------
#include<Windows.h>
//----------------------------------------------------------------------
namespace Gui
{
	enum Color
	{
		Black = 0,
		Blue = 1,
		Green = 2,
		Aqua = 3,
		Red = 4,
		Purple = 5,
		Yellow = 6,
		White = 7,
		Gray = 8,
		LightBlue = 9,
		LightGreen = 10,
		LightAqua = 11,
		LightRed = 12,
		LightPurple = 13,
		LightYellow = 14,
		BrightWhite = 14
	};
	//----------------------------------------------------------------------

	HANDLE GetOutputHandle();
	void ShowColorTable();
	void SetConsoleSize(int width, int height);
	void SetConsoleTextColor(Color foreground, Color background);
	//----------------------------------------------------------------------
	//----------------------------------------------------------------------
	//----------------------------------------------------------------------
}
