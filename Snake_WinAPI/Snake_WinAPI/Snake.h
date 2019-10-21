#include <Windows.h>

typedef	unsigned char boolean;
#define true 1
#define false 0
#define windowSize 800
#define constSize 16

typedef struct _point {
	byte x, y;
} point;

typedef enum _direction {
	up,
	right,
	down,
	left
} direction;

LRESULT WindowProc (HWND, UINT, WPARAM, LPARAM);
void Create (HINSTANCE);
void Init();
void Step();
void Draw();
void End();
void RandomPoint (point *);
void GetStringFromInt (int value, wchar_t *result);
boolean IsInSnake(point p);

#define Append(destination, source) wcscat_s (destination, BUFFER_SIZE, source);