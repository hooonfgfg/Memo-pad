#include "header.h"

class ProcEvent
{
private:
	HDC hDC;
	PAINTSTRUCT ps;
	HIMC hImc;
	HIMC m_hIMC = NULL;
	int lon = 0;
	int i;

	//proc 매개변수
	HWND hWnd;
	UINT iMsg;
	WPARAM wParam;
	LPARAM lParam;
	
	TCHAR szChar[3] = { '\0' };
	int line = 0;
	int str_len = 0;
	int pos = 0;
	TCHAR arr[255][255] = { '\0' };
	int input = 0;
	bool keydown_evt = false;
	int caret_x = 1, caret_y = 1;
	SIZE size;

public:

	void Set_Proc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam) { this->hWnd = hWnd; this->iMsg = iMsg; this->wParam = wParam; this->lParam = lParam; }
	void SET_FOCUS();
	void KILL_FOCUS();
	void KEY_DOWN();
	void CHAR();
	void IME_COMPOSITON();
	void PAINT();
	void DESTROY();
};
