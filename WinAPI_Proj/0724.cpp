// WindowsProject1.cpp : ���ø����̼ǿ� ���� �������� �����մϴ�.
//

#include "pch.h"
#include "framework.h"
#include "WinAPI_Proj.h"
#include "CCore.h"
#include "Player.h"
#include <process.h>
using namespace std;
#define MAX_LOADSTRING 100


HWND* mainHwnd;
HDC* mainHdc;

HWND g_hWnd;
HDC g_hdc;

// ���� ����:
HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.
HWND hWnd;

#ifdef UNICODE
#pragma comment(linker,"/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker,"/entry:WinMainCRTStartup /subsystem:console")
#endif


// �� �ڵ� ��⿡ ���Ե� �Լ��� ������ �����մϴ�:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ���⿡ �ڵ带 �Է��մϴ�.

    // ���� ���ڿ��� �ʱ�ȭ�մϴ�.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINAPIPROJ, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ���ø����̼� �ʱ�ȭ�� �����մϴ�:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINAPIPROJ));

    MSG msg;

    // �⺻ �޽��� �����Դϴ�:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}


//
//  �Լ�: MyRegisterClass()
//
//  �뵵: â Ŭ������ ����մϴ�.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_WINAPIPROJ));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINAPIPROJ);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   �뵵: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   �ּ�:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

    hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  �뵵: �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND  - ���ø����̼� �޴��� ó���մϴ�.
//  WM_PAINT    - �� â�� �׸��ϴ�.
//  WM_DESTROY  - ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//

#define timer_ID_1 11
#define timer_ID_2 12
const int circleRadius = 50;
double LengthPts(POINT pt1, POINT pt2)
{
    return (sqrt(
        ((float)(pt2.x - pt1.x) * (pt2.x - pt1.x)) +
        ((float)(pt2.y - pt1.y) * (pt2.y - pt1.y))
    ));
}
BOOL InCircle(POINT pt1, POINT pt2)
{
    if (LengthPts(pt1, pt2) < circleRadius) return TRUE;

    return FALSE;
}

unsigned __stdcall ThFunc(LPVOID lpParam);
vector<POINT> arrMousePos;
int idCount = 0;
POINT ptMousePos;
CRITICAL_SECTION cs;
vector<HANDLE> hThreads;
vector<DWORD> dwThIDs;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc= GetDC(hWnd);
    
    static POINT ptCurPos;


    static RECT rectView;
    static bool bFlag = false;


    

    unsigned long ulStackSize = 0;
    




    switch (message)
    {
    case WM_CREATE:
    {
        ptCurPos.x = circleRadius;
        ptCurPos.y = circleRadius;
    


        g_hWnd = hWnd;
        g_hdc = hdc;
        mainHdc = &hdc;
        mainHwnd = &hWnd;

        GetClientRect(hWnd, &rectView);

        InitializeCriticalSection(&cs);
    }
        break;





    case WM_KEYUP:
    {

        bFlag = FALSE;
        InvalidateRect(hWnd, NULL, TRUE);
    }break;


    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // �޴� ������ ���� �м��մϴ�:
        switch (wmId)
        {
        case IDM_ABOUT:
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
    case WM_LBUTTONDOWN:
    {
        ptMousePos.x = LOWORD(lParam);
        ptMousePos.y = HIWORD(lParam);

        dwThIDs.push_back(DWORD(idCount));
        HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, (unsigned(__stdcall*)(void*))ThFunc
            , NULL, 0, (unsigned*)&dwThIDs[idCount]);


        hThreads.push_back(hThread);
        arrMousePos.push_back(ptMousePos);


        //WaitForMultipleObjects(idCount, &hThreads[0], TRUE, INFINITE);

        idCount++;
        InvalidateRect(hWnd, NULL, TRUE);
    }
   
        break;

    case WM_LBUTTONUP:
        InvalidateRect(hWnd, NULL, TRUE);


        break;

    case WM_MOUSEMOVE:
        if (bFlag)
        {
            ptCurPos.x = LOWORD(lParam);
            ptCurPos.y = HIWORD(lParam);
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;
    case WM_PAINT:
    {

        // TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�...


        //Ellipse(hdc, rectView.left, rectView.top, rectView.right, rectView.bottom);

        

    }
    break;
    case WM_DESTROY:
    {
        for (int i = 0; i < idCount; i++)
        {
            CloseHandle(hThreads[i]);
        }


        DeleteCriticalSection(&cs);
        ReleaseDC(hWnd,hdc);

        PostQuitMessage(0);
    }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
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

unsigned __stdcall ThFunc(LPVOID lpParam)
{
    GetDC(g_hWnd);
    POINT _mousePos = ptMousePos;

    SelectObject(g_hdc, GetStockObject(LTGRAY_BRUSH));

    while (1)
    {
        EnterCriticalSection(&cs);
        _mousePos.x++;
        Ellipse(g_hdc, _mousePos.x - circleRadius, ptMousePos.y - circleRadius,
            _mousePos.x + circleRadius, ptMousePos.y + circleRadius);

        LeaveCriticalSection(&cs);
        Sleep(1);
    }
    ReleaseDC(g_hWnd, g_hdc);
    return 0;
}
