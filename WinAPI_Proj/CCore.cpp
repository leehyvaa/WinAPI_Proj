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
#include "CCamera.h"
#include "CUIMgr.h"
#include "CResMgr.h"
#include "CTexture.h"
CCore::CCore()
	:m_hWnd(0)
	, m_ptResolution{}
	, m_hDC(0)

	, m_arrPen{}
	, m_arrBrush{}
{
}

CCore::~CCore()
{
	ReleaseDC(m_hWnd,m_hDC); //GetDC로 만든 Dc는 릴리즈로 해제

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

	//더블버퍼링 용도의 텍스쳐 한장을 만든다.
	m_pMemTex = CResMgr::GetInst()->CreateTexture(L"BackBuffer",(UINT)m_ptResolution.x, (UINT)m_ptResolution.y);

	
	CreateBrushPen();

	//Manager 초기화
	CTimeMgr::GetInst()->init();
	CKeyMgr::GetInst()->init();
	CPathMgr::GetInst()->init();
	CCamera::GetInst()->init();
	CSceneMgr::GetInst()->init();


	return S_OK;
}



void CCore::Progress()
{
	//Manager 업데이트
	CTimeMgr::GetInst()->Update();
	CKeyMgr::GetInst()->Update();
	CCamera::GetInst()->Update();


	//씬 업데이트, 충돌처리
	CSceneMgr::GetInst()->Update();
	CCollisionMgr::GetInst()->Update();


	//UI 이벤트 체크
	CUIMgr::GetInst()->Update();



	//렌더링
	//화면 클리어
	//memDC에다 먼저 그린다
	
	Rectangle(m_pMemTex->GetDC(), -1, -1, m_ptResolution.x + 1, m_ptResolution.y + 1);

	


	CSceneMgr::GetInst()->Render(m_pMemTex->GetDC());
	CCamera::GetInst()->Render(m_pMemTex->GetDC());


	BitBlt(m_hDC, 0, 0, m_ptResolution.x, m_ptResolution.y
		, m_pMemTex->GetDC(), 0, 0, SRCCOPY); //소스 비트맵에서 목적비트맵으로 복사

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

	//TextOut(CCore::GetInst()->GetmemDC(), 0, 0, buf, _tcslen(buf));


	//MessageBox(CCore::GetInst()->GetMainHwnd(),buf, L"게임 종료!", MB_OK);


	
	DestroyWindow(GetMainHwnd());
}


