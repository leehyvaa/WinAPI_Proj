// WindowsProject11.cpp : 애플리케이션에 대한 진입점을 정의합니다.
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

    RECT rect = { 0,0, 1000, 1000 };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, true);
    SetWindowPos(hWnd, nullptr, 300, 10, rect.right - rect.left, rect.bottom - rect.top, 0);



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
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_WINAPIPROJ));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
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
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

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
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
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
           
            //맵 타입 따라서 색상 바꿔서 그리기
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

    //각각의 방향에 대한 정보를 넣고 오픈리스트에 등록
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

 

        
        //우선순위큐로 오픈리스트중 제일 좋은 경로 확인
        PriorityQueue p;
        for (int i = 0; i < openList.size(); i++)
        {
            p.Push(openList[i]);
        }

        Node next = p.Pop();

        //경로로 crnt를 옮기고 오픈리스트에서 꺼내서 클로즈리스트에 추가
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

        //crnt가 dest가 될때까지 반복
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

g 출발점에서 얼마나 떨어져 있는가? 또는 이동에 드는 코스트
h 목적지로부터 얼마나 떨어져있는가
f g + h

f값이 같으면 h값이 가장 작은거 둘다 같으면 상관없음



노드
g,h,v,parent

힙배열 (노드) 0번인덱스는 사용하지 않음

클로즈리스트 베열(노드)
오픈리스트(힙배열 노드)



오픈리스트에서 f가 제일 낮을 경로를 뽑아서
crnt로 설정 이걸 오픈리스트에서 꺼내서 클로즈리스트로 이동

인접경로를 서치해서 오픈리스트에서 넣는데 이때 중복이면 값g를 비교해서 새로운 경로가
g가 더 작을 경우 ghv 비용들을 수정하고 부모를 수정
인접경로가 닫힌경로나 벽이면 무시, 그 외면 오픈리스트에 추가



맵 배열(노드 어디가 연결되어있는지) 만들고


우선순위큐 클래스



*/