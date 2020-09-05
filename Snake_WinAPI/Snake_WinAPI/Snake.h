#pragma once

#ifndef __SNAKE_H__
#define __SNAKE_H__

#include <Windows.h>

#define WINDOW_SIZE 800
#define CONST_SIZE 16

#define MAIN_CLASS_NAME "MainClass"
#define CAPTION "Snake"
#define YOU_WIN "You win!"
#define CONGRATULATIONS "Congratulations!"
#define PLAY_AGAIN "Play again?"
#define ZERO '0'
#define SPACE ' '

#define Append(destination, source) wcscat_s (destination, BUFFER_SIZE, source);

typedef	unsigned char boolean;
typedef unsigned char byte;

#define true 1
#define false 0

typedef struct _Point
{
	byte x, y;
} Point;

typedef enum _Direction
{
	up,
	right,
	down,
	left
} Direction;

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
LRESULT WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void Init();
void Step();
void Draw();
void End();
void RandomPoint(Point *p);
void GetStringFromInt(int value, TCHAR *result);
boolean IsInSnake(Point p);

#endif // __SNAKE_H__