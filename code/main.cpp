#include "header.h"
using namespace std;

INT APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE, TCHAR *pszLine, INT nShow) { //�ν��Ͻ� �ڵ�, �ü��ȣȯ, �μ�, ���α׷� ����� ����
	
	WNDCLASS wc;
	HWND hWnd;
	MSG msg;

	wc.style = CS_OWNDC; //�����콺Ÿ��
	wc.lpfnWndProc = WndProc; //�޼��� ó��
	wc.cbClsExtra = 0; //Ŭ��������
	wc.cbWndExtra = 0; ////////////
	wc.hInstance = hInstance; // �ڵ� �ν��Ͻ�
	wc.hIcon = LoadIcon(NULL, IDC_ICON); //������
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); //Ŀ��
	wc.hbrBackground = CreateSolidBrush(RGB(255, 255, 255)); //����
	wc.lpszMenuName = NULL; //�޴�
	wc.lpszClassName = TEXT("MyApp");  //Ŭ�����̸�

	RegisterClass(&wc); //��������

	hWnd = CreateWindow(TEXT("MyApp"), TEXT("Sample App"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	//Ŭ����, Ÿ��Ʋ, â ����, x��, y��, x�� ��ġ, y�� ��ġ, �ڵ�
	ShowWindow(hWnd, SW_SHOW); //������ ���

	while (GetMessage(&msg, NULL, NULL, NULL)) { //�޼��� ����
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

