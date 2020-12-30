#include "ProcEvent.h"
ProcEvent proc;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	
	proc.Set_Proc(hWnd,iMsg,wParam,lParam);

	switch (iMsg) {

	case WM_SETFOCUS: //캐럿생성
		proc.SET_FOCUS();
		return 0;

	case WM_KILLFOCUS: //현재프로그램 이외의 프로그램 선택시 발생(다른 창이 눌리면 캐럿삭제후종료)
		proc.KILL_FOCUS();
		return 0;

	case WM_KEYDOWN:
		proc.KEY_DOWN();
		return 0;

	case WM_CHAR:
		proc.CHAR();
		break;

	case WM_IME_COMPOSITION:
		proc.IME_COMPOSITON();

	case WM_PAINT: //화면 변화 발생
		proc.PAINT();
		break;

	case WM_DESTROY: //종료
		proc.DESTROY();
		return 0;

	}

	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}
