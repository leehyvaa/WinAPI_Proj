// WinAPI_Proj.cpp : ���ø����̼ǿ� ���� �������� �����մϴ�.
//

#include "framework.h"
#include "WinAPI_Proj.h"
#include "pch.h"
#include <vector>
#include <math.h>
#include <commdlg.h>
#include <stdio.h>
#include "CObject.h"
using namespace std;



#pragma comment(lib,"msimg32.lib")
HBITMAP hBackImage;
BITMAP bitBack;


HBITMAP hFrontImage;
BITMAP bitFront;

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


//���ø� ������
HWND ChildWnd[2];
LRESULT CALLBACK ChildWndProc1(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ChildWndProc2(HWND, UINT, WPARAM, LPARAM);


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
   /* while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }*/

    while (true)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                break;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            //���� �����Ӹ��� ��Ȯ�ϰ� �۵��ϵ��� �ڵ带 �����ؾ���
            // update�� ���� �ð����� �۵��ϰ�,Render�� �Ź� �۵��ص� �ǰ�
            // ���� �ð����� �۵��ص� �ȴ�.
            //Update(); Ÿ�̸Ӱ� �ϴ� ������ �����
            // POINT mousePos; //���������� ����
            // GetCurSorPos(&mousePos) �̷��� mousePos�� ���콺 ��ġ���� ��
            // 
            //Render(); Paint�� �ϴ� ������ ��������
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
    RegisterClassExW(&wcex);

    wcex.lpfnWndProc = ChildWndProc1;
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = _T("Child Window Class 1");
    RegisterClassExW(&wcex);



    wcex.lpfnWndProc = ChildWndProc2;
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = _T("Child Window Class 2");
    RegisterClassExW(&wcex);



    return NULL;

    //return RegisterClassExW(&wcex);
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


// >> : ANI
HBITMAP hAniImage;
BITMAP bitAni;
const int SPRITE_SIZE_X = 57;
const int SPRITE_SIZE_Y = 52;

int RUN_FRAME_MAX = 0;
int RUN_FRAME_MIN = 0;
int curframe = RUN_FRAME_MIN;

int SPRITE_FRAME_COUNT_X = 0;
int SPRITE_FRAME_COUNT_Y = 0;

VOID CALLBACK AniProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);

HBITMAP hDoubleBufferImage;
void DrawBitmapDoubleBuffering(HWND hWnd, HDC hdc);

#define TIMER_1 1

#define TIMER_2 2

void DrawRectText(HDC hdc);
void UpdateFrame(HWND hWnd);
void DrawCircle(HDC hdc, POINT pt, BOOL bFlag);
void CenterRectangle(HDC hdc, POINT center, long double width, long double height);

RECT rectView;

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
    static POINT ptMousePos, ptMousePosPrev, ptMousePosCrt;
    static bool drawCheck = false;
    static bool dragCheck = false;
    static int selectedMenu = NONE;

    

    static HMENU hMenu, hSubMenu;
    static bool bFlag = false;
    static bool cFlag = false;
    static bool dFlag = false;
    static int pCount = 0;
    static vector<CObject> vCopu;

    CreateBitmap();

    switch (message)
    {
    case WM_SIZE:
        GetClientRect(hWnd, &rectView);
        break;

    case WM_CREATE:
        //��� ������ó�� ������ ������ �� �� �����
        count = 0;
        yPos = 15;
        strNum = 0;

        GetClientRect(hWnd, &rectView);

        CreateCaret(hWnd, NULL, 5, 15);
        ShowCaret(hWnd);
        hMenu = GetMenu(hWnd);
        hSubMenu = GetSubMenu(hMenu, 2);
        EnableMenuItem(hSubMenu, ID_EDITCOPY, MF_GRAYED);
        EnableMenuItem(hSubMenu, ID_EDITPASTE, MF_GRAYED);
        ptCurPos.x = 50;
        ptCurPos.y = 50;


        CreateBitmap();
        //SetTimer(hWnd, TIMER_2, 30, AniProc);


        //������ ����
        {
            ChildWnd[0] = CreateWindowEx(WS_EX_CLIENTEDGE, _T("Child Window Class 1"),
                NULL, WS_CHILD | WS_VISIBLE,
                0, 0, rectView.right, rectView.bottom / 2 - 1,
                hWnd, NULL, hInst, NULL);
        

            ChildWnd[1] = CreateWindowEx(WS_EX_CLIENTEDGE, _T("Child Window Class 2"),
                NULL, WS_CHILD | WS_VISIBLE,
                0, rectView.bottom/ 2+1, rectView.right, rectView.bottom / 2 - 1,
                hWnd, NULL, hInst, NULL);
        }

        break;

    /*case WM_TIMER:
        if (wParam == TIMER_1)
        {
            UpdateFrame(hWnd);
            InvalidateRect(hWnd, NULL, TRUE);
        }*/
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
            ptMousePosCrt.x = LOWORD(lParam);
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
    //case WM_PAINT:
    //{
    //    hdc = BeginPaint(hWnd, &ps);
    //    //TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�...
    //    //DrawBitmap(hWnd, hdc);
    //    //��׶���� �ѹ��� ���� �ؾ���
    //    DrawBitmapDoubleBuffering(hWnd, hdc);
    //    //���� ���۸�
    //    DrawRectText(hdc);
    //    //�ؽ�Ʈ ���� �ذ�



    //    //DrawCircle(hdc, ptCurPos, bFlag);


    //    //for (int  i = 0; i < pCount; i++)
    //    //{
    //    //    //���� ���
    //    //}

    //    //if (dFlag)
    //    //{
    //    //    DrawCircle(hdc, ptMousePos, bFlag);
    //    //    dFlag = false;
    //    //    //���Ϳ��� ����
    //    //}

    //    EndPaint(hWnd, &ps);
    //}
    break;
    case WM_DESTROY:


        KillTimer(hWnd, TIMER_2);
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

void CenterRectangle(HDC hdc, POINT center, long double width, long double height)
{
    POINT start = { center.x - width / 2 , center.y - height / 2 };
    POINT end = { center.x + width / 2 , center.y + height / 2 };

    Rectangle(hdc, start.x, start.y, end.x, end.y);
}


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
    //����
    {
        hBackImage = (HBITMAP)LoadImage(NULL, TEXT("images/����.bmp"),
            IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

        if (hBackImage == NULL)
        {
            DWORD dwError = GetLastError();
            MessageBox(NULL, _T("�̹��� �ε� ����"), _T("����"), MB_OK);
            return;
        }

        GetObject(hBackImage, sizeof(BITMAP), &bitBack);
    }

    //�ð�
    {
        hTransparentImage = (HBITMAP)LoadImage(NULL, TEXT("images/sigong.bmp"),
            IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

        if (hTransparentImage == NULL)
        {
            DWORD dwError = GetLastError();
            MessageBox(NULL, _T("�̹��� �ε� ���� 2"), _T("����"), MB_OK);
            return;
        }

        GetObject(hTransparentImage, sizeof(BITMAP), &bitTransparent);
    }

    //ani
    {
        hAniImage = (HBITMAP)LoadImage(NULL, TEXT("images/zero_run.bmp"),
            IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

        if (hAniImage == NULL)
        {
            DWORD dwError = GetLastError();
            MessageBox(NULL, _T("�̹��� �ε� ���� 3"), _T("����"), MB_OK);
            return;
        }

        GetObject(hAniImage, sizeof(BITMAP), &bitAni);
    }

    // hFrontImage; //������ ��
    {
        hFrontImage = (HBITMAP)LoadImage(NULL, TEXT("images/Background.bmp"),
            IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

        if (hFrontImage == NULL)
        {
            DWORD dwError = GetLastError();
            MessageBox(NULL, _T("�̹��� �ε� ����4"), _T("����"), MB_OK);
            return;
        }

        GetObject(hFrontImage, sizeof(BITMAP), &bitFront);
    }


    // :
    RUN_FRAME_MAX = bitAni.bmWidth / SPRITE_SIZE_X - 1;
    RUN_FRAME_MIN = 2;
    curframe = RUN_FRAME_MIN;
    SPRITE_FRAME_COUNT_X = bitAni.bmWidth / SPRITE_SIZE_X;
    SPRITE_FRAME_COUNT_Y = bitAni.bmHeight / SPRITE_SIZE_Y;

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

        /*

            bitBlt()�� �׸��� ���ϴ� ����
            âũ��
            ���Ʒ���ǥ
            �¿������ǥ

        */
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

    //ani
    {
        hMemDC = CreateCompatibleDC(hdc);
        hOldBitmap = (HBITMAP)SelectObject(hMemDC, hAniImage);
        //���ص���

        bx = bitAni.bmWidth / SPRITE_FRAME_COUNT_X;
        by = bitAni.bmHeight / SPRITE_FRAME_COUNT_Y;
        //�׸��� ũ��

        //BitBlt(hdc, 100, 100, bx, by, hMemDC, 0, 0, SRCCOPY);
        //�׳� �̹��� ���

        TransparentBlt(hdc, 150, 300, bx, by, hMemDC, 0, 0, bx, by, RGB(255, 0, 255));
        //��ũ�� ����

        SelectObject(hMemDC, hOldBitmap);
        DeleteDC(hMemDC);
    }



}

void DeleteBitmap()
{
    DeleteObject(hAniImage);
    DeleteObject(hBackImage);
    DeleteObject(hTransparentImage);
}
static int yPos = 0;

void DrawRectText(HDC hdc)
{
    TCHAR  strTest[] = _T("�̹��� ���");
    TextOut(hdc, 10, yPos, strTest, _tcslen(strTest));
}

void UpdateFrame(HWND hWnd)
{
    curframe++;
    if (curframe > RUN_FRAME_MAX)
        curframe = RUN_FRAME_MIN;
    InvalidateRect(hWnd, NULL, FALSE);
}

VOID CALLBACK AniProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
    UpdateFrame(hWnd);
    yPos += 5;
    if (yPos > rectView.bottom) yPos = 0;
}


void Update()
{
    //:wm_keydown , wm_keyup �� ������ �ﰢ������ ����
    if (GetKeyState('A') & 0x8000)//��������
    {

    }

    if (GetAsyncKeyState('W') & 0x8000)//���� �ణ ���̰� ������ Ȯ���ϰ� ���
    {

    }
}

void Render()
{

}
void DrawBitmapDoubleBuffering(HWND hWnd, HDC hdc)
{

    HDC hMemDC;
    HBITMAP hOldBitmap;
    int bx, by;

    HDC hMemDC2;
    HBITMAP hOldBitmap2;

    hMemDC = CreateCompatibleDC(hdc);
    if (hDoubleBufferImage == NULL)
    {
        hDoubleBufferImage = CreateCompatibleBitmap(hdc, rectView.right, rectView.bottom);
    }
    hOldBitmap = (HBITMAP)SelectObject(hMemDC, hDoubleBufferImage);


    {
        hMemDC2 = CreateCompatibleDC(hMemDC);
        hOldBitmap2 = (HBITMAP)SelectObject(hMemDC2, hBackImage);
        //���ص���
        bx = bitBack.bmWidth;
        by = bitBack.bmHeight;
        //�׸��� ũ��

        BitBlt(hMemDC, 0, 0, bx, by, hMemDC2, 0, 0, SRCCOPY);
        SelectObject(hMemDC2, hOldBitmap2);
        DeleteDC(hMemDC2);
    }

    {
        hMemDC2 = CreateCompatibleDC(hMemDC);
        hOldBitmap2 = (HBITMAP)SelectObject(hMemDC2, hTransparentImage);
        //���ص���

        bx = bitTransparent.bmWidth;
        by = bitTransparent.bmHeight;
        //�׸��� ũ��

        //BitBlt(hdc, 100, 100, bx, by, hMemDC, 0, 0, SRCCOPY);
        //�׳� �̹��� ���

        TransparentBlt(hMemDC, 150, 150, bx * 2, by * 2, hMemDC2, 0, 0, bx, by, RGB(255, 0, 255));
        //��ũ�� ����

        SelectObject(hMemDC2, hOldBitmap2);
        DeleteDC(hMemDC2);
    }


    //ani
    {
        hMemDC2 = CreateCompatibleDC(hMemDC);
        hOldBitmap2 = (HBITMAP)SelectObject(hMemDC2, hAniImage);
        //���ص���

        bx = bitAni.bmWidth / SPRITE_FRAME_COUNT_X;
        by = bitAni.bmHeight / SPRITE_FRAME_COUNT_Y;
        //�׸��� ũ��

        //BitBlt(hdc, 100, 100, bx, by, hMemDC, 0, 0, SRCCOPY);
        //�׳� �̹��� ���

        int xStart = curframe * bx;
        int yStart = 0;

        static int xPos = 0;
        TransparentBlt(hMemDC, 150 + xPos, 300, bx, by, hMemDC2, xStart, yStart, bx, by, RGB(255, 0, 255));
        //��ũ�� ����

        //StretchBlt(hMemDC , 150 + xPos, 300, -bx, by, hMemDC2, xStart, yStart, bx, by, SRCCOPY);
        //�÷� Ű�� ���� ����

        xPos++;
        SelectObject(hMemDC2, hOldBitmap2);
        DeleteDC(hMemDC2);

    }

   


    {
        //Front
        hMemDC2 = CreateCompatibleDC(hMemDC);
        hOldBitmap2 = (HBITMAP)SelectObject(hMemDC2, hFrontImage);
        bx = bitFront.bmWidth;
        by = bitFront.bmHeight;

        HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 255));
        HBRUSH oldBrush = (HBRUSH)SelectObject(hMemDC2, hBrush);

        Ellipse(hMemDC2, 250, 100, 750, 500);


        SelectObject(hMemDC2, oldBrush);
        DeleteObject(hBrush);

        TransparentBlt(hMemDC, 0, 0, bx, by,
            hMemDC2, 0, 0, bx, by,
            RGB(255, 0, 255));
        SelectObject(hMemDC2, hOldBitmap2);

        DeleteDC(hMemDC2);
    }




    //>> hdc�� �׷��ֱ�
    //BitBlt(


    //hdc�� �׷��ֱ�
    //BitBlt(hdc, 0, 0, rectView.right, rectView.bottom, hMemDC, 0, 0, SRCCOPY);

    TransparentBlt(hdc, 0, 0, rectView.right, rectView.bottom, hMemDC, 0, 0, rectView.right, rectView.bottom, RGB(255, 0, 255));



    SelectObject(hMemDC, hOldBitmap);
    DeleteDC(hMemDC);

}
#define IDC_CHILD1_BTN 2000

LRESULT CALLBACK ChildWndProc1(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HWND hBtn;
    static bool bToggle = false;

    switch (message)
    {
    case WM_CREATE:
        SetTimer(hWnd, TIMER_2, 30, AniProc);
        hBtn = CreateWindow(_T("button"), _T("OK"),
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            200, 10, 100, 30, hWnd, (HMENU)IDC_CHILD1_BTN,
            hInst, NULL);

        break;


    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_CHILD1_BTN:
            bToggle = !bToggle;
            break;
        }
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;


        HDC hdc = BeginPaint(hWnd, &ps);

        DrawBitmapDoubleBuffering(hWnd, hdc);

        if (bToggle)
        {
            TextOut(hdc, 200, 80, _T("Button Clicked"), 14);
        }


        //���� ���۸�
        DrawRectText(hdc);
        //�ؽ�Ʈ ���� �ذ�




        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        break;
    }

    return DefWindowProc(hWnd,message,wParam,lParam);
}

LRESULT CALLBACK ChildWndProc2(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static POINT ptMouse;
    switch (message)
    {
    case WM_CREATE:
        break;
    case WM_COMMAND:
        break;
    case WM_MOUSEMOVE:
        GetCursorPos(&ptMouse);
        InvalidateRect(hWnd, NULL, false);
        break;
    case WM_PAINT:
    {

        PAINTSTRUCT ps;


        HDC hdc = BeginPaint(hWnd, &ps);

        TCHAR str[128];
        wsprintf(str, TEXT("WORLD POSITION: (%04d, %04d)"), ptMouse.x, ptMouse.y);
        TextOut(hdc, 10, 30, str, lstrlen(str));


        ScreenToClient(hWnd, &ptMouse);
        wsprintf(str, TEXT("LOCAL POSITION: (%04d, %04d)"), ptMouse.x, ptMouse.y);
        TextOut(hdc, 10, 50, str, lstrlen(str));







        EndPaint(hWnd, &ps);
    }
        break;
    case WM_DESTROY:
        break;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}
