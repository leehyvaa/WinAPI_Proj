// WinAPI_Proj.cpp : ���ø����̼ǿ� ���� �������� �����մϴ�.
//

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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    static TCHAR lpstrFile[100] = _T("");
    static FILE* fPtr;

    static TCHAR msg[100];
    static TCHAR chat[10][100];

    enum {CIRCLE, RECTANGLE,STAR,NONE};

    static int count, yPos;
    static int strNum;
    static SIZE size;

    
    static POINT ptMousePos;
    static bool drawCheck = false;
    static int selectedMenu = NONE;
    

    switch (message)
    {
    case WM_CREATE:
        //��� ������ó�� ������ ������ �� �� �����
        count = 0;
        yPos = 15;
        strNum = 0;
        CreateCaret(hWnd, NULL, 5, 15);
        ShowCaret(hWnd);
        break;

    case WM_CHAR:
    {
        int breakpoint = 999;
        if (wParam == VK_BACK && count > 0) //�齺���̽��� �Էµ������
        {
            count--;
        }
        else if (wParam == VK_RETURN)
        {
            if (strNum > 9)
            {
                for (int i = 0; i < 10; i++)
                {
                    _tcscpy(chat[i], chat[1 + i]);
                }
                strNum--;
            }

            _tcscpy(chat[strNum], msg);
    

            _fputts(msg, fPtr);
            _fputts((L"\n"), fPtr);

            strNum++;
            count = 0;

        }
        else
        {
            msg[count++] = wParam;
        }
        msg[count] = NULL;
        InvalidateRect(hWnd, NULL, TRUE);
       
    } break;
       
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

    case WM_MOUSEMOVE:
    {
        ptMousePos.x = LOWORD(lParam);
        ptMousePos.y = HIWORD(lParam);

    }
    break;

    case WM_LBUTTONDOWN:
    {
        drawCheck = true;
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

        case ID_DRAW_RECTANGLE:
            selectedMenu = RECTANGLE;

            break;

        case ID_DRAW_STAR:
            selectedMenu = STAR;

            break;

        case ID_FILEOPEN:
        {
           
            TCHAR filter[] = _T("Every file(*.*) \0*.*\0Text file\0*.txt;*.doc\0");
            
            TCHAR buffer[500];
            
            TCHAR str[100];
            int line = 0;

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
                hdc = GetDC(hWnd);
                _stprintf(str, _T("%s ������ ���ڽ��ϱ�?"), ofn.lpstrFile);
                MessageBox(hWnd, lpstrFile, _T("���� ����"), MB_OK);
                _tfopen_s(&fPtr, lpstrFile, _T("w"));
                //_tfopen_s(&fPtr, lpstrFile, _T("r"));
                //while (_fgetts(buffer, 100, fPtr) != NULL)
                //{
                //    if (buffer[_tcslen(buffer) - 1] == _T('\n'))
                //        buffer[_tcslen(buffer) - 1] = NULL;
                //    TextOut(hdc, 0, line * 20, buffer, _tcslen(buffer));
                //    line++;
                //}
                //fclose(fPtr);
                ReleaseDC(hWnd, hdc);
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
    case WM_PAINT:
    {
        hdc = BeginPaint(hWnd, &ps);
         //TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�...

        GetTextExtentPoint(hdc, msg, _tcslen(msg), &size);
       
        for (int i = 0; i < 10; i++)
        {
            TextOut(hdc, 100, yPos*i, chat[i], _tcslen(chat[i]));
           
            
            
            //while (_fgetts(buffer, 100, fPtr) != NULL)
            //{
            //    if (buffer[_tcslen(buffer) - 1] == _T('\n'))
            //        buffer[_tcslen(buffer) - 1] = NULL;
            //    TextOut(hdc, 0, line * 20, buffer, _tcslen(buffer));
            //    line++;
            //}
            


        }
       
        TextOut(hdc, 100, 200 , msg, _tcslen(msg));

        SetCaretPos(100+ size.cx, 0 + 200);
        

        POINT center = { 300,300 };

        //DrawGrid(hdc,center,400,400,10,10);          
        //DrawCircle(hdc, center, 250);
        //SunFlower(hdc, center, 50, 5);

        HPEN hPen, oldPen;

        hPen = CreatePen(PS_DOT, 1, RGB(255, 0, 255));
        oldPen = (HPEN)SelectObject(hdc, hPen);
        //hPen�� �����ϰ� �� ���� �����ϴ� ���� oldPen�� ��â��

        HBRUSH hBrush, oldBrush;
        hBrush = CreateSolidBrush(RGB(0, 255, 255));
        //hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
        oldBrush = (HBRUSH)SelectObject(hdc, hBrush);




        SelectObject(hdc, oldPen);
        SelectObject(hdc, oldBrush);

        if (drawCheck)
        {
            switch (selectedMenu)
            {
            case CIRCLE:
                Ellipse(hdc, ptMousePos.x - 50, ptMousePos.y - 50, ptMousePos.x + 50, ptMousePos.y + 50);
                break;
            case RECTANGLE:
                Rectangle(hdc, ptMousePos.x - 50, ptMousePos.y - 50, ptMousePos.x + 50, ptMousePos.y+50);
                break;
            case STAR:
                break;
            case NONE:
                break;
            default:
                break;
            }
           drawCheck = false;
        }


        DeleteObject(hPen);
        DeleteObject(hBrush);




        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        fclose(fPtr);
        HideCaret(hWnd);
        DestroyCaret();
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