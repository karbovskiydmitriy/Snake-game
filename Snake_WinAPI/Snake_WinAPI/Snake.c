#include "Snake.h"

const DWORD windowStyle = WS_POPUP;
const int width = CONST_SIZE, height = CONST_SIZE, step = WINDOW_SIZE / CONST_SIZE;

int prevTime, length, score;
Point *snake;
Point apple;
Direction snakeDir = right;
RECT clientRect;
boolean inGame, madeMove;
HDC hdcBack;
HBRUSH appleBrush;

WNDCLASSEX WndClassEx =
{
	sizeof(WNDCLASSEX), 0, (WNDPROC)WindowProc, 0, 0, 0, 0, 0, 0, 0, MAIN_CLASS_NAME, 0
};

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;

	WndClassEx.hInstance = hInstance;
	WndClassEx.hCursor = LoadCursor(0, IDC_ARROW);
	RegisterClassEx(&WndClassEx);
	CreateWindowEx(0, MAIN_CLASS_NAME, CAPTION, windowStyle, 0, 0, WINDOW_SIZE, WINDOW_SIZE, 0, 0, hInstance, 0);

	while (GetMessage(&msg, 0, 0, 0))
	{
		DispatchMessage(&msg);
	}

	ExitProcess(0);

	return 0;
}

LRESULT WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	switch (uMsg)
	{
	case WM_CREATE:
		SetTimer(hWnd, 0, 125, 0);
		Init();
		ShowWindow(hWnd, SW_SHOWNORMAL);
		UpdateWindow(hWnd);
		GetClientRect(hWnd, &clientRect);
		DeleteDC(hdcBack);
		hdc = GetDC(0);
		hdcBack = CreateCompatibleDC(hdc);
		SelectObject(hdcBack, CreateCompatibleBitmap(hdc, WINDOW_SIZE, WINDOW_SIZE));
		appleBrush = CreateSolidBrush(RGB(0, 200, 40));
		SelectObject(hdcBack, GetStockObject(WHITE_BRUSH));
		ReleaseDC(0, hdc);
		InvalidateRect(hWnd, 0, 0);

		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);

		return 0;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			PostQuitMessage(0);

			return 0;
		}
		if (!madeMove)
		{
			switch (wParam)
			{
			case VK_UP:
				if (snakeDir != down && inGame)
				{
					snakeDir = up;
					madeMove = true;
					inGame = true;
				}

				return 0;
			case VK_DOWN:
				if (snakeDir != up)
				{
					snakeDir = down;
					madeMove = true;
					inGame = true;
				}

				return 0;
			case VK_RIGHT:
				if (snakeDir != left)
				{
					snakeDir = right;
					madeMove = true;
					inGame = true;
				}

				return 0;
			case VK_LEFT:
				if (snakeDir != right && inGame)
				{
					snakeDir = left;
					madeMove = true;
					inGame = true;
				}

				return 0;
			}
		}

		return 0;
	case WM_TIMER:
		if (inGame)
		{
			Step();
			InvalidateRect(hWnd, 0, 0);
		}

		return 0;
	case WM_PAINT:
		Draw();
		hdc = BeginPaint(hWnd, &ps);
		BitBlt(hdc, 0, 0, WINDOW_SIZE, WINDOW_SIZE, hdcBack, 0, 0, SRCCOPY);
		EndPaint(hWnd, &ps);

		return 0;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}

void Init()
{
		if (snake)
		{
			free(snake);
		}
		length = 2;
		score = 0;
		snakeDir = right;
		snake = (Point *)calloc(length, sizeof(Point));
		snake[0].x = 1;
		snake[0].y = 0;
		snake[1].x = 0;
		snake[1].y = 0;
		RandomPoint(&apple);
		prevTime = 0;
		madeMove = false;
		inGame = false;
}

void Step()
{
	int i;
	Point temp;
	Point *oldSnake;

	madeMove = false;
	temp = snake[0];
	switch (snakeDir)
	{
	case up:
		temp.y--;
		break;
	case right:
		temp.x++;
		break;
	case down:
		temp.y++;
		break;
	case left:
		temp.x--;
		break;
	}
	if (temp.x == apple.x && temp.y == apple.y)
	{
		score += 10;
		length++;
		oldSnake = snake;
		snake = (Point *)calloc(length, sizeof(Point));
		if (oldSnake)
		{
			for (i = 1; i < length; i++)
			{
				snake[i] = oldSnake[i - 1];
			}
		}
		snake[0] = apple;
		RandomPoint(&apple);
		free(oldSnake);
	}
	else
	{
		for (i = length - 1; i > 0; i--)
		{
			snake[i] = snake[i - 1];
		}
		snake[0] = temp;
	}
	if (IsInSnake(temp) || temp.x < 0 || temp.y < 0 || temp.x >= width || temp.y >= height)
	{
		End();
	}

}

void Draw()
{
	int i;
	RECT r;
	
	FillRect(hdcBack, &clientRect, 0);
	for (i = 1; i < height; i++)
	{
		MoveToEx(hdcBack, i * step, 0, 0);
		LineTo(hdcBack, i * step, WINDOW_SIZE);
	}
	for (i = 1; i < width; i++)
	{
		MoveToEx(hdcBack, 0, i * step, 0);
		LineTo(hdcBack, WINDOW_SIZE, i * step);
	}
	r.left = apple.x * step + 5;
	r.right = r.left + step - 10;
	r.top = apple.y * step + 5;
	r.bottom = r.top + step - 10;
	SelectObject(hdcBack, appleBrush);
	Ellipse(hdcBack, r.left, r.top, r.right, r.bottom);
	SelectObject(hdcBack, GetStockObject(WHITE_BRUSH));
	for (i = 0; i < length; i++)
	{
		r.left = snake[i].x * step + 3;
		r.right = r.left + step - 5;
		r.top = snake[i].y * step + 3;
		r.bottom = r.top + step - 5;
		FillRect(hdcBack, &r, (HBRUSH)10);
	}
}

void End()
{
	TCHAR *scoreText;

	inGame = false;
	if (length == 256)
	{
		MessageBox((HWND)0, YOU_WIN, CONGRATULATIONS, MB_OK);
	}
	scoreText = (TCHAR *)calloc(5, sizeof(TCHAR));
	GetStringFromInt(score, scoreText);
	if (MessageBox((HWND)0, PLAY_AGAIN, scoreText, MB_YESNO) != IDYES)
	{
		PostQuitMessage(0);
	}
	Init();
}

void RandomPoint(Point *p)
{
	byte time;

	do
	{
		time = ((byte)(GetTickCount() + rand())) % (width * height);
		p->x = time / width;
		p->y = time % height;
	} while (IsInSnake(*p));
}

void GetStringFromInt(int value, TCHAR *result)
{
	int index;

	for (index = 0; index < 4; index++)
	{
		result[index] = ZERO;
	}
	index = 3;
	do
	{
		result[index--] = ZERO + value % 10;
		value /= 10;
	} while (value);
	while (index >= 0)
	{
		result[index--] = SPACE;
	}
}

boolean IsInSnake(Point p)
{
	int i;

	for (i = 1; i < length; i++)
	{
		if (snake[i].x == p.x && snake[i].y == p.y)
		{
			return true;
		}
	}

	return false;
}