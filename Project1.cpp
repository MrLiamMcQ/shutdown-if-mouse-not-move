// Project1.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Project1.h"
#include <iostream>
#include <thread> 
#include <time.h>
#include <string>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
clock_t t = clock()* CLK_TCK;
RECT rect;
HWND hWnd;
PAINTSTRUCT ps;
int theCmdShow=0;
bool spawnWindow = false;
HWND hInputTextBefore;
HWND hInputTextAfter;
int timeBeforeShutDownStart = 30;
int timeAfterShutDownStart = 10;
bool isMinimised = false;
bool timerIsStarted = false;

void drawTheTime(int theTime) {
	std::wstring mystr = std::to_wstring(theTime);
	mystr.append(TEXT("   "));
	DrawText(ps.hdc, (LPCWSTR)mystr.c_str(), -1, &rect, DT_SINGLELINE | DT_NOCLIP);
}

void setShutDown(const char* shutDownType) {
	char ShutDown[35] = "shutdown ";
	strcat_s(ShutDown, shutDownType);
	system(ShutDown);
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);


void drawMessageToDisplay() {

	HDC HDC_Desktop = GetDC(NULL);
	RECT m_Rect;
	GetWindowRect(hWnd, &m_Rect);
	HFONT Font= CreateFont(96, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_CHARACTER_PRECIS, NONANTIALIASED_QUALITY, VARIABLE_PITCH, TEXT("Impact"));
	const char* message = "COMPUTER SHUTING DOWN IF YOU DO NOT MOVE MOUSE";

	while (timerIsStarted) {
		SelectObject(HDC_Desktop, Font);
		TextOutA(HDC_Desktop, 100, 100, message, strlen(message));
		DeleteObject(Font);
		Sleep(1);
	}
	int a;
}

void loadSettings() {
	FILE * loadF;
	fopen_s(&loadF, "C:\\ProgramData\\CompSutDwnSaveFile.bin", "r");
	fread(&timeBeforeShutDownStart, sizeof(int), 1, loadF);
	fread(&timeAfterShutDownStart, sizeof(int), 1, loadF);
	fclose(loadF);
}

void saveSettings() {
	FILE * saveF;
	fopen_s(&saveF, "C:\\ProgramData\\CompSutDwnSaveFile.bin", "w");
	fwrite(&timeBeforeShutDownStart,sizeof(int),1,saveF);
	fwrite(&timeAfterShutDownStart, sizeof(int), 1, saveF);
	fclose(saveF);
}

void mainFunc() {
	LPPOINT mousePos = new POINT;
	LPPOINT oldMousePos = new POINT;

	int thurtySecs = 0;

	long a1=0;
	long b1=0;

	time_t savedtime= time(NULL);

	
	
	spawnWindow = true;

	int savedBefore = 0;
	int savedAfter = 0;

	char firstUserInput[30];
	char secondUserInput[30];

	while (1) {

		if (!isMinimised) {
			GetWindowTextA(hInputTextBefore, firstUserInput, 30);
			GetWindowTextA(hInputTextAfter,secondUserInput,30);

			savedBefore = timeBeforeShutDownStart;
			savedAfter = timeAfterShutDownStart;

			timeBeforeShutDownStart = atoi(firstUserInput);
			timeAfterShutDownStart = atoi(secondUserInput);

			if (savedBefore != timeBeforeShutDownStart || savedAfter!= timeAfterShutDownStart) {
				saveSettings();
			}

			drawTheTime(thurtySecs);
		}


		time_t timer=time(NULL);

		thurtySecs = timer - savedtime;

		if (thurtySecs == timeBeforeShutDownStart *60) {

			char words[35];
			sprintf_s(words, "-s -t %d", timeAfterShutDownStart * 60);
			setShutDown(words);
			timerIsStarted = true;
			std::thread drawWarning(drawMessageToDisplay);
			drawWarning.detach();
		}

		GetCursorPos(mousePos);

		oldMousePos->x = a1;
		oldMousePos->y = b1;

		a1 = mousePos->x;
		b1 = mousePos->y;

		if (mousePos->x != oldMousePos->x && mousePos->y != oldMousePos->y) {
			savedtime= time(NULL);
			if (timerIsStarted) {
				setShutDown("/a");
				timerIsStarted = false;
			}
		}

		Sleep(10);
		
	}
}

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int width,hight;
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
	theCmdShow = nCmdShow;
    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROJECT1));
	

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
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
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PROJECT1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;

    return RegisterClassExW(&wcex);
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
   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 340, 180, nullptr, nullptr, hInstance, nullptr);

   CreateWindowW(L"static", L"m till SD Start:", WS_VISIBLE | WS_CHILD, 10, 60, 120, 20, hWnd, 0, 0, 0);
   CreateWindowW(L"static", L"m after SD Start:", WS_VISIBLE | WS_CHILD, 10, 90, 120, 20, hWnd, 0, 0, 0);

   hInputTextBefore = CreateWindowW(L"edit", 0, WS_VISIBLE | WS_CHILD | WS_BORDER, 140, 60, 150, 25, hWnd, 0, 0, 0);
   hInputTextAfter = CreateWindowW(L"edit", 0, WS_VISIBLE | WS_CHILD | WS_BORDER, 140, 90, 150, 25, hWnd, 0, 0, 0);

   struct stat buff;
   if (stat("C:\\ProgramData\\CompSutDwnSaveFile.bin", &buff) != 0) {
	   saveSettings();
   }

   loadSettings();

   char cTimeBeforeSD[30];
   _itoa_s(timeBeforeShutDownStart, cTimeBeforeSD,10);
   SetWindowTextA(hInputTextBefore, (char*)cTimeBeforeSD);

   char cTimeAfterSD[30];
   _itoa_s(timeAfterShutDownStart, cTimeAfterSD, 10);
   SetWindowTextA(hInputTextAfter, (char*)cTimeAfterSD);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   GetClientRect(hWnd, &rect);

   std::thread first(mainFunc);
   first.detach();
   
   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM) 
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (spawnWindow) {

		spawnWindow = false;
	}
    switch (message)
    {
	case WM_SYSCOMMAND://WM_SIZE
		if ((wParam & SIZE_RESTORED)== SIZE_RESTORED) {
			isMinimised = false;
		}
		if ((wParam & 0xFFF0) == SC_MINIMIZE)//SC_MINIMIZE
		{
			isMinimised = true;
		}
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
			

            case IDM_ABOUT:
				//MessageBox(NULL, L"TEXT", L"OK", MB_YESNO | MB_TOPMOST);
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
			
            //EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
		exit(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
