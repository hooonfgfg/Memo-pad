#include "ProcEvent.h"

void ProcEvent::SET_FOCUS()
{
	printf("캐럿@생성\n");
	CreateCaret(hWnd, NULL, 2, 15);
	SetCaretPos(1, 10 * line); //시작위치
	ShowCaret(hWnd);
}

void ProcEvent::KILL_FOCUS()
{
	printf("캐럿@삭제\n");
	HideCaret(hWnd);
	DestroyCaret();
}

void ProcEvent::KEY_DOWN()
{
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
}

void ProcEvent::CHAR()
{
	
	printf("#################### CHAR 이벤트 ########################\n");
	if (wParam == VK_BACK) { //백스페이스 return
		return;
	}
	if (wParam == VK_RETURN) { //ENTER 입력시
		arr[input][str_len] = '\0'; //초기화
		input++; // arr 행 증가
		line += 2; //rectangle 줄 증가
		str_len = strlen(arr[input]);
		return;
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

	//----------------------------------------------------------------------
}

void ProcEvent::IME_COMPOSITON()
{
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
}

void ProcEvent::PAINT()
{
	printf("----------WM_PAINT : paint동작 발생 ------------\n");
	hDC = BeginPaint(hWnd, &ps);
	GetTextExtentPoint(hDC, arr[input], strlen(arr[input]) + 1, &size); //arr input의 길이 까지의 가로와 세로의 크기 구하기 (사실상 가로만 필요)
	SetCaretPos(size.cx, 10 * line); // 캐럿 위치 조정(문자 길이와 줄에 맞게. 단, 텍스트마다 픽셀사이즈가 다르므로 픽셀값 구해서 추가해줘야함.

	Rectangle(hDC, 0, 10 * line, 10000, 10000);
	printf("-->SzChar 문자 %s \n", szChar);
	TextOut(hDC, 10, 10 * line, arr[input], strlen(arr[input]));
	EndPaint(hWnd, &ps); //프린트 종료
	printf("==========WM_PAINT : paint동작 종료=============\n\n");
	return;
}

void ProcEvent::DESTROY()
{
	PostQuitMessage(0);
	return;
}
