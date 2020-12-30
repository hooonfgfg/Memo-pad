#include "ProcEvent.h"

void ProcEvent::SET_FOCUS()
{
	printf("ĳ��@����\n");
	CreateCaret(hWnd, NULL, 2, 15);
	SetCaretPos(1, 10 * line); //������ġ
	ShowCaret(hWnd);
}

void ProcEvent::KILL_FOCUS()
{
	printf("ĳ��@����\n");
	HideCaret(hWnd);
	DestroyCaret();
}

void ProcEvent::KEY_DOWN()
{
	printf("###########KEYDOWN�̺�Ʈ �߻�#############\n\n");
	keydown_evt = true;
	switch (wParam) {
	case VK_BACK: //�齺���̽�

		str_len = strlen(arr[input]);
		if (input >= 1 && str_len <= 0) { //�ι�° ���̻�, ���� 0����
			input--;
			line -= 2;
			caret_y = line;
		}
		if (str_len >= 1) {//1�̻�				
			if (arr[input][str_len - 1] & 0x80) { //�ѱ�?
				printf("�ѱ��̴�. 1����Ʈ�� ����ؼ� ?�� ���� : %c\n", arr[input][str_len - 2]);
				arr[input][str_len - 1] = '\0'; //�ѱ� �����
				arr[input][str_len - 2] = '\0'; //�ڿ�ĭ �ϳ� �� �����
			}
			else {
				arr[input][str_len - 1] = '\0'; //�׹��� ���(����)
			}
		}
		str_len = strlen(arr[input]);
		break;
	}
	InvalidateRect(hWnd, NULL, FALSE);
}

void ProcEvent::CHAR()
{
	
	printf("#################### CHAR �̺�Ʈ ########################\n");
	if (wParam == VK_BACK) { //�齺���̽� return
		return;
	}
	if (wParam == VK_RETURN) { //ENTER �Է½�
		arr[input][str_len] = '\0'; //�ʱ�ȭ
		input++; // arr �� ����
		line += 2; //rectangle �� ����
		str_len = strlen(arr[input]);
		return;
	}

	hDC = GetDC(hWnd);
	SetTextColor(hDC, RGB(100, 100, 250)); //����

	//�����Է½ø��� Ű�ٿ��̺�Ʈ�� �߻� => WM_CHAR�� WM_IME_COMP�� �����ʿ� => WNM_IME_COMP�� ��ĥ�� Ű�ٿ��̺�Ʈ�� false�� ���� ��, ���չ��ڰ� �ƴ� 1����Ʈ ���ڸ� �Է½ÿ��� true
	if (keydown_evt == true)//Ű�ٿ� �̺�Ʈ ���ý�(���� ���� true)
	{
		str_len = strlen(arr[input]); //����
		arr[input][str_len] = (TCHAR)wParam; //��
		arr[input][str_len + 1] = '\0';
		str_len = strlen(arr[input]);
		keydown_evt = false;
	}

	else {} //�ѱ��Է�(COMP false) PASS
	InvalidateRect(hWnd, NULL, FALSE);
	printf("WM_CHAR �̺�Ʈ ����: %s\n", arr[input]);
	printf("######################## CHAR ���� #####################\n\n");

	//----------------------------------------------------------------------
}

void ProcEvent::IME_COMPOSITON()
{
	printf("WM_IME_COMPOSITION �̺�Ʈ �߻�!!!!!!!!!!\n\n");
	hImc = ImmGetContext(hWnd);
	if (lParam&GCS_COMPSTR) { //������

		lon = ImmGetCompositionString(hImc, GCS_COMPSTR, NULL, 0);
		ImmGetCompositionString(hImc, GCS_COMPSTR, szChar, lon);
		szChar[lon] = '\0';
		printf("���յ��� �齺���̽� : %d \n", strlen(szChar));
		if (strlen(szChar) == 0) {
			arr[input][str_len] = '\0'; //�ʱ�ȭ����
		}
		for (int i = 0; i < strlen(szChar); i++) {
			arr[input][str_len + i] = szChar[i]; //arr �迭�� ��Ʈ �������
		}

	}
	else if (lParam&GCS_RESULTSTR) { //������ �ϼ�

		lon = ImmGetCompositionString(hImc, GCS_RESULTSTR, NULL, 0);
		ImmGetCompositionString(hImc, GCS_RESULTSTR, szChar, lon);
		for (int i = 0; i < strlen(szChar); i++) {
			arr[input][str_len + i] = szChar[i]; //arr �迭�� ��Ʈ �������
		}
		str_len = strlen(arr[input]); //�迭�� ����
	}

	keydown_evt = false;
	ImmReleaseContext(hWnd, hImc);
	szChar[lon] = '\0';
	printf("���յ��� �齺���̽�-2- : %d \n", strlen(szChar));
	//----------------------------------------------------------------------
}

void ProcEvent::PAINT()
{
	printf("----------WM_PAINT : paint���� �߻� ------------\n");
	hDC = BeginPaint(hWnd, &ps);
	GetTextExtentPoint(hDC, arr[input], strlen(arr[input]) + 1, &size); //arr input�� ���� ������ ���ο� ������ ũ�� ���ϱ� (��ǻ� ���θ� �ʿ�)
	SetCaretPos(size.cx, 10 * line); // ĳ�� ��ġ ����(���� ���̿� �ٿ� �°�. ��, �ؽ�Ʈ���� �ȼ������ �ٸ��Ƿ� �ȼ��� ���ؼ� �߰��������.

	Rectangle(hDC, 0, 10 * line, 10000, 10000);
	printf("-->SzChar ���� %s \n", szChar);
	TextOut(hDC, 10, 10 * line, arr[input], strlen(arr[input]));
	EndPaint(hWnd, &ps); //����Ʈ ����
	printf("==========WM_PAINT : paint���� ����=============\n\n");
	return;
}

void ProcEvent::DESTROY()
{
	PostQuitMessage(0);
	return;
}
