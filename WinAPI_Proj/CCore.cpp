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
	ReleaseDC(m_hWnd,m_hDC); //GetDC�� ���� Dc�� ������� ����

	for (int i = 0; i < (UINT)PEN_TYPE::END; i++)
	{
		DeleteObject(m_arrPen[i]);
	}
}





int CCore::init(HWND _hWnd, POINT _ptResolution)
{
	


	m_hWnd = _hWnd;
	m_ptResolution = _ptResolution;

	//�ػ󵵿� �°� ������ ũ�� ����
	RECT rect = { 0,0, m_ptResolution.x, m_ptResolution.y };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, true);
	SetWindowPos(m_hWnd, nullptr, 300, 10, rect.right - rect.left, rect.bottom - rect.top, 0);

	m_hDC = GetDC(m_hWnd);

	//������۸� �뵵�� �ؽ��� ������ �����.
	m_pMemTex = CResMgr::GetInst()->CreateTexture(L"BackBuffer",(UINT)m_ptResolution.x, (UINT)m_ptResolution.y);

	
	CreateBrushPen();

	//Manager �ʱ�ȭ
	CTimeMgr::GetInst()->init();
	CKeyMgr::GetInst()->init();
	CPathMgr::GetInst()->init();
	CCamera::GetInst()->init();
	CSceneMgr::GetInst()->init();


	return S_OK;
}



void CCore::Progress()
{
	//Manager ������Ʈ
	CTimeMgr::GetInst()->Update();
	CKeyMgr::GetInst()->Update();
	CCamera::GetInst()->Update();


	//�� ������Ʈ, �浹ó��
	CSceneMgr::GetInst()->Update();
	CCollisionMgr::GetInst()->Update();


	//UI �̺�Ʈ üũ
	CUIMgr::GetInst()->Update();



	//������
	//ȭ�� Ŭ����
	//memDC���� ���� �׸���
	
	Rectangle(m_pMemTex->GetDC(), -1, -1, m_ptResolution.x + 1, m_ptResolution.y + 1);

	


	CSceneMgr::GetInst()->Render(m_pMemTex->GetDC());
	CCamera::GetInst()->Render(m_pMemTex->GetDC());


	BitBlt(m_hDC, 0, 0, m_ptResolution.x, m_ptResolution.y
		, m_pMemTex->GetDC(), 0, 0, SRCCOPY); //�ҽ� ��Ʈ�ʿ��� ������Ʈ������ ����

	CTimeMgr::GetInst()->Render();

	//�̺�Ʈ ����ó��
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
	//wsprintf(buf, TEXT("%d ��"), g_spawner.m_iScore);

	//TextOut(CCore::GetInst()->GetmemDC(), 0, 0, buf, _tcslen(buf));


	//MessageBox(CCore::GetInst()->GetMainHwnd(),buf, L"���� ����!", MB_OK);


	
	DestroyWindow(GetMainHwnd());
}


