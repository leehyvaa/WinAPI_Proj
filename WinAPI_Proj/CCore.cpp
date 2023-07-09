#include "pch.h"
#include "CCore.h"

//CCore* CCore::g_pInst = nullptr;
CCore::CCore()
	:m_hWnd(0)
	,m_ptResolution{}
	,m_hDC(0)
{
}

CCore::~CCore()
{
	ReleaseDC(m_hWnd,m_hDC);
}

int CCore::init(HWND _hWnd, POINT _ptResolution)
{
	m_hWnd = _hWnd;
	m_ptResolution = _ptResolution;

	RECT rect = { 0,0, m_ptResolution.x, m_ptResolution.y };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, true);
	SetWindowPos(m_hWnd, nullptr, 300, 20, rect.right - rect.left, rect.bottom - rect.top, 0);

	m_hDC = GetDC(m_hWnd);

	return S_OK;
}

void CCore::Progress()
{
	Update();
	Render();
}

void CCore::Update()
{
}

void CCore::Render()
{
}


