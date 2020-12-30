#include <Windows.h>
#include <tchar.h>
#include <cstring>
#include <iostream>
#include <stdlib.h>
#include <imm.h>
#pragma comment(lib,"imm32")
#pragma comment( linker, "/entry:WinMainCRTStartup /subsystem:console" )//Ãß°¡
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
