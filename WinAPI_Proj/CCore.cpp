#include "pch.h"
#include "CCore.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "Vec2.h"
#include "GObject.h"
//CCore* CCore::g_pInst = nullptr;
CCore::CCore()
	:m_hWnd(0)
	, m_ptResolution{}
	, m_hDC(0)
	, m_hBit(0)
	, m_memDC(0)
{
}

CCore::~CCore()
{
	ReleaseDC(m_hWnd,m_hDC); //GetDC로 만든 Dc는 릴리즈로 해제
	DeleteDC(m_memDC);//createCompatable 로 만든 DC는 Delete로 지운다
	DeleteObject(m_hBit);
}


GObject g_obj;

int CCore::init(HWND _hWnd, POINT _ptResolution)
{
	m_hWnd = _hWnd;
	m_ptResolution = _ptResolution;

	//해상도에 맞게 윈도우 크기 조절
	RECT rect = { 0,0, m_ptResolution.x, m_ptResolution.y };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, true);
	SetWindowPos(m_hWnd, nullptr, 300, 20, rect.right - rect.left, rect.bottom - rect.top, 0);

	m_hDC = GetDC(m_hWnd);

	//더블버퍼링
	m_hBit = CreateCompatibleBitmap(m_hDC, m_ptResolution.x, m_ptResolution.y);
	m_memDC = CreateCompatibleDC(m_hDC);

	HBITMAP hOldBit = (HBITMAP)SelectObject(m_memDC, m_hBit);
	DeleteObject(hOldBit); //DC가 생성되면 1픽셀짜리 비트맵을 가지고있어서 바로삭제

	//Manager 초기화
	CTimeMgr::GetInst()->init();
	CKeyMgr::GetInst()->init();



	g_obj.SetPos(Vec2(m_ptResolution.x / 2.f, m_ptResolution.y / 2.f));
	g_obj.SetScale(Vec2(20.f, 20.f));

	return S_OK;
}



void CCore::Progress()
{
	CTimeMgr::GetInst()->Update();

	Update();
	Render();
}

void CCore::Update()
{
	Vec2 vPos = g_obj.GetPos();

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		vPos.x += 100.f * fDT;

	vPos.y -= 10.f * fDT;
	g_obj.SetPos(vPos);

}

void CCore::Render()
{
	//memDC에다 먼저 그린다
	Rectangle(m_memDC, -1,-1,m_ptResolution.x+1,m_ptResolution.y+1);


	Vec2 vPos = g_obj.GetPos();
	Vec2 vScale = g_obj.GetScale();

	Rectangle(m_memDC,  int(vPos.x - vScale.x / 2.f)
					, int(vPos.y - vScale.y / 2.f)
					, int(vPos.x + vScale.x / 2.f)
					, int(vPos.y + vScale.y / 2.f));

	BitBlt(m_hDC, 0, 0, m_ptResolution.x, m_ptResolution.y
		, m_memDC, 0, 0, SRCCOPY); //소스 비트맵에서 목적비트맵으로 복사
}


