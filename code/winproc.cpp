#include "ProcEvent.h"
ProcEvent proc;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	
	proc.Set_Proc(hWnd,iMsg,wParam,lParam);

	switch (iMsg) {

	case WM_SETFOCUS: //ĳ������
		proc.SET_FOCUS();
		return 0;

	case WM_KILLFOCUS: //�������α׷� �̿��� ���α׷� ���ý� �߻�(�ٸ� â�� ������ ĳ������������)
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

	case WM_PAINT: //ȭ�� ��ȭ �߻�
		proc.PAINT();
		break;

	case WM_DESTROY: //����
		proc.DESTROY();
		return 0;

	}

	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}
