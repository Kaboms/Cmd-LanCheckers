#pragma once
//----------------------------------------------------------------------
#include <vector>
#include <Windows.h>
//----------------------------------------------------------------------
#define YELLOW_PLAYER 1
#define RED_PLAYER 2
//----------------------------------------------------------------------
const int BoardSize = 8;
//----------------------------------------------------------------------

struct Point
{
public:

	int x, y;

	Point()
	{
		x = 0;
		y = 0;
	}
	//----------------------------------------------------------------------

	Point(int new_x, int new_y)
	{
		x = new_x;
		y = new_y;
	}
	//----------------------------------------------------------------------

	bool operator == (const Point& other_point)
	{
		if (x == other_point.x && y == other_point.y)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	//----------------------------------------------------------------------

	bool operator != (const Point& other_point)
	{
		if (x == other_point.x && y == other_point.y)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	//----------------------------------------------------------------------

	COORD ToConsoleCoord()
	{
		COORD cursor_pos;
		cursor_pos.X = x + 1 + (2 * x);
		cursor_pos.Y = y + 1;

		return cursor_pos;
	}
	//----------------------------------------------------------------------

};
//----------------------------------------------------------------------

struct Cursor
{
	enum class Type
	{
		Default = 0,
		OnTarget = 1,
		OnWalk = 2
	};

	Point pos;
	Point checker;
	std::vector<Point> steps;
	Type type;

	Cursor()
	{
		pos.x = 0;
		pos.y = 0;

		checker.x = 0;
		checker.y = 0;
		type = Type::Default;
	}
	//----------------------------------------------------------------------

	Cursor(int new_x, int new_y, bool on_target = false)
	{
		pos.x = new_x;
		pos.y = new_y;

		checker.x = 0;
		checker.y = 0;
		type = Type::Default;
	}
	//----------------------------------------------------------------------

	bool OnStep()
	{
		if (steps.empty() == false)
		{
			auto it = std::find(steps.begin(), steps.end(), pos);

			if (it != steps.end())
			{
				return true;
			}
		}
		return false;
	}
	//----------------------------------------------------------------------
};
//----------------------------------------------------------------------

enum Direction
{
	Left = -1,
	Right = 1
};

class Board
{
public:
	Board();
	void Draw();
	void SetPlayer(int player);
	int Controle();

private:
	void FillBoard();
	void DrawRow(int y);
	void DrawCell(Point pos);
	void SetCursor();
	void RevertCell();
	void CheckSteps();
	void DrawStep(Point cursor_pos);

	int map[BoardSize][BoardSize];
	int player;
	Cursor cursor;
};
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
