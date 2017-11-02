#include <Windows.h>
#include "resource.h"
#include <fstream>
#include <io.h>
#include <fcntl.h>
#include <locale>
#include <codecvt>
#include <string>
#include <windowsx.h>
using namespace std;

#define ID_EDIT 100

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL FAR PASCAL CreateFont(HFONT&);
BOOL OpenFile(HWND, WCHAR[]);
BOOL SaveFile(HWND, WCHAR[]);
void writeFile(HWND, WCHAR[]);
void readFile(HWND, WCHAR[]);
int  WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int);
