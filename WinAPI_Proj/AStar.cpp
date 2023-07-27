// WindowsProject11.cpp : ���ø����̼ǿ� ���� �������� �����մϴ�.
//

#include "framework.h"
#include "WinAPI_Proj.h"
#include "pch.h"
#include "PriorityQueue.h"
#include <stack>
#define MAX_LOADSTRING 100

HDC g_hdc;
enum MapType
{
    Default,
    Start,
    Dest,
    Wall,
    OpenList,
    CloseList,
    Rail,
};
struct MapNode
{
    MapType  mType;
    POINT pos;
};
int dirX[8] = { 0,1,0,-1,1,1,-1,-1 };
int dirY[8] = { -1,0,1,0,-1,1,1,-1 };
float cost[8] = { 1,1,1,1,1.4,1.4,1.4,1.4 };
stack<POINT> path;

MapNode arrMap[10][10];
vector<Node> openList;
vector<Node> closeList;
vector<Node> wallList;

void DrawMap(HDC hdc);
void InitMap();
void AStar(HDC hdc);
void AddOpenList(Node crnt);
void PlayerMove(HDC hdc);

double LengthPts(POINT pt1, POINT pt2);
HWND hWnd;



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

    RECT rect = { 0,0, 1000, 1000 };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, true);
    SetWindowPos(hWnd, nullptr, 300, 10, rect.right - rect.left, rect.bottom - rect.top, 0);



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
        200, 100, 1000,1000, nullptr, nullptr, hInstance, nullptr);

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

POINT ptMousePos;
const int radius = 50;
enum Sequence
{
    StartPoint,
    DestPoint,
    WallPoint,
    Play,
    End,
};

Sequence sequence;
#define TIMER_1 1

int RUN_FRAME_MAX = 0;
int RUN_FRAME_MIN = 0;
int curframe = RUN_FRAME_MIN;
RECT rectView;

void UpdateFrame(HWND hWnd);
VOID CALLBACK AniProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);
void Update();

POINT startPos;
POINT destPos;


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PriorityQueue PriQueue;
 
    PAINTSTRUCT ps;
    HDC hdc;



    switch (message)
    {
    case WM_CREATE:
    {
        POINT ptMousePos = { NULL };
        sequence = StartPoint;
        InitMap();
        SetTimer(hWnd, TIMER_1, 500, AniProc);
        GetClientRect(hWnd, &rectView);
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
    case WM_TIMER:
    {
        if (wParam == TIMER_1)
        {
            UpdateFrame(hWnd);
            InvalidateRect(hWnd, NULL, TRUE);
        }
    }
    break;
    case WM_LBUTTONDOWN:
    {
        ptMousePos.x = LOWORD(lParam);
        ptMousePos.y = HIWORD(lParam);

        for (int i = 0; i < 10; i++)
        {
            for (int j = 0; j < 10; j++)
            {
                if (ptMousePos.x > arrMap[j][i].pos.x - radius && ptMousePos.x < arrMap[j][i].pos.x + radius)
                    if (ptMousePos.y > arrMap[j][i].pos.y - radius && ptMousePos.y < arrMap[j][i].pos.y + radius)
                    {
                        switch (sequence)
                        {
                        case StartPoint:
                            arrMap[j][i].mType = Start;
                            sequence = DestPoint;
                            startPos = { i,j };
                            break;
                        case DestPoint:
                            arrMap[j][i].mType = Dest;
                            sequence = WallPoint;
                            destPos = { i,j };
                            break;
                        case WallPoint:
                            arrMap[j][i].mType = Wall;
                            break;
                        default:
                            break;
                        }
                        
                    }

            }
        }
        InvalidateRect(hWnd, NULL, TRUE);

    }break;
    case WM_KEYDOWN:
    {
        hdc = GetDC(hWnd);

        if (wParam == '1')
        {
            sequence = Play;
            AStar(hdc);
        }
        if (wParam == '2')
        {
            sequence = StartPoint;
            InitMap();
        }

        ReleaseDC(hWnd, hdc);
    }
    break;
    case WM_PAINT:
    {
        hdc =BeginPaint(hWnd, &ps);

        DrawMap(hdc);
        if (sequence == Play)
        {
            PlayerMove(hdc);
        }
        
            


        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        KillTimer(hWnd, TIMER_1);
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



void InitMap()
{
    POINT center;

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            center = { i * 100 + 50,j * 100 + 50 };

            arrMap[j][i].mType = Default;
            arrMap[j][i].pos = center;
        }
    }
    
    openList.clear();
    closeList.clear();


}




void DrawMap(HDC hdc)
{
    POINT center = { 500,500 };


    SelectObject(hdc,GetStockObject(DC_BRUSH));

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
           
            //�� Ÿ�� ���� ���� �ٲ㼭 �׸���
            if(arrMap[j][i].mType == Start)
                SetDCBrushColor(hdc, RGB(0, 0, 255));
            else if(arrMap[j][i].mType == Dest)
                SetDCBrushColor(hdc, RGB(255, 0, 0));
            else if(arrMap[j][i].mType == Wall)
                SetDCBrushColor(hdc, RGB(0, 0, 0));
            else if (arrMap[j][i].mType == OpenList)
                SetDCBrushColor(hdc, RGB(150, 150, 150));
            else if (arrMap[j][i].mType == CloseList)
                SetDCBrushColor(hdc, RGB(0, 100, 0));
            else if (arrMap[j][i].mType == Rail)
                SetDCBrushColor(hdc, RGB(0, 255, 0));
            else
                SetDCBrushColor(hdc, RGB(255, 255, 255));

           
           
            Rectangle(hdc, arrMap[j][i].pos.x - radius,
                arrMap[j][i].pos.y - radius,
                arrMap[j][i].pos.x + radius,
                arrMap[j][i].pos.y + radius);     

        }
    }


   

    for (int i = 0; i < openList.size(); i++)
    {
       

        center = { openList[i].pos.x * 100 + 50, openList[i].pos.y * 100 + 50 };
		TCHAR buf[20];
		_stprintf_s(buf, TEXT("%.1lf"), openList[i].g);
		TextOut(hdc, center.x-20, center.y - 20, buf, _tcslen(buf));

		_stprintf_s(buf, TEXT("%.1lf"), openList[i].h);
		TextOut(hdc, center.x + 20, center.y - 20, buf, _tcslen(buf));

		_stprintf_s(buf, TEXT("%.1lf"), openList[i].f);
		TextOut(hdc, center.x, center.y + 20, buf, _tcslen(buf));
    }


    for (int i = 0; i < closeList.size(); i++)
    {


        center = { closeList[i].pos.x * 100 + 50, closeList[i].pos.y * 100 + 50 };
        TCHAR buf[20];
        _stprintf_s(buf, TEXT("%.1lf"), closeList[i].g);
        TextOut(hdc, center.x - 20, center.y - 20, buf, _tcslen(buf));

        _stprintf_s(buf, TEXT("%.1lf"), closeList[i].h);
        TextOut(hdc, center.x + 20, center.y - 20, buf, _tcslen(buf));

        _stprintf_s(buf, TEXT("%.1lf"), closeList[i].f);
        TextOut(hdc, center.x, center.y + 20, buf, _tcslen(buf));
    }

   
}


void AddOpenList(Node crnt)
{

    //������ ���⿡ ���� ������ �ְ� ���¸���Ʈ�� ���
    for (int i = 0; i < 8; i++)
    {
        POINT next;
        next = { crnt.pos.x + dirX[i] ,crnt.pos.y + dirY[i] };

        if (arrMap[next.y][next.x].mType == Wall
            || arrMap[next.y][next.x].mType == CloseList)
            continue;

        if (next.x < 0 || next.x > 9 || next.y < 0 || next.y >9)
            continue;
        

        bool dataPush = true;

        Node n;
        n.pos = { crnt.pos.x + dirX[i],crnt.pos.y + dirY[i] };
        n.parent = crnt.pos;
        n.h = abs(destPos.x-n.pos.x) + abs(destPos.y-n.pos.y);
        n.g = crnt.g + cost[i];
      
        n.f = n.h + n.g;
        for (int j = 0; j < openList.size(); j++)
        {
            if (openList[j].pos.x == n.pos.x &&
                openList[j].pos.y == n.pos.y)
            {
                if (openList[j].g > n.g)
                {
                    openList[j].h = n.h;
                    openList[j].g = n.g;
                    openList[j].f = n.f;
                    openList[j].parent = n.parent;
                }
                dataPush = false;
            }
        }

        if (dataPush)
        {
            openList.push_back(n);
            arrMap[n.pos.y][n.pos.x].mType = OpenList;
        }
    }
}

void AStar(HDC hdc)
{
    Node crnt;
    crnt.pos = startPos;
    crnt.parent = crnt.pos;
    crnt.h = abs(destPos.x - crnt.pos.x) + abs(destPos.y - crnt.pos.y);
    crnt.g = 0;
    crnt.f = crnt.h + crnt.g;
    openList.push_back(crnt);
    //closeList.push_back(crnt);

    while (1)
    {
        
        AddOpenList(crnt);

 

        
        //�켱����ť�� ���¸���Ʈ�� ���� ���� ��� Ȯ��
        PriorityQueue p;
        for (int i = 0; i < openList.size(); i++)
        {
            p.Push(openList[i]);
        }

        Node next = p.Pop();

        //��η� crnt�� �ű�� ���¸���Ʈ���� ������ Ŭ�����Ʈ�� �߰�
        vector<Node>::iterator iter = openList.begin();
        for (iter = openList.begin(); iter != openList.end();)
        {
            if (crnt.pos.x == iter->pos.x &&
                crnt.pos.y == iter->pos.y)
            {
                iter = openList.erase(iter);
                arrMap[crnt.pos.y][crnt.pos.x].mType = CloseList;
            }
            else
                iter++;
        }
        closeList.push_back(crnt);


       
        crnt = next;

        //crnt�� dest�� �ɶ����� �ݺ�
        if (crnt.pos.x == destPos.x &&
            crnt.pos.y == destPos.y)
        {
            closeList.push_back(crnt);
            arrMap[startPos.y][startPos.x].mType = Start;
            arrMap[crnt.pos.y][crnt.pos.x].mType = Dest;
            break;
        }
 

        //DrawMap(hdc);
    }
    

    
    POINT tempPath{ destPos.x,destPos.y };
  
    while (1)
    {
        if (tempPath.x == startPos.x && tempPath.y == startPos.y)
            break;
            
        for (int i = 0; i < closeList.size(); i++)
        {
            if (closeList[i].pos.x == tempPath.x &&
                closeList[i].pos.y == tempPath.y)
            {
                path.push(closeList[i].pos);
                arrMap[tempPath.y][tempPath.x].mType = Rail;
                tempPath = closeList[i].parent;
                break;
            }
        }
    }
    arrMap[crnt.pos.y][crnt.pos.x].mType = Dest;
}

void PlayerMove(HDC hdc)
{
    POINT p = path.top();
    path.pop();
    POINT center = { p.x, p.y };
    int r = 20;

    center = { p.x * 100 + 50, p.y * 100 + 50 };


    Ellipse(hdc, center.x - r, center.y - r, center.x + r, center.y + r);

    if (p.x == destPos.x && p.y == destPos.y)
        sequence = End;
}

double LengthPts(POINT pt1, POINT pt2)
{
    return (sqrt(
        ((float)(pt2.x - pt1.x) * (pt2.x - pt1.x)) +
        ((float)(pt2.y - pt1.y) * (pt2.y - pt1.y))
    ));
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

}




/*

g ��������� �󸶳� ������ �ִ°�? �Ǵ� �̵��� ��� �ڽ�Ʈ
h �������κ��� �󸶳� �������ִ°�
f g + h

f���� ������ h���� ���� ������ �Ѵ� ������ �������



���
g,h,v,parent

���迭 (���) 0���ε����� ������� ����

Ŭ�����Ʈ ����(���)
���¸���Ʈ(���迭 ���)



���¸���Ʈ���� f�� ���� ���� ��θ� �̾Ƽ�
crnt�� ���� �̰� ���¸���Ʈ���� ������ Ŭ�����Ʈ�� �̵�

������θ� ��ġ�ؼ� ���¸���Ʈ���� �ִµ� �̶� �ߺ��̸� ��g�� ���ؼ� ���ο� ��ΰ�
g�� �� ���� ��� ghv ������ �����ϰ� �θ� ����
������ΰ� ������γ� ���̸� ����, �� �ܸ� ���¸���Ʈ�� �߰�



�� �迭(��� ��� ����Ǿ��ִ���) �����


�켱����ť Ŭ����



*/