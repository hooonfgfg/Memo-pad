#include "header.h"

HINSTANCE g_hInst;
HWND hWndMain;
HDC hdc;
HGLRC hrc;


void DoDisplay(char srcString[][255],int input)  
{
	glClear(GL_COLOR_BUFFER_BIT); //중복츨력방지
	
	wchar_t dwChar[255];
	int ListNum;
	for (int i = 0; i < input+1; i++) {
		int nLength = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, srcString[i], -1, dwChar, 255);
		glRasterPos2f(-1,0.95-(i*0.05)); //출력 좌표
		for (int j = 0; j < nLength - 1; j++) {
			ListNum = glGenLists(1);
			wglUseFontBitmapsW(hdc, dwChar[j], 1, ListNum);
			glCallList(ListNum);
			glDeleteLists(ListNum, 1);
		}
	}
	glFinish(); //끝날때까지, flush: 버퍼지움
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	HDC hDC;
	PAINTSTRUCT ps;
	HIMC hImc;
	static TCHAR szChar[3] = { '\0' }; //유니코드 문자 저장
	int i;
	int len;
	static int line = 0; //줄
	// float형 y좌표 (opengl은 0~1사이)
	static int str_len = 0; // 문자열길이
	static TCHAR arr[255][255] = { '\0' }; //2차원 test
	static int input = 0; //배열 행 부분
	HIMC m_hIMC = NULL;
	int lon = 0;
	static bool keydown_evt = false; //키이벤트
	static int caret_x = 1, caret_y = 1;
	SIZE size;


	switch (iMsg) {
	case WM_CREATE: //create
		hWndMain = hWnd;
		PIXELFORMATDESCRIPTOR pfd; //픽셀 포멧
		int nPixelFormat;

		hdc = GetDC(hWnd);
		memset(&pfd, 0, sizeof(pfd));

		pfd.nSize = sizeof(pfd); //pfd(픽셀포맷?)크기
		pfd.nVersion = 1; //버전번호
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL; //윈도우 지원, opengl지원
		pfd.iPixelType = PFD_TYPE_RGBA; //RGBA 타입
		pfd.cColorBits = 32; //컬러비트

		nPixelFormat = ChoosePixelFormat(hdc, &pfd);
		SetPixelFormat(hdc, nPixelFormat, &pfd);

		hrc = wglCreateContext(hdc);
		wglMakeCurrent(hdc, hrc);
		return 0;

	case WM_SETFOCUS: //캐럿생성
		printf("캐럿@생성\n");
		CreateCaret(hWnd, NULL, 2, 15);
		SetCaretPos(1, 10 * line); //시작위치
		ShowCaret(hWnd);
		return 0;
	case WM_KILLFOCUS: //현재프로그램 이외의 프로그램 선택시 발생(다른 창이 눌리면 캐럿삭제후종료)
		printf("캐럿@삭제\n");
		HideCaret(hWnd);
		DestroyCaret();
		return 0;
		//캐럿 처리
	case WM_KEYDOWN:
		printf("###########KEYDOWN이벤트 발생#############\n\n");
		keydown_evt = true;
		switch (wParam) {
		case VK_BACK: //백스페이스
			str_len = strlen(arr[input]);
			if (input >= 1 && str_len <= 0) { //두번째 행이상, 길이 0이하
				input--;
				line -= 2;
				caret_y = line;
			}
			if (str_len >= 1) {//1이상
				if (arr[input][str_len - 1] & 0x80) { //한글?
					printf("한글이다. 1바이트만 출력해서 ?로 찍힘 : %c\n", arr[input][str_len - 2]);
					arr[input][str_len - 1] = '\0'; //한글 지우고
					arr[input][str_len - 2] = '\0'; //뒤에칸 하나 더 지우고
				}
				else {
					arr[input][str_len - 1] = '\0'; //그밖의 언어(영문)
				}
			}
			str_len = strlen(arr[input]);
			break;
		}
		InvalidateRect(hWnd, NULL, FALSE);
		return 0;

	case WM_CHAR:
		printf("#################### CHAR 이벤트 ########################\n");
		if (wParam == VK_BACK) { //백스페이스 return
			break;
		}
		if (wParam == VK_RETURN) { //ENTER 입력시
			arr[input][str_len] = '\0'; //초기화
			input++; // arr 행 증가
			line += 2; //rectangle 줄 증가
			str_len = strlen(arr[input]);
			break;
		}
		hDC = GetDC(hWnd);
		SetTextColor(hDC, RGB(100, 100, 250)); //색상

		//글자입력시마다 키다운이벤트가 발생 => WM_CHAR와 WM_IME_COMP와 구별필요 => WNM_IME_COMP를 거칠때 키다운이벤트를 false로 해줌 즉, 조합문자가 아닌 1바이트 문자를 입력시에만 true
		if (keydown_evt == true)//키다운 이벤트 들어올시(영어 구분 true)
		{
			str_len = strlen(arr[input]); //길이
			arr[input][str_len] = (TCHAR)wParam; //문
			arr[input][str_len + 1] = '\0';
			str_len = strlen(arr[input]);
			keydown_evt = false;
		}
		else {} //한글입력(COMP false) PASS
		InvalidateRect(hWnd, NULL, FALSE);
		printf("WM_CHAR 이벤트 종료: %s\n", arr[input]);
		printf("######################## CHAR 종료 #####################\n\n");
		break;

		//----------------------------------------------------------------------

	case WM_IME_COMPOSITION:
		printf("WM_IME_COMPOSITION 이벤트 발생!!!!!!!!!!\n\n");
		hImc = ImmGetContext(hWnd);
		if (lParam&GCS_COMPSTR) { //조합중

			lon = ImmGetCompositionString(hImc, GCS_COMPSTR, NULL, 0);
			ImmGetCompositionString(hImc, GCS_COMPSTR, szChar, lon);
			szChar[lon] = '\0';
			printf("조합도중 백스페이스 : %d \n", strlen(szChar));
			if (strlen(szChar) == 0) {
				arr[input][str_len] = '\0'; //초기화해줌
			}
			for (int i = 0; i < strlen(szChar); i++) {
				arr[input][str_len + i] = szChar[i]; //arr 배열에 비트 집어넣음
			}

		}
		else if (lParam&GCS_RESULTSTR) { //한음절 완성

			lon = ImmGetCompositionString(hImc, GCS_RESULTSTR, NULL, 0);
			ImmGetCompositionString(hImc, GCS_RESULTSTR, szChar, lon);
			for (int i = 0; i < strlen(szChar); i++) {
				arr[input][str_len + i] = szChar[i]; //arr 배열에 비트 집어넣음
			}
			str_len = strlen(arr[input]); //배열값 증가
		}

		keydown_evt = false;
		ImmReleaseContext(hWnd, hImc);
		szChar[lon] = '\0';
		printf("조합도중 백스페이스-2- : %d \n", strlen(szChar));
		//----------------------------------------------------------------------

	case WM_PAINT: //화면 변화 발생
		printf("----------WM_PAINT : paint동작 발생 ------------\n");
		hDC = BeginPaint(hWnd, &ps);
		GetTextExtentPoint(hDC, arr[input], strlen(arr[input]) + 1, &size); //arr input의 길이 까지의 가로와 세로의 크기 구하기 (사실상 가로만 필요)
		//size.cx=strlen(arr[input]) 까지의 문자열 폰트크기
		SetCaretPos(size.cx, 10 * line); // 캐럿 위치 조정(문자 길이와 줄에 맞게. 단, 텍스트마다 픽셀사이즈가 다르므로 픽셀값 구해서 추가해줘야함.

		//Rectangle(hDC, 0, 10 * line, 10000, 10000);
		printf("-->SzChar 문자 %s \n", szChar);
		//TextOut(hDC, 10, 10 * line, arr[input], strlen(arr[input]));

		DoDisplay(arr,input); //gl print
		EndPaint(hWnd, &ps); //프린트 종료
		printf("==========WM_PAINT : paint동작 종료=============\n\n");
		break;

	case WM_DESTROY: //종료
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}