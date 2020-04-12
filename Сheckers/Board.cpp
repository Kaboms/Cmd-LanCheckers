#include "Board.h"
#include "Gui.h"
#include "Key.h"
#include <conio.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <locale>
//----------------------------------------------------------------------
using namespace std;
using namespace Gui;
//----------------------------------------------------------------------

Board::Board()
{
	player = NULL;

	FillBoard();
}
//----------------------------------------------------------------------

void Board::FillBoard()
{
	//Set all cells to NULL
	for (int i = 0; i < BoardSize; ++i)
		fill_n(map[i], BoardSize, 0);

	for (int i = 0; i < BoardSize; ++i)
	{
		map[0][i] = (i % 2 != 0) ? RED_PLAYER : NULL;
		map[1][i] = (i % 2 == 0) ? RED_PLAYER : NULL;
		map[2][i] = (i % 2 != 0) ? RED_PLAYER : NULL;

		map[5][i] = (i % 2 == 0) ? YELLOW_PLAYER : NULL;
		map[6][i] = (i % 2 != 0) ? YELLOW_PLAYER : NULL;
		map[7][i] = (i % 2 == 0) ? YELLOW_PLAYER : NULL;
	}
}
//----------------------------------------------------------------------

void Board::DrawCell(Point pos)
{
	//Draw cell without cursor
	COORD cursor_pos = pos.ToConsoleCoord();

	SetConsoleCursorPosition(Gui::GetOutputHandle(), cursor_pos);

	if (map[pos.y][pos.x] == NULL)
	{
		if (pos.y % 2 != 0)
		{
			if (pos.x % 2 == 0)
			{
				SetConsoleTextColor(Color::White, Color::Black);
			}
			else
			{
				SetConsoleTextColor(Color::Black, Color::White);
			}
			cout << "   ";
		}
		else
		{
			if (pos.x % 2 == 0)
			{
				SetConsoleTextColor(Color::Black, Color::White);
			}
			else
			{
				SetConsoleTextColor(Color::White, Color::Black);
			}
			cout << "   ";
		}
	}
	else if (map[pos.y][pos.x] == RED_PLAYER)
	{
		SetConsoleTextColor(Color::Red, Color::Black);
		cout << " O ";
	}
	else if (map[pos.y][pos.x] == YELLOW_PLAYER)
	{
		SetConsoleTextColor(Color::LightYellow, Color::Black);
		cout << " O ";
	}
}
//----------------------------------------------------------------------

void Board::DrawRow(int y)
{
	SetConsoleTextColor(Color::Black, Color::Yellow);
	cout << "|";

	if (player == YELLOW_PLAYER)
		for (int x = 0; x < BoardSize; ++x)
			DrawCell(Point(x, y));
	else
		for (int x = BoardSize - 1; x >= 0; --x)
			DrawCell(Point(x, y));

	SetConsoleTextColor(Color::Black, Color::Yellow);

	cout << "|\n";
}
//----------------------------------------------------------------------

void Board::Draw()
{
	COORD cursor;
	cursor.X = 0;
	cursor.Y = 0;
	SetConsoleCursorPosition(Gui::GetOutputHandle(), cursor);

	SetConsoleTextColor(Color::Black, Color::Yellow);
	cout << "--------------------------\n";

	if(player == YELLOW_PLAYER)
		for (int y = 0; y < BoardSize; ++y)
			DrawRow(y);
	else
		for (int y = BoardSize - 1; y >= 0; --y)
			DrawRow(y);

	SetConsoleTextColor(Color::Black, Color::Yellow);
	cout << "--------------------------\n";

	SetConsoleTextColor(Color::White, Color::Black);
}
//----------------------------------------------------------------------

void Board::DrawStep(Point cursor_pos)
{
	COORD coord = cursor_pos.ToConsoleCoord();
	SetConsoleCursorPosition(Gui::GetOutputHandle(), coord);
	SetConsoleTextColor(Color::Black, Color::LightRed);

	cout << "   ";
}
//----------------------------------------------------------------------

void Board::CheckSteps()
{
	auto add_step = [&](Point cursor_pos, int length, Direction direction)
	{
		cursor_pos.x += length * direction;
		cursor_pos.y -= length;

		if (map[cursor_pos.y][cursor_pos.x] == NULL)
		{
			DrawStep(cursor_pos);
			cursor.steps.push_back(cursor_pos);
		}
	};

	Point cursor_pos = cursor.pos;

	if (player == YELLOW_PLAYER && map[cursor.pos.y][cursor.pos.x] == YELLOW_PLAYER )
	{
		cursor.checker = cursor.pos;

		if (cursor.pos.y - 1 > 0)
		{
			if (cursor.pos.x - 1 >= 0)
			{
				add_step(cursor.pos, 1, Direction::Left);
			}
			if (cursor.pos.x + 1 < BoardSize)
			{
				add_step(cursor.pos, 1, Direction::Right);
			}
		}
	}

	if(cursor.steps.empty() == false)
	{
		cursor.type = Cursor::Type::OnTarget;
	}
	else
	{
		cursor.type = Cursor::Type::Default;
	}
}
//----------------------------------------------------------------------

void Board::SetCursor()
{
	//Draw cell on cursor
	COORD cursor_pos = cursor.pos.ToConsoleCoord();

	cursor_pos.X = cursor.pos.x + 1 + (2 * cursor.pos.x);
	cursor_pos.Y = cursor.pos.y + 1;

	SetConsoleCursorPosition(Gui::GetOutputHandle(), cursor_pos);

	//Draw cursor lambda
	auto draw_cursor = [&](int x, int y)
	{
		if (map[y][x] == NULL)
		{
			SetConsoleTextColor(Color::Black, Color::Gray);
			cout << "   ";
		}
		else if (map[y][x] == RED_PLAYER)
		{
			SetConsoleTextColor(Color::Red, Color::Gray);
			cout << " O ";
		}
		else if (map[y][x] == YELLOW_PLAYER)
		{
			SetConsoleTextColor(Color::Yellow, Color::Gray);
			cout << " O ";
		}
	};

	if (player == YELLOW_PLAYER)
		draw_cursor(cursor.pos.x, cursor.pos.y);
	else
		draw_cursor((BoardSize - 1) - cursor.pos.x, (BoardSize - 1) - cursor.pos.y);

	if (cursor.type != Cursor::Type::OnWalk)
		CheckSteps();

	//Set cursor on cell`s center
	cursor_pos = cursor.pos.ToConsoleCoord();
	cursor_pos.X++;
	SetConsoleCursorPosition(Gui::GetOutputHandle(), cursor_pos);
}
//----------------------------------------------------------------------

void Board::RevertCell()
{
	if(cursor.OnStep())
	{
		DrawStep(cursor.pos);
		return;
	}

	if (player == YELLOW_PLAYER)
		DrawCell(cursor.pos);
	else
		DrawCell(Point((BoardSize - 1) - cursor.pos.x, (BoardSize - 1) - cursor.pos.y));

	//Reset steps
	if (cursor.type == Cursor::Type::OnTarget)
	{
		if (cursor.pos.y - 1 > 0)
		{
			Point cursor_pos;

			if (cursor.pos.x - 1 >= 0)
			{
				cursor_pos.x = cursor.pos.x - 1;
				cursor_pos.y = cursor.pos.y - 1;
				SetConsoleCursorPosition(Gui::GetOutputHandle(), cursor_pos.ToConsoleCoord());

				DrawCell(Point(cursor.pos.x - 1, cursor.pos.y - 1));
			}
			if (cursor.pos.x + 1 < BoardSize)
			{
				cursor_pos.x = cursor.pos.x + 1;
				cursor_pos.y = cursor.pos.y - 1;
				SetConsoleCursorPosition(Gui::GetOutputHandle(), cursor_pos.ToConsoleCoord());

				DrawCell(Point(cursor.pos.x + 1, cursor.pos.y - 1));
			}
		}
	}
}
//----------------------------------------------------------------------

void Board::SetPlayer(int player)
{
	this->player = player;
}
//----------------------------------------------------------------------

int Board::Controle()
{
	auto move = [&](int key)
	{
		if (key == Key::DownArrow && cursor.pos.y < BoardSize - 1)
		{
			cursor.pos.y++;
		}
		else if (key == Key::UpArrow && cursor.pos.y > 0)
		{
			cursor.pos.y--;
		}
		else if (key == Key::RightArrow && cursor.pos.x < BoardSize - 1)
		{
			cursor.pos.x++;
		}
		else if (key == Key::LeftArrow && cursor.pos.x > 0)
		{
			cursor.pos.x--;
		}
	};

	int key = Key::GetKey();

	if (key == Key::Esc)
	{
		//Завершение программы
		return 0;
	}

	if (key == Key::Space)
	{
		if (cursor.type == Cursor::Type::OnTarget)
		{
			cursor.type = Cursor::Type::OnWalk;
		}
		else if (cursor.type == Cursor::Type::OnWalk)
		{
			if (cursor.OnStep())
			{
				map[cursor.pos.y][cursor.pos.x] = map[cursor.checker.y][cursor.checker.x];
				map[cursor.checker.y][cursor.checker.x] = NULL;

				DrawCell(cursor.checker);
			}

			// Clear targets
			cursor.type = Cursor::Type::Default;

			for (auto it = cursor.steps.begin(); it != cursor.steps.end(); ++it)
			{
				DrawCell((*it));
			}
			cursor.steps.clear();

			//Set cursor on cell`s center
			COORD cursor_pos = cursor.pos.ToConsoleCoord();
			cursor_pos.X++;
			SetConsoleCursorPosition(Gui::GetOutputHandle(), cursor_pos);

		}
		return 1;
	}

	if (cursor.type == Cursor::Type::OnWalk)
	{
		RevertCell();

		move(key);

		SetCursor();
	}

	if (cursor.type == Cursor::Type::Default || cursor.type == Cursor::Type::OnTarget)
	{
		cursor.steps.clear();

		RevertCell();

		move(key);

		SetCursor();
	}

	return 2;
}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------