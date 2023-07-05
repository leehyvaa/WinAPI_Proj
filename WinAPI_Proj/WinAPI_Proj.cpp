// WinAPI_Proj.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "WinAPI_Proj.h"
#include <vector>
#include <math.h>
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
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINAPIPROJ));

    MSG msg;

    // 기본 메시지 루프입니다:
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
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINAPIPROJ));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINAPIPROJ);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
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
      200, 200,1024, 768, nullptr, nullptr, hInstance, nullptr);

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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;


    static TCHAR msg[100];
    static TCHAR chat[10][100];
    static int count, yPos;
    static int strNum;
    static SIZE size;
    switch (message)
    {
    case WM_CREATE:
        //얘는 생성자처럼 윈도우 생성시 한 번 실행됨
        count = 0;
        yPos = 15;
        strNum = 0;
        CreateCaret(hWnd, NULL, 5, 15);
        ShowCaret(hWnd);
        break;
    case WM_KEYDOWN:
    {
        int breakpoint = 999;
    }
        break;
    case WM_KEYUP:
    {
        int breakpoint = 999;
    }
        break;
    case WM_CHAR:
    {
        int breakpoint = 999;
        if (wParam == VK_BACK && count >0) //백스페이스가 입력됐을경우
        {
            count--;
        }
        else if (wParam == VK_RETURN)
        {
            if (strNum > 9)
            {
                for (int i = 0; i < 10; i++)
                {
                    _tcscpy(chat[i], chat[1+i]);
                }
                strNum--;
            }
            
			_tcscpy(chat[strNum], msg);
			strNum++;
			count = 0;  
          
        }
        else
        {
            msg[count++] = wParam;
        }
        msg[count] = NULL;
        InvalidateRect(hWnd, NULL, TRUE);
        //InvalidateRgn(hWnd,NULL,TRUE);
        
    }
        break;
        
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
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
    case WM_PAINT:
        {
            hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

            /*GetTextExtentPoint(hdc, msg, _tcslen(msg), &size);

            for (int i = 0; i < 10; i++)
            {
                TextOut(hdc, 100, yPos*i, chat[i], _tcslen(chat[i]));
            }

            TextOut(hdc, 100, 200 , msg, _tcslen(msg));

            SetCaretPos(100+ size.cx, 0 + 200);
            */

            POINT center = { 300,300 };

            //DrawGrid(hdc,center,400,400,10,10);          
            //DrawCircle(hdc, center, 250);
            //SunFlower(hdc, center, 50, 5);

            HPEN hPen, oldPen;

            hPen = CreatePen(PS_DOT, 1, RGB(255, 0, 255));
            oldPen = (HPEN)SelectObject(hdc, hPen);
            //hPen을 장착하고 그 전에 장착하던 펜은 oldPen에 저창함
            
            HBRUSH hBrush, oldBrush;
            hBrush = CreateSolidBrush(RGB(0, 255, 255));
            //hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
            oldBrush = (HBRUSH)SelectObject(hdc, hBrush);



            SunFlower(hdc, center, 50, 5);
            DrawCircle(hdc, center, 500);
            DrawCircle(hdc, center, 250);
            SelectObject(hdc, oldPen);
            SelectObject(hdc, oldBrush);


            DeleteObject(hPen);
            DeleteObject(hBrush);

            DrawGrid(hdc,center,400,400,10,10);     
          


            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        HideCaret(hWnd);
        DestroyCaret();
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

template<class T>
void DrawGrid(HDC hdc,POINT center,T width,T height,int xCount,int yCount)
{
    POINT start = { center.x - width / 2 , center.y - height/2};
    POINT end = { center.x + width / 2 , center.y + height / 2 };


    T xInterval = width / xCount;
    T yInterval = height / yCount;

    for (int i = 0; i < xCount+1; i++)
    {
        MoveToEx(hdc, start.x+xInterval*i, start.y, NULL);
        LineTo(hdc, start.x+xInterval*i, end.y);
    }

    for (int i = 0; i < yCount+1; i++)
    {
        MoveToEx(hdc, start.x , start.y + yInterval * i, NULL);
        LineTo(hdc, end.x, start.y + yInterval * i);
    }
   
}

template<class T>
void DrawCircle(HDC hdc, POINT center, T radius)
{
    Ellipse(hdc, center.x-radius, center.y-radius, center.x+radius, center.y+radius);

}

template<class T>
void SunFlower(HDC hdc, POINT center,T radius ,int count)
{
    //360/count = 원 하나가 가질 수 있는 각도
    //3.141592
    //1도 = 0.0174533 라디안
    //안쪽원 + 외곽원  
    //안족원 + 외곽원 두 직선 사이의 각도가 360/count


    
    double angle = degreeToRadian(360/count);
    double val = sin(angle);

    double r = (sin(angle/2) * radius) / (1 - sin(angle/2));

    DrawCircle(hdc, center, radius);

    for (int i = 1; i <= count; i++)
    {
        double t = (360 / count) * i;
        double tAngle = degreeToRadian(t);

        double tempX = cos(tAngle) * (radius + r);
        double tempY = sin(tAngle) * (radius + r);


        POINT temp = {center.x + tempX, center.y+tempY };
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