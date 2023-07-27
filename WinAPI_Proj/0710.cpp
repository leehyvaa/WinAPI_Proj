
#define  _CRT_SECURE_NO_WARNINGS
#include "framework.h"
#include "WinAPI_Proj.h"
#include "CObject.h"
#include <math.h>
#include <vector>
#include <commdlg.h>
#include <stdio.h>

// >> GDI+
#include <objidl.h>
#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")
using namespace Gdiplus;

ULONG_PTR g_GdiToken;

void Gdi_Init();
void Gdi_Draw(HDC hdc);
void Gdi_End();
//시작 그리기 끝 함수 제작

// << :


#define MAX_LOADSTRING 100
#define PI 3.141592

#define degreeToRadian(degree)((degree)*PI/180)

// >> :
#pragma comment (lib,"msimg32.lib")

HBITMAP hbackImage;
BITMAP bitBack;

HBITMAP hFrontImage;
BITMAP bitFront;

HBITMAP hTransparentImage;
BITMAP bitTransparent;

void CreateBitmap();
void DrawBitmap(HWND hWnd, HDC hdc);
void DeleteBitmap();

// << :
using namespace std;

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
    LoadStringW(hInstance, IDC_WINAPPPROJ, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    Gdi_Init();
    //위치 선정을 잘하자 안그러면 꼬임

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance(hInstance, nCmdShow))
    {
        Gdi_End();
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINAPPPROJ));

    MSG msg;

    // 기본 메시지 루프입니다:
    /*
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    */


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
            //업데이트나 루틴
        }
    }

    Gdi_End();

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
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINAPPPROJ));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); //숫자를 바꾸면 색이 변한다
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINAPPPROJ);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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

const int circleRadius = 40;//원의 크기 랜덤으로 재정의 해야함
bool bFlag = false;



BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) //여기서 윈도우 창을 설정 변경을 해주면 된다
{
    hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, // WS_OVERLAPPEDWINDOW | WS_VSCROLL 을 추가면 스크롤이 생김
        200, 300, 1024, 768, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow
    (hWnd);

    return TRUE;
}

#define TIMER_1 1
#define TIMER_2 2 //Ani 

void OutFromFile(TCHAR filename[], HWND hWnd)
{
    FILE* fptr;
    HDC hdc;
    int line;
    TCHAR buffer[500];
    line = 0;
    hdc = GetDC(hWnd);
#ifdef _UNICODE
    _tfopen_s(&fptr, filename, _T("r,ccs =UNICODE"));
#else
    _tfopem_s(&fptr, filename, _T("r"));

#endif // _UNICODE
    while (_fgetts(buffer, 100, fptr) != NULL)
    {
        if (buffer[_tcslen(buffer) - 1] == _T('\n'))
            buffer[_tcslen(buffer) - 1] = NULL;
        TextOut(hdc, 0, line * 20, buffer, _tcslen(buffer));
        line++;
    }
    fclose(fptr);
    ReleaseDC(hWnd, hdc);

}


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
const int SPIRTE_SIZE_X = 57;
const int SPIRTE_SIZE_Y = 52;

int RUN_FRAME_MAX = 0;
int RUN_FRAME_MIN = 0;
int curframe = RUN_FRAME_MIN;

int SPIRTE_FRAME_COUNT_X = 0;
int SPIRTE_FRAME_COUNT_Y = 0;

RECT rectView;

void DrawRectText(HDC hdc);
void UpdateFrame(HWND hWnd);
void DrawCircle(HDC hdc, POINT pt, BOOL bFlag);
void CenterRectangle(HDC hdc, POINT center, long double width, long double height);

VOID CALLBACK AniProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);

void Update();
void Render();

// << : ANI

HBITMAP hDoubleBufferImage;
void DrawBitmapDoubleBuffering(HWND hWnd, HDC hdc);
//double

BOOL CALLBACK Dialog_Test1_Proc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);




LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    /*

        객체지향 방식

        0. 메세지가 들어오기 전에는 계속 디폴트 값으로 보냄 아니면 switch케이스 문을 탄다

        1. 메뉴를 하나 선택
        2. command 이벤트를 받아사용한다.
        3. 그리고 이벤트 값을 저장하고
        4. 마우스 이벤트에서 위치를 받아 in함수를 받아
        5. paint로 넘어간다 그리고 스위치에서 값을 받아서 실행이 된다.

    */

    /*
        1. 도형 선택
        2. 복사 메뉴 활성
            좌클릭 업 이벤트에서 복사 메뉴 활성
        3. 복사 메뉴 선택
        4. 붙여 넣기 메뉴 활성, 복사 메뉴 비활성
        5. 붙여 넣기 메뉴 선택, 붙여 넣기 메뉴 비활성
        6. 선택된 도형 복사해서 좌상단에 그리기

    */
    PAINTSTRUCT ps;
    HDC hdc;
    POINT point{ 300,300 };
    POINT point1{ 100,100 };
    static POINT ptCurPos;
    static POINT ptMouse;
    static POINT ptMousePos;
    static vector <CObject*> c;
    static enum { Circle, Rectangle, Star, NONE };
    static TCHAR str[100];
    static int SeletedMenu = 4;
    static bool drawch = false;
    static bool fileopen = false;
    static int count = 0;

    static HMENU hMenu, hSubMenu;

    static bool bFlag = false;

    switch (message)
    {
    case WM_SIZE:
        GetClientRect(hWnd, &rectView);

        break;

    case WM_CREATE:
    {
        ptCurPos.x = circleRadius;
        ptCurPos.y = circleRadius;

        GetClientRect(hWnd, &rectView);

        hMenu = GetMenu(hWnd);
        hSubMenu = GetSubMenu(hMenu, 2);
        EnableMenuItem(hSubMenu, ID_EDITCOPY, MF_GRAYED);
        EnableMenuItem(hSubMenu, ID_EDITPASTE, MF_GRAYED);

        CreateBitmap();

        SetTimer(hWnd, TIMER_2, 30, AniProc);

    }
    break;

    case WM_TIMER:
    {
        if (wParam == TIMER_1)
        {
            UpdateFrame(hWnd);
            InvalidateRect(hWnd, NULL, TRUE);
        }
    }
        break;

    case WM_KEYDOWN://키보드 누르면
    {
        int breakpoint = 999;

    }
    break;

    case WM_KEYUP:
    {
        int breakpoint = 999;
        UINT state = GetMenuState(hSubMenu, ID_EDITCOPY, MF_BYCOMMAND);
        if (wParam == 'C' || wParam == 'c')
            //C를 누르면 변환
        {
            if ((state == MF_DISABLED) || (state == MF_GRAYED))
            {
                EnableMenuItem(hSubMenu, ID_EDITCOPY, MF_ENABLED);
                //버튼이 클릭이 안되지만 c를 누르면 복사 가능
                EnableMenuItem(hSubMenu, ID_EDITPASTE, MF_ENABLED);

            }
            else if (state == MF_ENABLED)
            {
                EnableMenuItem(hSubMenu, ID_EDITCOPY, MF_GRAYED);
            }
        }
        if (wParam == 'V' || wParam == 'v')
        {
            bFlag = true;
            if (state == MF_ENABLED)
            {
                EnableMenuItem(hSubMenu, ID_EDITPASTE, MF_GRAYED);

            }
            if (bFlag == true)
            {
                InvalidateRect(hWnd, NULL, TRUE);
            }
        }

        InvalidateRect(hWnd, NULL, TRUE);
    }
    break;

    case WM_CHAR:
    {

        int breakpoint = 999; // 문자열 받아서 처리
        if (wParam == VK_BACK && count > 0)
        {
            count--;
            //backspace처리
        }
        else if (wParam == VK_RETURN)
        {
            HDC memDC = GetWindowDC(hWnd);



            ReleaseDC(hWnd, memDC);
        }
        else
        {
            str[count++] = wParam;
        }
        str[count] = NULL;
        InvalidateRect(hWnd, NULL, TRUE);
    }

    case WM_COMMAND:
    {

        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        case ID_DRAW_CIRCLE:
            //IDE 값은 resource 헤더 파일에 있음
        {
            int temp = SeletedMenu;
            int ans = MessageBox(hWnd, _T("원 그릴래?"), _T("도형 그릴래?"), MB_YESNOCANCEL);
            if (ans == IDYES)
            {
                SeletedMenu = Circle;
            }
            else if (ans == IDNO)
            {
                SeletedMenu = NONE;
            }
            else
            {
                SeletedMenu = temp;
            }
        }
        break;
        case ID_DRAW_RECTANGLE:
            SeletedMenu = Rectangle;

            DialogBoxW(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, Dialog_Test1_Proc);
            //속성에 아무것도 안건드림
            //기능 추가 보기 -> 도구상자 -> 1번째 도구 상자 편집기

            break;
        case ID_DRAW_STAR:
            SeletedMenu = Star;
            break;

        case ID_FILEOPEN:
        {
            TCHAR filter[] = _T("Every file(*.*) \0*.*\0Text file\0*.txt;*.doc\0");
            // *필터 역할
            TCHAR lpstrFile[100] = _T("");

            OPENFILENAME ofn;
            memset(&ofn, 0, sizeof(OPENFILENAME));
            ofn.lStructSize = sizeof(OPENFILENAME);
            ofn.hwndOwner = hWnd;
            ofn.lpstrFilter = filter;
            ofn.lpstrFile = lpstrFile;
            ofn.nMaxFile = 100;
            ofn.lpstrInitialDir = _T(".");
            if (GetOpenFileName(&ofn) != 0)
            {
                TCHAR str[100];
                _stprintf(str, _T("%s 파일을 열겠습니까?"), ofn.lpstrFile);
                MessageBox(hWnd, str, _T("파일 선택"), MB_OK);
                fileopen = true;
                OutFromFile(ofn.lpstrFile, hWnd);

            }

        }
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
    case WM_LBUTTONDOWN:// 마우스 누르기 
    {
        ptMouse.x = LOWORD(lParam);
        ptMouse.y = HIWORD(lParam);
        drawch = true;
        if (InCircle(ptMousePos, ptCurPos)) bFlag = TRUE;

        InvalidateRect(hWnd, NULL, TRUE);
    }
    break;
    case WM_LBUTTONUP:
    {
        UINT state = GetMenuState(hSubMenu, ID_EDITCOPY, MF_BYCOMMAND);
        if ((state == MF_ENABLED))
        {
            EnableMenuItem(hSubMenu, ID_EDITCOPY, MF_GRAYED);
        }
    }
    break;

    case WM_MOUSEMOVE:
    {

    }
    break;
    case WM_PAINT:
    {
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
        hdc = BeginPaint(hWnd, &ps);
        //DrawBitmap(hWnd, hdc); 
        // 백그라운드는 한번만 실행 해야함
        DrawBitmapDoubleBuffering(hWnd, hdc);
        // 더블 버퍼링
        //DrawRectText(hdc);
        // 텍스트 문제 해결


        if (bFlag == true)
        {
            DrawCircle(hdc, point1, bFlag);
        }

        if (drawch == true)
        {
            switch (SeletedMenu)
            {
            case Circle:
                DrawCircle(hdc, ptMouse, bFlag);

                break;
            case Rectangle:
                CenterRectangle(hdc, ptMouse, 100, 100);
                break;
            case Star:
            {
                static TCHAR str[100] = L"별이 없다";
                TextOut(hdc, ptMouse.x, ptMouse.y, str, _tcslen(str));
            }
            break;
            default:
            {

                /*static TCHAR str[100] =  L"별일 없다";
                TextOut(hdc, 100, 100, str, _tcslen(str));*/

                break;
            }
            }
        }
        EndPaint(hWnd, &ps);
    }
    break;

    case WM_DESTROY:
    {
        KillTimer(hWnd, TIMER_2);
        DeleteBitmap();
        PostQuitMessage(0);
    }
    break;

    default:

        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
    }

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

/*

    bitBlt()는 그리기 원하는 영역
    창크기
    위아래좌표
    좌우시작좌표

*/


//template<class T>
//void DrawGrid(HDC hdc, POINT center, T width, T height, int count)
//{
//    POINT start = { center.x - width / 2 , center.y - height / 2 };
//    POINT end = { center.x + width / 2 , center.y + height / 2 };
//
//
//    T xInterval = width / count;
//    T yInterval = height / count;
//
//    for (int i = 0; i < count + 1; i++)
//    {
//        MoveToEx(hdc, start.x + xInterval * i, start.y, NULL);
//        LineTo(hdc, start.x + xInterval * i, end.y);
//    }
//
//    for (int i = 0; i < count + 1; i++)
//    {
//        MoveToEx(hdc, start.x, start.y + yInterval * i, NULL);
//        LineTo(hdc, end.x, start.y + yInterval * i);
//    }
//
//
//}
//

void DrawCircle(HDC hdc, POINT pt, BOOL bFlag)
{
    if (bFlag)
        SelectObject(hdc, GetStockObject(LTGRAY_BRUSH));
    Ellipse(hdc, pt.x - circleRadius, pt.y - circleRadius, pt.x + circleRadius, pt.y + circleRadius);
}

void CenterRectangle(HDC hdc, POINT center, long double width, long double height)
{
    POINT start = { center.x - width / 2 , center.y - height / 2 };
    POINT end = { center.x + width / 2 , center.y + height / 2 };

    Rectangle(hdc, start.x, start.y, end.x, end.y);
}

void CreateBitmap()
{
    //수지
    {
        hbackImage = (HBITMAP)LoadImage(NULL, TEXT("image/수지.bmp"),
            IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

        if (hbackImage == NULL)
        {
            DWORD dwError = GetLastError();
            MessageBox(NULL, _T("이미지 로드 에러"), _T("에러"), MB_OK);
            return;
        }

        GetObject(hbackImage, sizeof(BITMAP), &bitBack);
    }

    //시공
    {
        hTransparentImage = (HBITMAP)LoadImage(NULL, TEXT("image/sigong.bmp"),
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
        hAniImage = (HBITMAP)LoadImage(NULL, TEXT("image/zero_run.bmp"),
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
        hFrontImage = (HBITMAP)LoadImage(NULL, TEXT("image/Background.bmp"),
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
    RUN_FRAME_MAX = bitAni.bmWidth / SPIRTE_SIZE_X - 1;
    RUN_FRAME_MIN = 2;
    curframe = RUN_FRAME_MIN;
    SPIRTE_FRAME_COUNT_X = bitAni.bmWidth / SPIRTE_SIZE_X;
    SPIRTE_FRAME_COUNT_Y = bitAni.bmHeight / SPIRTE_SIZE_Y;

}

void DrawBitmap(HWND hWnd, HDC hdc)
{
    HDC hMemDC;
    HBITMAP hOldBitmap;
    int bx, by;
    {
        hMemDC = CreateCompatibleDC(hdc);
        hOldBitmap = (HBITMAP)SelectObject(hMemDC, hbackImage);
        //안해도됨
        bx = bitBack.bmWidth;
        by = bitBack.bmHeight;
        //그림의 크기

        BitBlt(hdc, 0, 0, bx, by, hMemDC, 0, 0, SRCCOPY);
        SelectObject(hMemDC, hOldBitmap);
        DeleteDC(hMemDC);
    }

    {
        hMemDC = CreateCompatibleDC(hdc);
        hOldBitmap = (HBITMAP)SelectObject(hMemDC, hTransparentImage);
        //안해도됨

        bx = bitTransparent.bmWidth;
        by = bitTransparent.bmHeight;
        //그림의 크기

        //BitBlt(hdc, 100, 100, bx, by, hMemDC, 0, 0, SRCCOPY);
        //그냥 이미지 출력

        TransparentBlt(hdc, 150, 150, bx * 2, by * 2, hMemDC, 0, 0, bx, by, RGB(255, 0, 255));
        //핑크색 빼기
        //SelectObject(hMemDC, hOldBitmap);

        DeleteDC(hMemDC);
    }


    //ani
    {
        hMemDC = CreateCompatibleDC(hdc);
        hOldBitmap = (HBITMAP)SelectObject(hMemDC, hAniImage);
        //안해도됨

        bx = bitAni.bmWidth / SPIRTE_FRAME_COUNT_X;
        by = bitAni.bmHeight / SPIRTE_FRAME_COUNT_Y;
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
    DeleteObject(hbackImage);
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
    //전에 사용한 WM ketdown은 한박자 느림
    if (GetKeyState('A') & 0x8000)//눌린 상태 0001은 눌렀다 땜거 찾아보기
    {

    }
    if (GetKeyState('D') & 0x8000)
    {

    }
    if (GetAsyncKeyState('W') & 0x8000)//둘이 다른 이유를 찾아봐라
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
        hOldBitmap2 = (HBITMAP)SelectObject(hMemDC2, hbackImage);
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

        bx = bitAni.bmWidth / SPIRTE_FRAME_COUNT_X;
        by = bitAni.bmHeight / SPIRTE_FRAME_COUNT_Y;
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
        hMemDC2 = CreateCompatibleDC(hMemDC);
        hOldBitmap2 = (HBITMAP)SelectObject(hMemDC2, hFrontImage);
        //안해도됨
        bx = bitFront.bmWidth;
        by = bitFront.bmHeight;
        //그림의 크기

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

    Gdi_Draw(hMemDC);
    //메모리 dc에 그려주기

    //hdc에 그려주기
    //BitBlt(hdc, 0, 0, rectView.right, rectView.bottom, hMemDC, 0, 0, SRCCOPY);

    TransparentBlt(hdc, 0, 0, rectView.right, rectView.bottom, hMemDC, 0, 0, rectView.right, rectView.bottom, RGB(255, 0, 255));

    //여기에서 Gdi를 추가를 하면 hdc를 넣어야함

    SelectObject(hMemDC, hOldBitmap);
    DeleteDC(hMemDC);

}
BOOL CALLBACK Dialog_Test1_Proc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    static int Check[3], Radio;
    TCHAR hobby[][30] = { _T("게임"),_T("개발해야지"),_T("집") };
    TCHAR sex[][30] = { _T("남"),_T("여") };
    TCHAR output[200];

    switch (iMsg)
    {
    case WM_INITDIALOG:
    {
        HWND hBtn = GetDlgItem(hDlg, IDC_PUSE);
        EnableWindow(hBtn, false);

        CheckRadioButton(hDlg, IDC_RADIO_M, IDC_RADIO_W, IDC_RADIO_M);
        //남, 여, 디폴트로 남자나 여자로 설정 해줘야함


    }
    return 1;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_GAME:
            Check[0] = 1 - Check[0];
            //체크가 되면 1이 됨
            break;
        case IDC_DE:
            Check[1] = 1 - Check[1];
            break;
        case IDC_HOME:
            Check[2] = 1 - Check[2];
            break;
        case IDC_RADIO_M:
            Radio = 0;
            //누가 선택이 되었는지 확인
            break;
        case IDC_RADIO_W:
            Radio = 1;
            break;
        case IDC_BUTTON_OUTPUT:
            _stprintf_s(output, _T("선택한 취미는 %s %s %s 입니다\r\n")
                _T("선택한 성별은 %s 입니다."),
                Check[0] ? hobby[0] : _T(""),
                Check[1] ? hobby[1] : _T(""),
                Check[2] ? hobby[2] : _T(""),
                sex[Radio]);
            //조심히 사용하자
            SetDlgItemText(hDlg, IDC_EDIT_OUTPUT, output);

        case IDC_BUTTON_COPY:
        {
            //리소스 뷰에서 암호를 true로 바꿔 주면 비번 입력하듯이 바뀜
            //리소스 뷰에서 컨 + d 누르면 탭키 순서 정할수 있음
            TCHAR str[100];
            GetDlgItemText(hDlg, IDC_EDIT_INPUT, str, 100);
            //입력받기 
            SetDlgItemText(hDlg, IDC_EDIT_COPY, str);
            //위에거 그대로 출력
        }
        break;
        case IDC_BUTTON_CLEAR:
        {
            SetDlgItemText(hDlg, IDC_EDIT_INPUT, _T(""));
            //입력받기
            SetDlgItemText(hDlg, IDC_EDIT_COPY, _T(""));
        }
        break;

        case IDC_START:
        {
            HDC hdc = GetDC(hDlg);
            SetDlgItemText(hDlg, IDC_TEXT, _T("Start"));
            ReleaseDC(hDlg, hdc);

            HWND hBtn = GetDlgItem(hDlg, IDC_START);
            EnableWindow(hBtn, false);

            hBtn = GetDlgItem(hDlg, IDC_PUSE);
            EnableWindow(hBtn, true);
        }
        break;
        case IDC_PUSE:
        {
            HDC hdc = GetDC(hDlg);
            SetDlgItemText(hDlg, IDC_TEXT, _T("Pause"));
            ReleaseDC(hDlg, hdc);

            HWND hBtn = GetDlgItem(hDlg, IDC_START);
            EnableWindow(hBtn, true);

            hBtn = GetDlgItem(hDlg, IDC_PUSE);
            EnableWindow(hBtn, false);

        }
        break;

        case IDC_BUTTON_PRINT:
        {
            HDC hdc = GetDC(hDlg);
            TextOut(hdc, 0, 0, _T("Print"), 5);
            SetDlgItemText(hDlg, IDC_TEXT, _T("Print"));

            ReleaseDC(hDlg, hdc);
        }
        break;
        case IDOK:
            EndDialog(hDlg, 0);
            break;
        case ID_EXIT:
            EndDialog(hDlg, 0);
            break;
        }
        break;
    }
    return 0;
}
/*

게임 타이틀 출력 -> Press Any Key
추가요소: 아이디 입력

본 게임 진행

게임 결과 출력 -> 점수, 랭킹
추가요소: 파일 입/출력


1. 벽돌과 공 생성

2. 벽돌과 충돌 되면 입사각 반사각을 구하여 반대로 공 튀기기

3. rectView.bottom에 공이 닿으면 게임 끝

4. 깬 벽돌 점수 카운트
﻿

*/

void Gdi_Init()
{
    GdiplusStartupInput gpsi;
    GdiplusStartup(&g_GdiToken, &gpsi, NULL);
    // 토큰값 넣어주기 확인 하겠다
}

void Gdi_Draw(HDC hdc)
{
    //이거를 사용할려면 미리 로드를 하고 사용해야함
    //너무 많이 사용하면 느리다는 단점이 있다
    //장점은 이미지 출력이 깔끔하다
    Graphics graphics(hdc);

    // :text
    SolidBrush brush(Color(255, 255, 0, 0));
    //반투명 설정(255 -> 100,255,0,0);
    FontFamily fontFamily(L"Times New Roman");
    Font font(&fontFamily, 24, FontStyleRegular, UnitPixel);
    PointF pointF(10.0f, 20.0f);
    graphics.DrawString(L"Hello GDI+!!", -1, &font, pointF, &brush);

    //:line
    Pen pen(Color(255, 255, 0, 0));
    graphics.DrawLine(&pen, 0, 0, 200, 100);

    //: image
    Image img((WCHAR*)L"image/sigong.png");
    int w = img.GetWidth();
    int h = img.GetHeight();
    graphics.DrawImage(&img, 300, 100, w, h);

    //: ani
    Image img2((WCHAR*)L"image/zero_run.png");
    w = img2.GetWidth() / SPIRTE_FRAME_COUNT_X;
    h = img2.GetHeight() / SPIRTE_FRAME_COUNT_Y;
    int xStart = curframe * w;
    int yStart = 0;

    ImageAttributes imgAttr0;
    imgAttr0.SetColorKey(Color(245, 0, 245), Color(255, 10, 255));
    graphics.DrawImage(&img2, Rect(400, 100, w, h), xStart, yStart, w, h, UnitPixel, &imgAttr0);

    // >> alpha rect
    brush.SetColor(Color(128, 255, 0, 0));
    graphics.FillRectangle(&brush, 100, 100, 200, 300);

    // >> : rotation
    Image* pImg = nullptr;
    pImg = Image::FromFile((WCHAR*)L"image/sigong.png");
    int xPos = 400;
    int yPos = 200;
    if (pImg)
    {
        w = pImg->GetWidth();
        h = pImg->GetHeight();

        Gdiplus::Matrix mat;
        static int rot = 0;
        mat.RotateAt((rot % 360), Gdiplus::PointF(xPos + (float)(w / 2), yPos + (float)(h / 2)));
        //돌리는 위치 
        graphics.SetTransform(&mat);
        graphics.DrawImage(pImg, xPos, yPos, w, h);
        rot += 10;
        //이미지가 좌 상단에 잡혀있어서 시계방향으로 돌고 있음

        mat.Reset();
        graphics.SetTransform(&mat);
        //이 그림에만 돌리는 reset
    }

    ImageAttributes imgAttr;
    imgAttr.SetColorKey(Color(240, 0, 240), Color(255, 10, 255));
    xPos = 500;
    graphics.DrawImage(pImg, Rect(xPos, yPos, w, h), 0, 0, w, h, UnitPixel, &imgAttr);

    if (pImg)
    {
        REAL transparency = 0.5f;
        ColorMatrix colorMatrix =
        {
            1.0f, 0.0f, 0.0f, 0.0f,0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,0.0f,
            0.0f, 0.0f, 0.0f, transparency,0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,1.0f
        };
        imgAttr.SetColorMatrix(&colorMatrix);
        xPos = 600;
        graphics.DrawImage(pImg, Rect(xPos, yPos, w, h), 0, 0, w, h, UnitPixel, &imgAttr);
        //반 투명

        ColorMatrix grayMatrix =
        {
            0.3f, 0.3f, 0.3f, 0.0f,0.0f,
            0.6f, 0.6f, 0.6f, 0.0f,0.0f,
            0.1f, 0.1f, 0.1f, 0.0f,0.0f,
            0.0f, 0.0f, 0.0f, 1.0f,0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,1.0f
        };
        imgAttr.SetColorMatrix(&grayMatrix);
        xPos = 700;
        graphics.DrawImage(pImg, Rect(xPos, yPos, w, h), 0, 0, w, h, UnitPixel, &imgAttr);
        //회색 이미지

        xPos = 800;
        pImg->RotateFlip(RotateNoneFlipX);
        graphics.DrawImage(pImg, Rect(xPos, yPos, w, h), 0, 0, w, h, UnitPixel, &imgAttr);
        //뒤집기

        delete pImg;
    }
}

void Gdi_End()
{
    GdiplusShutdown(g_GdiToken);
}
