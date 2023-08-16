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
#include "SelectGDI.h"
#include "resource.h"


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

	for (int i = 0; i < (UINT)BRUSH_TYPE::END; i++)
	{
		DeleteObject(m_arrBrush[i]);
	}


	DestroyMenu(m_hMenu);
}





int CCore::init(HWND _hWnd, POINT _ptResolution)
{
	


	m_hWnd = _hWnd;
	m_ptResolution = _ptResolution;

	//해상도에 맞게 윈도우 크기 조절
	ChangeWindowSize(m_ptResolution, false);
	
	//메뉴바 생성
	m_hMenu = LoadMenu(nullptr, MAKEINTRESOURCEW(IDC_WINAPIPROJ));

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


	//Sound 로드 테스트
	//CResMgr::GetInst->LoadSound(L"BGM_01", L"sound\\DM.wav");
	//CSound* pNewSound = CResMgr::GetInst()->FindSound(L"BGM_01");

	//pNewSound->Play();

	//pNewSound->SetPosition(50.f); //백분율, 소리 위치 설정
	//pNewSound -> PlayToBGM(true);
	//pNewSound->SetVolume(60.f);


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
	Clear();
	


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
	m_arrBrush[(UINT)BRUSH_TYPE::BLACK] = (HBRUSH)GetStockObject(BLACK_BRUSH);
	m_arrBrush[(UINT)BRUSH_TYPE::RED] = CreateSolidBrush(RGB(255, 0, 0));



	m_arrPen[(UINT)PEN_TYPE::RED] = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	m_arrPen[(UINT)PEN_TYPE::GREEN] = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	m_arrPen[(UINT)PEN_TYPE::BLUE] = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));


}

void CCore::Clear()
{
	//SelectGDI gdi(m_pMemTex->GetDC(), BRUSH_TYPE::BLACK);
	Rectangle(m_pMemTex->GetDC(), -1, -1, m_ptResolution.x + 1, m_ptResolution.y + 1);
}

void CCore::DockMenu()
{
	//메뉴바 장착
	SetMenu(m_hWnd, m_hMenu);
	ChangeWindowSize(GetResolution(), true);

}

void CCore::DivideMenu()
{
	SetMenu(m_hWnd, nullptr);
	ChangeWindowSize(GetResolution(), false);
}

void CCore::ChangeWindowSize(Vec2 _vResolution, bool _bMenu)
{
	RECT rect = { 0,0, (long)_vResolution.x, (long)_vResolution.y};
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(m_hWnd, nullptr, 0, 0, rect.right - rect.left, rect.bottom - rect.top, 0);


}

HDC CCore::GetmemDC()
{
	return m_pMemTex->GetDC();
}



void CCore::GameOver()
{
	//TCHAR buf[10];
	//wsprintf(buf, TEXT("%d 점"), g_spawner.m_iScore);

	//TextOut(CCore::GetInst()->GetmemDC(), 0, 0, buf, _tcslen(buf));


	//MessageBox(CCore::GetInst()->GetMainHwnd(),buf, L"게임 종료!", MB_OK);


	
	DestroyWindow(GetMainHwnd());
}


