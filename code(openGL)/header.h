#include <Windows.h>
#include <tchar.h>
#include <cstring>
#include <stdlib.h>
#include <imm.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/freeglut.h> //freeglut
#include <iostream>
//#include <cstring>
using namespace std;
#pragma comment(lib,"imm32")

#ifdef UNICODE //콘솔창입력
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif