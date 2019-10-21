#include "Snake.h"

const DWORD windowStyle = WS_POPUP;
const int width = constSize, height = constSize, step = windowSize / constSize;
int prevTime, length, score;
point *snake;
point apple;
direction snakeDir = right;
boolean inGame;
RECT clientRect = {0, 0, windowSize, windowSize};
HDC hdcBack;
HBRUSH appleBrush;
const wchar_t *caption = L"Snake";
#define mainClassName L"MainClass"

WNDCLASSEX WndClassEx = {sizeof(WNDCLASSEX), 0, (WNDPROC)WindowProc, 0, 0, 0, 0, 0, 0, 0, mainClassName, 0};

INT WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	
	MSG msg;
	RECT R;

	R = clientRect;
	AdjustWindowRectEx (&R, WS_OVERLAPPED, 0, 0);
	WndClassEx.hInstance = hInstance;
	WndClassEx.hCursor = LoadCursor (0, IDC_ARROW);
	RegisterClassEx (&WndClassEx);
	CreateWindowExW (0, mainClassName, (LPCWSTR)caption, windowStyle, 0, 0, R.right, R.bottom, 0, 0, hInstance, 0);

	while (GetMessage (&msg, 0, 0, 0)) {
		TranslateMessage (&msg);
		DispatchMessage (&msg);
	}
	ExitProcess(0);

}

LRESULT WindowProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	HDC hdc;
	PAINTSTRUCT ps;

	switch (uMsg) {
	case WM_CREATE:
		SetTimer (hWnd, 0, 125, 0);
		Init();
		ShowWindow (hWnd, SW_SHOWNORMAL);
		UpdateWindow (hWnd);
		GetClientRect (hWnd, &clientRect);
		DeleteDC (hdcBack);
		hdc = GetDC (0);
		hdcBack = CreateCompatibleDC (hdc);
		SelectObject (hdcBack, CreateCompatibleBitmap (hdc, windowSize, windowSize));
		appleBrush = CreateSolidBrush (RGB(0, 200, 40));
		SelectObject (hdcBack, GetStockObject (WHITE_BRUSH));
		ReleaseDC (0, hdc);
		InvalidateRect (hWnd, 0, 0);
	return 0;
	case WM_DESTROY:
		ExitProcess (0);
	case WM_KEYDOWN:
		inGame = true;
		if (wParam == VK_ESCAPE) {
			PostQuitMessage (0);
			return 0;
		}
		switch (wParam) {
		case VK_UP:
			if (snakeDir != down) {
				snakeDir = up;
			}
			return 0;
		case VK_DOWN:
			if (snakeDir != up) {
				snakeDir = down;
			}
			return 0;
		case VK_RIGHT:
			if (snakeDir != left) {
				snakeDir = right;
			}
			return 0;
		case VK_LEFT:
			if (snakeDir != right) {
				snakeDir = left;
			}
			return 0;
		}
		return 0;
	case WM_TIMER:
		if (inGame) {
			Step();
			InvalidateRect (hWnd, 0, 0);
		}
		return 0;
	case WM_PAINT:
		FillRect (hdcBack, &clientRect, 0);
		Draw();
		hdc = BeginPaint (hWnd, &ps);
		BitBlt (hdc, 0, 0, windowSize, windowSize, hdcBack, 0, 0, SRCCOPY);
		EndPaint (hWnd, &ps);
		return 0;
	default:
		return DefWindowProc (hWnd, uMsg, wParam, lParam);
	}

}

void Init()
{

		if (snake) {
			free (snake);
		}
		length = 2;
		snake = (point *)calloc (length, sizeof(point));
		snake[0].x = 1;
		snake[0].y = 0;
		snake[1].x = 0;
		snake[1].y = 0;
		RandomPoint (&apple);
		prevTime = 0;
		inGame = false;

}

void Step()
{

	int i;
	point temp;
	point *oldSnake;

	temp = snake[0];
	switch (snakeDir) {
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
	if (temp.x == apple.x && temp.y == apple.y) {
		score += 10;
		length ++;
		oldSnake = snake;
		snake = (point *)calloc (length, sizeof(point));
		if (oldSnake) {
			for (i = 1; i < length; i++) {
				snake[i] = oldSnake[i - 1];
			}
		}
		snake[0] = apple;
		RandomPoint (&apple);
		free (oldSnake);
	} else {
		for (i = length - 1; i > 0; i--) {
			snake[i] = snake[i - 1];
		}
		snake[0] = temp;
	}
	if (IsInSnake (temp) || temp.x < 0 || temp.y < 0 || temp.x >= width || temp.y >= height) {
		End();
	}

}

void Draw()
{

	int i;
	RECT r;

	for (i = 1; i < height; i++) {
		MoveToEx (hdcBack, i * step, 0, 0);
		LineTo (hdcBack, i * step, windowSize);
	}
	for (i = 1; i < width; i++) {
		MoveToEx (hdcBack, 0, i * step, 0);
		LineTo (hdcBack, windowSize, i * step);
	}
	r.left = apple.x * step + 5;
	r.right = r.left + step - 10;
	r.top = apple.y * step + 5;
	r.bottom = r.top + step - 10;
	SelectObject (hdcBack, appleBrush);
	Ellipse (hdcBack, r.left, r.top, r.right, r.bottom);
	SelectObject (hdcBack, GetStockObject (WHITE_BRUSH));
	for (i = 0; i < length; i++) {
		r.left = snake[i].x * step + 3;
		r.right = r.left + step - 5;
		r.top = snake[i].y * step + 3;
		r.bottom = r.top + step - 5;
		FillRect (hdcBack, &r, (HBRUSH)10);
	}

}

void End()
{

	wchar_t *scoreText;

	inGame = false;
	if (length == 256) {
		MessageBox (0, L"You win!", L"Congratulations!", 0);
	}
	scoreText = (wchar_t *)calloc (5, sizeof(wchar_t));
	GetStringFromInt (score, scoreText);
	if (MessageBox (0, L"Play again?", scoreText, MB_YESNO) != IDYES) {
		ExitProcess (0);
	}
	Init();

}

void RandomPoint (point *p)
{

	do {
		unsigned char time = ((char)GetTickCount ()) % (width * height);
		p->x = time / width;
		p->y = time % height;
	} while (IsInSnake (*p));

}

void GetStringFromInt (int value, wchar_t *result)
{

	int index;

	for (index = 0; index < 4; index++) {
		result[index] = L'0';
	}
	index = 3;
	do {
		result[index--] = L'0' + value % 10;
		value /= 10;
	} while (value);
	while (index >= 0) {
		result[index--] = L' ';
	}

}

boolean IsInSnake(point p)
{

	int i;

	for (i = 1; i < length; i++) {
		if (snake[i].x == p.x && snake[i].y == p.y) {
			return true;
		}
	}
	return false;

}