// WinAPI_Proj.cpp : 애플리케이션에 대한 진입점을 정의합니다.
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


//스플릿 윈도우
HWND ChildWnd[2];
LRESULT CALLBACK ChildWndProc1(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ChildWndProc2(HWND, UINT, WPARAM, LPARAM);


// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
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

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINAPIPROJ, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINAPIPROJ));

    MSG msg;


    // 기본 메시지 루프입니다:
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
            //일정 프레임마다 정확하게 작동하도록 코드를 수정해야함
            // update는 일정 시간마다 작동하고,Render는 매번 작동해도 되고
            // 일정 시간마다 작동해도 된다.
            //Update(); 타이머가 하는 역할을 여기로
            // POINT mousePos; //전역변수로 선언
            // GetCurSorPos(&mousePos) 이러면 mousePos로 마우스 위치값이 들어감
            // 
            //Render(); Paint가 하는 역할을 이쪽으로
        }
  
    }
    return (int)msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
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
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

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
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
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
        //얘는 생성자처럼 윈도우 생성시 한 번 실행됨
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


        //윈도우 분할
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

        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        case ID_DRAW_CIRCLE: //원 그리기
        {
            int temp = selectedMenu;

            int ans = MessageBox(hWnd, _T("원 그릴래?"), _T("도형 선택"), MB_YESNOCANCEL);
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
            //선택된 그림 저장
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
    //    //TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
    //    //DrawBitmap(hWnd, hdc);
    //    //백그라운드는 한번만 실행 해야함
    //    DrawBitmapDoubleBuffering(hWnd, hdc);
    //    //더블 버퍼링
    //    DrawRectText(hdc);
    //    //텍스트 문제 해결



    //    //DrawCircle(hdc, ptCurPos, bFlag);


    //    //for (int  i = 0; i < pCount; i++)
    //    //{
    //    //    //벡터 출력
    //    //}

    //    //if (dFlag)
    //    //{
    //    //    DrawCircle(hdc, ptMousePos, bFlag);
    //    //    dFlag = false;
    //    //    //벡터에다 넣음
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

// 정보 대화 상자의 메시지 처리기입니다.
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






//스타트 엔드 격자

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
    //수지
    {
        hBackImage = (HBITMAP)LoadImage(NULL, TEXT("images/수지.bmp"),
            IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

        if (hBackImage == NULL)
        {
            DWORD dwError = GetLastError();
            MessageBox(NULL, _T("이미지 로드 에러"), _T("에러"), MB_OK);
            return;
        }

        GetObject(hBackImage, sizeof(BITMAP), &bitBack);
    }

    //시공
    {
        hTransparentImage = (HBITMAP)LoadImage(NULL, TEXT("images/sigong.bmp"),
            IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

        if (hTransparentImage == NULL)
        {
            DWORD dwError = GetLastError();
            MessageBox(NULL, _T("이미지 로드 에러 2"), _T("에러"), MB_OK);
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
            MessageBox(NULL, _T("이미지 로드 에러 3"), _T("에러"), MB_OK);
            return;
        }

        GetObject(hAniImage, sizeof(BITMAP), &bitAni);
    }

    // hFrontImage; //실제로 백
    {
        hFrontImage = (HBITMAP)LoadImage(NULL, TEXT("images/Background.bmp"),
            IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

        if (hFrontImage == NULL)
        {
            DWORD dwError = GetLastError();
            MessageBox(NULL, _T("이미지 로드 에러4"), _T("에러"), MB_OK);
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

    { //수지
        hMemDC = CreateCompatibleDC(hdc);
        hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBackImage);
        bx = bitBack.bmWidth;
        by = bitBack.bmHeight;

        BitBlt(hdc, 0, 0, bx, by, hMemDC, 0, 0, SRCCOPY);

        /*

            bitBlt()는 그리기 원하는 영역
            창크기
            위아래좌표
            좌우시작좌표

        */
        SelectObject(hMemDC, hOldBitmap);
        DeleteDC(hMemDC);

    }

    {//시공
        hMemDC = CreateCompatibleDC(hdc);
        hOldBitmap = (HBITMAP)SelectObject(hMemDC, hTransparentImage);
        bx = bitTransparent.bmWidth;
        by = bitTransparent.bmHeight;

        //BitBlt(hdc, 150, 150, bx, by, hMemDC, 0, 0, SRCCOPY);
        TransparentBlt(hdc, 150, 150, bx, by, hMemDC, 0, 0, bx, by, RGB(255, 0, 255));
        //RGB 자리가 컬러키값

       // SelectObject(hMemDC, hOldBitmap);
        DeleteDC(hMemDC);
    }

    //ani
    {
        hMemDC = CreateCompatibleDC(hdc);
        hOldBitmap = (HBITMAP)SelectObject(hMemDC, hAniImage);
        //안해도됨

        bx = bitAni.bmWidth / SPRITE_FRAME_COUNT_X;
        by = bitAni.bmHeight / SPRITE_FRAME_COUNT_Y;
        //그림의 크기

        //BitBlt(hdc, 100, 100, bx, by, hMemDC, 0, 0, SRCCOPY);
        //그냥 이미지 출력

        TransparentBlt(hdc, 150, 300, bx, by, hMemDC, 0, 0, bx, by, RGB(255, 0, 255));
        //핑크색 빼기

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
    TCHAR  strTest[] = _T("이미지 출력");
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
    //:wm_keydown , wm_keyup 은 반응이 즉각적이지 않음
    if (GetKeyState('A') & 0x8000)//눌린상태
    {

    }

    if (GetAsyncKeyState('W') & 0x8000)//위와 약간 차이가 있으니 확인하고 사용
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
        //안해도됨
        bx = bitBack.bmWidth;
        by = bitBack.bmHeight;
        //그림의 크기

        BitBlt(hMemDC, 0, 0, bx, by, hMemDC2, 0, 0, SRCCOPY);
        SelectObject(hMemDC2, hOldBitmap2);
        DeleteDC(hMemDC2);
    }

    {
        hMemDC2 = CreateCompatibleDC(hMemDC);
        hOldBitmap2 = (HBITMAP)SelectObject(hMemDC2, hTransparentImage);
        //안해도됨

        bx = bitTransparent.bmWidth;
        by = bitTransparent.bmHeight;
        //그림의 크기

        //BitBlt(hdc, 100, 100, bx, by, hMemDC, 0, 0, SRCCOPY);
        //그냥 이미지 출력

        TransparentBlt(hMemDC, 150, 150, bx * 2, by * 2, hMemDC2, 0, 0, bx, by, RGB(255, 0, 255));
        //핑크색 빼기

        SelectObject(hMemDC2, hOldBitmap2);
        DeleteDC(hMemDC2);
    }


    //ani
    {
        hMemDC2 = CreateCompatibleDC(hMemDC);
        hOldBitmap2 = (HBITMAP)SelectObject(hMemDC2, hAniImage);
        //안해도됨

        bx = bitAni.bmWidth / SPRITE_FRAME_COUNT_X;
        by = bitAni.bmHeight / SPRITE_FRAME_COUNT_Y;
        //그림의 크기

        //BitBlt(hdc, 100, 100, bx, by, hMemDC, 0, 0, SRCCOPY);
        //그냥 이미지 출력

        int xStart = curframe * bx;
        int yStart = 0;

        static int xPos = 0;
        TransparentBlt(hMemDC, 150 + xPos, 300, bx, by, hMemDC2, xStart, yStart, bx, by, RGB(255, 0, 255));
        //핑크색 빼기

        //StretchBlt(hMemDC , 150 + xPos, 300, -bx, by, hMemDC2, xStart, yStart, bx, by, SRCCOPY);
        //컬러 키를 빼지 못함

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




    //>> hdc에 그려주기
    //BitBlt(


    //hdc에 그려주기
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


        //더블 버퍼링
        DrawRectText(hdc);
        //텍스트 문제 해결




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
