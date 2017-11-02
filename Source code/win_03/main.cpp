#include "Header.h"
int  WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hIPrev, LPWSTR cmd,int cmdShow)
{
	static HINSTANCE hinst;
	hinst = hInst;
	WNDCLASS wnd;
	wnd.cbClsExtra = 0;
	wnd.cbWndExtra = 0;
	wnd.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));
	wnd.hCursor = LoadCursor(NULL, IDC_IBEAM);
	wnd.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
	wnd.hInstance = hInst;
	wnd.lpfnWndProc = WndProc;
	wnd.lpszClassName = L"Notepad";
	wnd.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wnd.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wnd);
	
	HWND  hwnd = CreateWindow(L"Notepad", L"Notepad",WS_OVERLAPPEDWINDOW, 20, 20, 600, 400, NULL, NULL, hInst, NULL);
	ShowWindow(hwnd, cmdShow);
	UpdateWindow(hwnd);
	MSG msg;

	HACCEL  h = LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_ACCELERATOR1));

	while (GetMessage(&msg,0, 0, 0))
	{
		if (!TranslateAccelerator(hwnd,h, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	static HWND hwndEdit;
	static WCHAR str[255];
	HFONT hfont;
	switch (msg)
	{
	case WM_CREATE:
		hwndEdit = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL, 0, 0, 0, 0, hwnd, (HMENU)ID_EDIT, ((LPCREATESTRUCT)lparam)->hInstance, NULL);
		break;
	case WM_SETFOCUS:
		SetFocus(hwndEdit);
		break;
	case WM_SIZE:
		MoveWindow(hwndEdit, 0, 0, LOWORD(lparam), HIWORD(lparam), TRUE);
		break;
	case WM_COMMAND:
		switch (LOWORD (wparam))
		{
		case ID_FILE_NEW:
			SendMessage(hwndEdit, EM_SETSEL, 0, -1);
			SendMessage(hwndEdit, WM_CLEAR, 0, 0);
			break;
		case ID_FILE_OPEN:
			if (OpenFile(hwnd, str))
			{
				SendMessage(hwndEdit, EM_SETSEL, 0, -1);
				SendMessage(hwndEdit, WM_CLEAR, 0, 0);
				readFile(hwndEdit, str);
			}
			break;
		case ID_FILE_SAVE:
			if (SaveFile(hwnd, str))
				writeFile(hwndEdit, str);
			break;
		case ID_FILE_EXIT:
			DestroyWindow(hwnd);
			break;
		case ID_EDIT_UNDO:
			SendMessage(hwndEdit, WM_UNDO, 0, 0);
			break;
		case ID_EDIT_CUT:
			SendMessage(hwndEdit, WM_CUT, 0, 0);
			break;
		case ID_EDIT_COPY:
			SendMessage(hwndEdit, WM_COPY, 0, 0);
			break;
		case ID_EDIT_PASTE:
			SendMessage(hwndEdit, WM_PASTE, 0, 0);
			break;
		case ID_EDIT_DELETE:
			SendMessage(hwndEdit, WM_CLEAR, 0, 0);
			break;
		case ID_EDIT_SELECTALL:
			SendMessage(hwndEdit, EM_SETSEL, 0, -1);
			break;
		case ID_FORMAT_FONT:
			if (CreateFont(hfont))
				SendMessage(hwndEdit, WM_SETFONT, (WPARAM)hfont, TRUE);
			break;
		}
		break;
	case WM_DESTROY:
	{
		int id = MessageBox(hwnd, L"Bạn có muốn save File không?", L"Thông Báo", MB_OKCANCEL);
		if (id == IDOK)
		 {
			 if (SaveFile(hwnd, str))
				 writeFile(hwndEdit, str);
		 }
			PostQuitMessage(0);
	}
		break;
	default:
		break;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}
BOOL FAR PASCAL CreateFont(HFONT &hfont)
{
	CHOOSEFONT font;
	LOGFONT log;  
	font.Flags = CF_SCREENFONTS;
	font.hwndOwner = (HWND)NULL;
	font.hInstance = NULL;
	font.hDC = (HDC)NULL;
	font.lpLogFont = &log;
	font.lStructSize = sizeof(CHOOSEFONT);
	font.lCustData = 0L;
	font.lpfnHook = NULL;
	font.lpTemplateName = NULL;
	font.lpszStyle = NULL;
	font.iPointSize = 0;
	font.nFontType = SCREEN_FONTTYPE;
	font.nSizeMin = 0;
	font.nSizeMax = 0;
	font.rgbColors = RGB(0, 0, 0);
	ChooseFont(&font);
	hfont = CreateFontIndirect(font.lpLogFont);
	return true;
}
BOOL OpenFile(HWND hwnd, WCHAR str[])
{
	OPENFILENAMEW file = { 0 };
	WCHAR sz[255];
	file.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	file.hwndOwner = hwnd;
	file.lStructSize = sizeof(file);
	file.lpstrFile = sz;
	file.lpstrFile[0] = L'\0';
	file.lpstrFilter = L"Text(*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
	file.nMaxFile = sizeof(sz);
	file.nFilterIndex = 1;
	if (!GetOpenFileName(&file))
		return FALSE;
	wcscpy(str, file.lpstrFile);
	return TRUE;
}
void readFile(HWND hwnd, WCHAR str[])
{
	_setmode(_fileno(stdin), _O_WTEXT);
	locale loc(locale(), new codecvt_utf8<wchar_t>);
	wifstream fi(str);
	fi.imbue(loc);
	if (!fi.is_open())
		return;
	WCHAR ch;
	while (!fi.eof())
	{
		fi >> ch;
		wstring s;
		s.push_back(ch);
		SendMessage(hwnd, EM_REPLACESEL, 0, (LPARAM)s.c_str());
	}
	fi.close();
}
void writeFile(HWND hwnd, WCHAR str[])
{
	_setmode(_fileno(stdout), _O_WTEXT);
	int length = GetWindowTextLength(hwnd);
	WCHAR* s = new WCHAR[length + 1];
	GetWindowText(hwnd, s, length + 1);
	locale loc(locale(), new codecvt_utf8<wchar_t>);
	wofstream fo(str);
	fo.imbue(loc);
	if (!fo.is_open())
		return;
	fo << s;
	fo.close();
	delete[]s;
}
BOOL SaveFile(HWND hwnd, WCHAR str[])
{
	OPENFILENAMEW filename = { 0 };
	WCHAR sz[255] = L"*.txt";
	filename.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	filename.hwndOwner = hwnd;
	filename.lStructSize = sizeof(filename);
	filename.lpstrFile = sz;
	filename.lpstrFilter = L"Text(*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
	filename.nMaxFile = sizeof(sz);
	filename.nFilterIndex = 1;
	if (!GetSaveFileName(&filename))
		return FALSE;
	wcscpy(str, filename.lpstrFile);
	return TRUE;
}

