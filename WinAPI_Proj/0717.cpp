// WinAPI_Proj.cpp : ���ø����̼ǿ� ���� �������� �����մϴ�.
//
#include "pch.h"
#include "framework.h"
#include "WinAPI_Proj.h"
#include <vector>
#include <math.h>
#include <commdlg.h>
#include <stdio.h>
using namespace std;

#define PI 3.141592
#define degreeToRadian(degree) ((degree)*PI/180)
#define MAX_LOADSTRING 100

template<class T>
void DrawGrid(HDC hdc, POINT center, T width, T height, int xCount, int yCount);
template<class T>
void DrawCircle(HDC hdc, POINT center, T radius);
template<class T>
void SunFlower(HDC hdc, POINT center, T radius, int count);
void FileRead(HWND hWnd, LPTSTR filename);
void FileSave(HWND hWnd, LPTSTR filename);


// ���� ����:
HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.

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
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINAPIPROJ));
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

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        200, 200, 1024, 768, nullptr, nullptr, hInstance, nullptr);

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

void FileRead(HWND hWnd, LPTSTR filename)
{
    HANDLE hFile;
    LPTSTR buffer;
    DWORD size, charNum;
    int fileSize;
    hFile = CreateFile(filename, GENERIC_READ, 0, 0,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    if (hFile == INVALID_HANDLE_VALUE)
        return;

    fileSize = GetFileSize(hFile, &size);
    buffer = new TCHAR[(int)fileSize / sizeof(TCHAR) + 1];
    memset(buffer, 0, fileSize);
    ReadFile(hFile, buffer, fileSize, &charNum, NULL);
    buffer[(int)fileSize / sizeof(TCHAR)] = NULL;
    SetWindowText(hWnd, buffer);
    
    delete[] buffer;
    CloseHandle(hFile);

}
void FileSave(HWND hWnd, LPTSTR filename)
{
    HANDLE hFile;
    LPTSTR buffer;
    int size;
#ifdef _UNICODE
    WORD uni = 0xfeff;
    DWORD nSize;
#endif
    hFile = CreateFile(filename, GENERIC_WRITE,
        0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

#ifdef _UNICODE
    WriteFile(hFile, &uni, 2, &nSize, NULL);
#endif
    size = GetWindowTextLength(hWnd);
    buffer = new TCHAR[size + 1];
    memset(buffer, 0, (size + 1) * sizeof(TCHAR));
    size = GetWindowText(hWnd, (LPTSTR)buffer, size + 1);
    buffer[size] = NULL;
    WriteFile(hFile, buffer, size * sizeof(TCHAR),
        (LPDWORD)&size, NULL);
    delete[] buffer;
    CloseHandle(hFile);
}


#define IDC_EDIT_NOTEPAD 123
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;



    static HWND hEdit;
    RECT rectView;

    switch (message)
    {
    case WM_CREATE:
        //��� ������ó�� ������ ������ �� �� �����
        GetClientRect(hWnd, &rectView);

        hEdit = CreateWindow(_T("edit"), NULL,
            WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL |
            ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE,
            0, 100, rectView.right, rectView.bottom,
            hWnd,
            (HMENU)IDC_EDIT_NOTEPAD,
            hInst, NULL);


        break;
    case WM_SIZE:
        GetClientRect(hWnd, &rectView);
        MoveWindow(hEdit,0,0,rectView.right,rectView.bottom,TRUE);
        break;
    case WM_CHAR:
    {
       

    } break;

    case WM_KEYDOWN:
    {
       
    }
    break;
    case WM_KEYUP:
    {
        
    }
    break;

    case WM_MOUSEMOVE:
    {

    }
    break;

    case WM_LBUTTONDOWN:
    {
       
    }
    break;



    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        hdc = BeginPaint(hWnd, &ps);

        // �޴� ������ ���� �м��մϴ�:
        switch (wmId)
        {
        
        case ID_BUTTON_FILEOPEN:
            FileRead(hEdit, (LPTSTR)_T("test2.txt"));
            break;
        case ID_BUTTON_SAVE:
            FileSave(hEdit, (LPTSTR)_T("test2.txt"));
            break;
        


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
    case WM_PAINT:
    {
        hdc = BeginPaint(hWnd, &ps);
        //TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�...

       

        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
      
        PostQuitMessage(0);
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

//��ŸƮ ���� ����

template<class T>
void DrawGrid(HDC hdc, POINT center, T width, T height, int xCount, int yCount)
{
    POINT start = { center.x - width / 2 , center.y - height / 2 };
    POINT end = { center.x + width / 2 , center.y + height / 2 };


    T xInterval = width / xCount;
    T yInterval = height / yCount;

    for (int i = 0; i < xCount + 1; i++)
    {
        MoveToEx(hdc, start.x + xInterval * i, start.y, NULL);
        LineTo(hdc, start.x + xInterval * i, end.y);
    }

    for (int i = 0; i < yCount + 1; i++)
    {
        MoveToEx(hdc, start.x, start.y + yInterval * i, NULL);
        LineTo(hdc, end.x, start.y + yInterval * i);
    }

}

template<class T>
void DrawCircle(HDC hdc, POINT center, T radius)
{
    Ellipse(hdc, center.x - radius, center.y - radius, center.x + radius, center.y + radius);

}

template<class T>
void SunFlower(HDC hdc, POINT center, T radius, int count)
{
    //360/count = �� �ϳ��� ���� �� �ִ� ����
    //3.141592
    //1�� = 0.0174533 ����
    //���ʿ� + �ܰ���  
    //������ + �ܰ��� �� ���� ������ ������ 360/count



    double angle = degreeToRadian(360 / count);
    double val = sin(angle);

    double r = (sin(angle / 2) * radius) / (1 - sin(angle / 2));

    DrawCircle(hdc, center, radius);

    for (int i = 1; i <= count; i++)
    {
        double t = (360 / count) * i;
        double tAngle = degreeToRadian(t);

        double tempX = cos(tAngle) * (radius + r);
        double tempY = sin(tAngle) * (radius + r);


        POINT temp = { center.x + tempX, center.y + tempY };
        DrawCircle(hdc, temp, r);
    }
}

template<class T>
void Star(HDC hdc, POINT center, T radius)
{
    double angle = degreeToRadian(72);
    double cosA = cos(angle);



    angle = degreeToRadian(18);
    double bh = radius * cos(angle);



}