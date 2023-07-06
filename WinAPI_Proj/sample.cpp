
#define  _CRT_SECURE_NO_WARNINGS
#include "framework.h"
#include "CObject.h"
#include <math.h>
#include <vector>
#include <commdlg.h>
#include <stdio.h>

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
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); //숫자를 바꾸면 색이 변한다
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINAPIPROJ);
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

// << : ANI

HBITMAP hDoubleBufferImage;
void DrawBitmapDoubleBuffering(HWND hWnd, HDC hdc);
//double


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
        DrawRectText(hdc);
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
        hbackImage = (HBITMAP)LoadImage(NULL, TEXT("images/수지.bmp"),
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


    //hdc에 그려주기
    //BitBlt(hdc, 0, 0, rectView.right, rectView.bottom, hMemDC, 0, 0, SRCCOPY);

    TransparentBlt(hdc, 0, 0, rectView.right, rectView.bottom, hMemDC, 0, 0, rectView.right, rectView.bottom, RGB(255, 0, 255));



    SelectObject(hMemDC, hOldBitmap);
    DeleteDC(hMemDC);

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