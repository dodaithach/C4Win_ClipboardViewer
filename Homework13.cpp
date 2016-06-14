// Homework13.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Homework13.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
char*		g_clipboardAnsi = NULL;
wchar_t*	g_clipboardUnicode = NULL;
HBITMAP		g_clipboardBitmap = NULL;


// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

void				MyStrCopy(char* dest, char* src, int count);
void				MyStrCopy(wchar_t* dest, wchar_t* src, int count);
void				MyRefresh(HWND hWnd);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_HOMEWORK13, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_HOMEWORK13));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_HOMEWORK13));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_HOMEWORK13);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;

		case ID_FILE_PASTEANSITEXT: {
			hdc = GetDC(hWnd);
			RECT rect;
			GetClientRect(hWnd, &rect);

			if (OpenClipboard(hWnd)) {
				HANDLE data = GetClipboardData(CF_TEXT);
				if (data != NULL) {
					MyRefresh(hWnd);
					
					char* pData = (char*) GlobalLock(data);
					g_clipboardAnsi = new char[strlen(pData) + 1];
					MyStrCopy(g_clipboardAnsi, pData, strlen(pData));
					GlobalUnlock(data);

					DrawTextA(hdc, g_clipboardAnsi, strlen(g_clipboardAnsi), &rect, DT_LEFT);
				}
				else
					MessageBox(hWnd, L"No ANSI in clipboard", NULL, MB_OK);

				CloseClipboard();
				
			}

			ReleaseDC(hWnd, hdc);

			break;
									}

		case ID_FILE_PASTEUNICODETEXT: {
			hdc = GetDC(hWnd);
			RECT rect;
			GetClientRect(hWnd, &rect);

			if (OpenClipboard(hWnd)) {
				HANDLE data = GetClipboardData(CF_UNICODETEXT);
				if (data != NULL) {
					MyRefresh(hWnd);
					
					wchar_t* pData = (wchar_t*) GlobalLock(data);
					g_clipboardUnicode = new wchar_t[wcslen(pData) + 1];
					MyStrCopy(g_clipboardUnicode, pData, wcslen(pData));
					GlobalUnlock(data);

					DrawTextW(hdc, g_clipboardUnicode, wcslen(g_clipboardUnicode), &rect, DT_LEFT);
				}
				else
					MessageBox(hWnd, L"No Unicode in clipboard", NULL, MB_OK);

				CloseClipboard();
				
			}

			ReleaseDC(hWnd, hdc);

			break;
									   }

		case ID_FILE_PASTEBITMAP: {
			hdc = GetDC(hWnd);
			RECT rect;
			GetClientRect(hWnd, &rect);

			if (OpenClipboard(hWnd)) {
				HBITMAP hBitmap = (HBITMAP) GetClipboardData(CF_BITMAP);
				if (hBitmap != NULL) {
					MyRefresh(hWnd);
					
					GlobalLock(hBitmap);
					g_clipboardBitmap = (HBITMAP) CopyImage(hBitmap, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
					GlobalUnlock(hBitmap);
					
					HDC memDC = CreateCompatibleDC(hdc);
					HBITMAP oldBitmap = (HBITMAP) SelectObject(memDC, g_clipboardBitmap);
					BitBlt(hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, memDC, 0, 0, SRCCOPY);
					SelectObject(memDC, oldBitmap);

					DeleteDC(memDC);
				}
				else
					MessageBox(hWnd, L"No Bitmap in clipboard", NULL, MB_OK);

				CloseClipboard();
			}

			ReleaseDC(hWnd, hdc);

			break;
								  }

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		RECT rect;
		GetClientRect(hWnd, &rect);
		
		if (g_clipboardAnsi != NULL) {
			DrawTextA(hdc, g_clipboardAnsi, strlen(g_clipboardAnsi), &rect, DT_LEFT);
		}

		if (g_clipboardUnicode != NULL) {
			DrawTextW(hdc, g_clipboardUnicode, wcslen(g_clipboardUnicode), &rect, DT_LEFT);
		}

		if (g_clipboardBitmap != NULL) {
			HDC memDC = CreateCompatibleDC(hdc);
			HBITMAP oldBitmap = (HBITMAP) SelectObject(memDC, g_clipboardBitmap);
			BitBlt(hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, memDC, 0, 0, SRCCOPY);
			SelectObject(memDC, oldBitmap);

			DeleteDC(memDC);
		}

		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void MyStrCopy(char* dest, char* src, int count) {
	for (int i = 0; i < count; i++)
		dest[i] = src[i];

	dest[count] = '\0';
}

void MyStrCopy(wchar_t* dest, wchar_t* src, int count) {
	for (int i = 0; i < count; i++)
		dest[i] = src[i];

	dest[count] = L'\0';
}

void MyRefresh(HWND hWnd) {
	HDC hdc = GetDC(hWnd);
	RECT rect;
	GetClientRect(hWnd, &rect);

	FillRect(hdc, &rect, (HBRUSH) (COLOR_WINDOW + 1));
	ReleaseDC(hWnd, hdc);

	if (g_clipboardAnsi != NULL) {
		delete g_clipboardAnsi;
		g_clipboardAnsi = NULL;
	}

	if (g_clipboardUnicode != NULL) {
		delete g_clipboardUnicode;
		g_clipboardUnicode = NULL;
	}

	if (g_clipboardBitmap != NULL) {
		BITMAP* tmp = NULL;
		GetObject(g_clipboardBitmap, sizeof(BITMAP), tmp);
		if (tmp != NULL)
			delete tmp;
		
		g_clipboardBitmap = NULL;
	}
}