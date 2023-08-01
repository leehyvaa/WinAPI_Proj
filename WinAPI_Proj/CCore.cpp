#include "pch.h"
#include "CCore.h"
#include "CKeyMgr.h"
#include "GameObject.h"
#include "CTimeMgr.h"
#include "SPlayer.h"
#include "CSceneMgr.h"
#include "CPathMgr.h"
#include "SelectGDI.h"
#include "CCollisionMgr.h"
#include "CEventMgr.h"
CCore::CCore()
	:m_hWnd(0)
	, m_ptResolution{}
	, m_hDC(0)
	, m_hBit(0)
	, m_memDC(0)
	, m_arrPen{}
	, m_arrBrush{}
{
}

CCore::~CCore()
{
	ReleaseDC(m_hWnd,m_hDC); //GetDC로 만든 Dc는 릴리즈로 해제
	DeleteDC(m_memDC);//createCompatable 로 만든 DC는 Delete로 지운다
	DeleteObject(m_hBit);

	for (int i = 0; i < (UINT)PEN_TYPE::END; i++)
	{
		DeleteObject(m_arrPen[i]);
	}
}


int CCore::init(HWND _hWnd, POINT _ptResolution)
{
	


	m_hWnd = _hWnd;
	m_ptResolution = _ptResolution;

	//해상도에 맞게 윈도우 크기 조절
	RECT rect = { 0,0, m_ptResolution.x, m_ptResolution.y };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, true);
	SetWindowPos(m_hWnd, nullptr, 300, 10, rect.right - rect.left, rect.bottom - rect.top, 0);

	m_hDC = GetDC(m_hWnd);

	//더블버퍼링
	m_hBit = CreateCompatibleBitmap(m_hDC, m_ptResolution.x, m_ptResolution.y);
	m_memDC = CreateCompatibleDC(m_hDC);

	HBITMAP hOldBit = (HBITMAP)SelectObject(m_memDC, m_hBit);
	DeleteObject(hOldBit); //DC가 생성되면 1픽셀짜리 비트맵을 가지고있어서 바로삭제

	CreateBrushPen();

	//Manager 초기화
	CTimeMgr::GetInst()->init();
	CKeyMgr::GetInst()->init();
	CPathMgr::GetInst()->init();
	CSceneMgr::GetInst()->init();
	


	return S_OK;
}



void CCore::Progress()
{
	//Manager 업데이트
	CTimeMgr::GetInst()->Update();
	CKeyMgr::GetInst()->Update();

	//씬 업데이트, 충돌처리
	CSceneMgr::GetInst()->Update();
	CCollisionMgr::GetInst()->Update();

	//렌더링
	//화면 클리어
	//memDC에다 먼저 그린다
	Rectangle(m_memDC, -1, -1, m_ptResolution.x + 1, m_ptResolution.y + 1);

	


	CSceneMgr::GetInst()->Render(m_memDC);

	BitBlt(m_hDC, 0, 0, m_ptResolution.x, m_ptResolution.y
		, m_memDC, 0, 0, SRCCOPY); //소스 비트맵에서 목적비트맵으로 복사

	CTimeMgr::GetInst()->Render();

	//이벤트 지연처리
	CEventMgr::GetInst()->Update();
}

void CCore::CreateBrushPen()
{
	m_arrBrush[(UINT)BRUSH_TYPE::HOLLOW] = (HBRUSH)GetStockObject(HOLLOW_BRUSH);

	m_arrPen[(UINT)PEN_TYPE::RED] = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	m_arrPen[(UINT)PEN_TYPE::GREEN] = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	m_arrPen[(UINT)PEN_TYPE::BLUE] = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));


}



void CCore::GameOver()
{
	TCHAR buf[10];
	//wsprintf(buf, TEXT("%d 점"), g_spawner.m_iScore);

	TextOut(CCore::GetInst()->GetmemDC(), 0, 0, buf, _tcslen(buf));


	MessageBox(CCore::GetInst()->GetMainHwnd(),buf, L"게임 종료!", MB_OK);


	
	DestroyWindow(GetMainHwnd());
}


