// WinAPI_Proj.cpp : ���ø����̼ǿ� ���� �������� �����մϴ�.
//

#include "framework.h"
#include "WinAPI_Proj.h"
#include <vector>
#include <math.h>
#include <commdlg.h>
#include <stdio.h>
#include "CObject.h"
using namespace std;



#pragma comment(lib,"msimg32.lib")
HBITMAP hBackImage;
BITMAP bitBack;

HBITMAP hTransparentImage;
BITMAP bitTransparent;

void CreateBitmap();
void DrawBitmap(HWND hWnd, HDC hdc);
void DeleteBitmap();



#define PI 3.141592
#define degreeToRadian(degree) ((degree)*PI/180)
#define MAX_LOADSTRING 100

template<class T>
void DrawGrid(HDC hdc, POINT center, T width, T height, int xCount, int yCount);

template<class T>
void SunFlower(HDC hdc, POINT center, T radius, int count);
void DrawCircle(HDC hdc, POINT pt, BOOL bFlag);
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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    static TCHAR lpstrFile[100] = _T("");
    static FILE* fPtr;

    static TCHAR msg[100];
    static TCHAR chat[10][100];

    enum { CIRCLE, RECTANGLE, STAR, NONE };

    static int count, yPos;
    static int strNum;
    static SIZE size;

    static POINT ptCurPos;
    static POINT ptMousePos,ptMousePosPrev,ptMousePosCrt;
    static bool drawCheck = false;
    static bool dragCheck = false;
    static int selectedMenu = NONE;



    static HMENU hMenu,hSubMenu;
    static bool bFlag = false;
    static bool cFlag = false;
    static bool dFlag = false;
    static int pCount = 0;
    static vector<CObject> vCopu;

    CreateBitmap();

    switch (message)
    {
    case WM_CREATE:
        //��� ������ó�� ������ ������ �� �� �����
        count = 0;
        yPos = 15;
        strNum = 0;
        CreateCaret(hWnd, NULL, 5, 15);
        ShowCaret(hWnd);
        hMenu = GetMenu(hWnd);
        hSubMenu = GetSubMenu(hMenu, 2);
        EnableMenuItem(hSubMenu, ID_EDITCOPY, MF_GRAYED);
        EnableMenuItem(hSubMenu, ID_EDITPASTE, MF_GRAYED);
        ptCurPos.x = 50;
        ptCurPos.y = 50;


        break;

    case WM_CHAR:
    {
        

    } break;

    case WM_KEYDOWN:
    {
        int breakpoint = 999;
    }
    break;
    case WM_KEYUP:
    {
        if (wParam == 'C' || wParam == 'c')
        {
            UINT state = GetMenuState(hSubMenu, ID_EDITCOPY, MF_BYCOMMAND);
            if ((state & MF_DISABLED) || (state & MF_GRAYED))
            {
                EnableMenuItem(hSubMenu, ID_EDITCOPY, MF_ENABLED);
            }
            else if (state == MF_ENABLED)
            {
                EnableMenuItem(hSubMenu, ID_EDITCOPY, MF_GRAYED);

            }
            

        }

        int breakpoint = 999;
    }
    break;

    case WM_MOUSEMOVE:
    {
        //ptCurPos.x= LOWORD(lParam);
        //ptCurPos.y = HIWORD(lParam);

        if (dragCheck)
        {
            ptMousePosCrt.x= LOWORD(lParam);
            ptMousePosCrt.y = HIWORD(lParam);
        InvalidateRect(hWnd, NULL, TRUE);
        }

    }
    break;

    case WM_LBUTTONDOWN:
    {
        ptMousePos.x = LOWORD(lParam);
        ptMousePos.y = HIWORD(lParam);
        if (InCircle(ptMousePos, ptCurPos))
        {
            UINT state = GetMenuState(hSubMenu, ID_EDITCOPY, MF_BYCOMMAND);
            if ((state & MF_DISABLED) || (state & MF_GRAYED))
            {
                EnableMenuItem(hSubMenu, ID_EDITCOPY, MF_ENABLED);
            }

            bFlag = TRUE;
        }


        if (bFlag && cFlag)
        {
            dFlag = true;
            bFlag = false;
            cFlag = false;
        }

       


        ptMousePosPrev.x = LOWORD(lParam);
        ptMousePosPrev.y = HIWORD(lParam);
        dragCheck = true;
        InvalidateRect(hWnd, NULL, TRUE);
    }
    break;

    case WM_LBUTTONUP:
    {
        ptMousePos.x = LOWORD(lParam);
        ptMousePos.y = HIWORD(lParam);
        dragCheck = false;


        InvalidateRect(hWnd, NULL, TRUE);
    }
    break;


    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        hdc = BeginPaint(hWnd, &ps);

        // �޴� ������ ���� �м��մϴ�:
        switch (wmId)
        {
        case ID_DRAW_CIRCLE: //�� �׸���
        {
            int temp = selectedMenu;

            int ans = MessageBox(hWnd, _T("�� �׸���?"), _T("���� ����"), MB_YESNOCANCEL);
            if (ans == IDYES)
            {
                selectedMenu = CIRCLE;
            }
            else if (ans == IDNO)
            {
                selectedMenu = NONE;
            }
            else
            {
                selectedMenu = temp;
            }
        }
        break;

        case ID_EDITCOPY:
        {
    
            EnableMenuItem(hSubMenu, ID_EDITCOPY, MF_GRAYED);
            EnableMenuItem(hSubMenu, ID_EDITPASTE, MF_ENABLED);
            //���õ� �׸� ����
        }break;

        case ID_EDITPASTE:
        {
            EnableMenuItem(hSubMenu, ID_EDITPASTE, MF_GRAYED);
            cFlag = true;
        }break;



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


        //DrawCircle(hdc, ptCurPos, bFlag);


        //for (int  i = 0; i < pCount; i++)
        //{
        //    //���� ���
        //}

        //if (dFlag)
        //{
        //    DrawCircle(hdc, ptMousePos, bFlag);
        //    dFlag = false;
        //    //���Ϳ��� ����
        //}
        
        DrawBitmap(hWnd, hdc);



        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:

        DeleteBitmap();
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
void DrawCircle(HDC hdc, POINT pt, BOOL bFlag)
{

    if (bFlag)
    {
        SelectObject(hdc, GetStockObject(LTGRAY_BRUSH));
        
    }
    Ellipse(hdc, pt.x - circleRadius, pt.y - circleRadius, pt.x + circleRadius, pt.y + circleRadius);

}



template<class T>
void Star(HDC hdc, POINT center, T radius)
{
    double angle = degreeToRadian(72);
    double cosA = cos(angle);



    angle = degreeToRadian(18);
    double bh = radius * cos(angle);



}

void CreateBitmap()
{
    { // >> ����
        hBackImage = (HBITMAP)LoadImage(NULL, TEXT("images/����.bmp")
            , IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

        if (hBackImage == NULL)
        {
            DWORD dwError = GetLastError();
            MessageBox(NULL, _T("�̹��� �ε� ����"), _T("����"), MB_OK);
            return;
        }

        GetObject(hBackImage, sizeof(BITMAP), &bitBack);

    }

    {  // �ð�
        hTransparentImage = (HBITMAP)LoadImage(NULL, TEXT("images/sigong.bmp")
            , IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

        if (hTransparentImage == NULL)
        {
            DWORD dwError = GetLastError();
            MessageBox(NULL, _T("�̹��� �ε� ����2"), _T("����"), MB_OK);
            return;
        }

        GetObject(hTransparentImage, sizeof(BITMAP), &bitTransparent);
    }
   
}

void DrawBitmap(HWND hWnd, HDC hdc)
{
    HDC hMemDC;
    HBITMAP hOldBitmap;
    int bx, by;

    { //����
        hMemDC = CreateCompatibleDC(hdc);
        hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBackImage);
        bx = bitBack.bmWidth;
        by = bitBack.bmHeight;

        BitBlt(hdc, 0, 0, bx, by, hMemDC, 0, 0, SRCCOPY);

        SelectObject(hMemDC, hOldBitmap);
        DeleteDC(hMemDC);

    }
    
    {//�ð�
        hMemDC = CreateCompatibleDC(hdc);
        hOldBitmap = (HBITMAP)SelectObject(hMemDC, hTransparentImage);
        bx = bitTransparent.bmWidth;
        by = bitTransparent.bmHeight;

        //BitBlt(hdc, 150, 150, bx, by, hMemDC, 0, 0, SRCCOPY);
        TransparentBlt(hdc, 150, 150, bx, by, hMemDC, 0, 0, bx, by, RGB(255, 0, 255));
        //RGB �ڸ��� �÷�Ű��

       // SelectObject(hMemDC, hOldBitmap);
        DeleteDC(hMemDC);
    }
}

void DeleteBitmap()
{
    DeleteObject(hBackImage);
    DeleteObject(hTransparentImage);

}
