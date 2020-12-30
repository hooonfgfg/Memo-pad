#include "header.h"
using namespace std;

INT APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE, TCHAR *pszLine, INT nShow) { //인스턴스 핸들, 운영체제호환, 인수, 프로그램 실행될 상태
	
	WNDCLASS wc;
	HWND hWnd;
	MSG msg;

	wc.style = CS_OWNDC; //윈도우스타일
	wc.lpfnWndProc = WndProc; //메세지 처리
	wc.cbClsExtra = 0; //클래스세팅
	wc.cbWndExtra = 0; ////////////
	wc.hInstance = hInstance; // 핸들 인스턴스
	wc.hIcon = LoadIcon(NULL, IDC_ICON); //아이콘
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); //커서
	wc.hbrBackground = CreateSolidBrush(RGB(255, 255, 255)); //배경색
	wc.lpszMenuName = NULL; //메뉴
	wc.lpszClassName = TEXT("MyApp");  //클래스이름

	RegisterClass(&wc); //윈도우등록

	hWnd = CreateWindow(TEXT("MyApp"), TEXT("Sample App"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	//클래스, 타이틀, 창 형태, x축, y축, x축 배치, y축 배치, 핸들
	ShowWindow(hWnd, SW_SHOW); //윈도우 출력

	while (GetMessage(&msg, NULL, NULL, NULL)) { //메세지 루프
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

