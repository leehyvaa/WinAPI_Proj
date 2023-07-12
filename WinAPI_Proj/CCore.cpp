#include "pch.h"
#include "CCore.h"
#include "CKeyMgr.h"
#include "GObject.h"
#include "CTimeMgr.h"
#include "GMap.h"
#include "GPlayer.h"

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
	ReleaseDC(m_hWnd,m_hDC); //GetDC�� ���� Dc�� ������� ����
	DeleteDC(m_memDC);//createCompatable �� ���� DC�� Delete�� �����
	DeleteObject(m_hBit);
}


GObject* g_player;
int CCore::init(HWND _hWnd, POINT _ptResolution)
{
	


	m_hWnd = _hWnd;
	m_ptResolution = _ptResolution;

	//�ػ󵵿� �°� ������ ũ�� ����
	RECT rect = { 0,0, m_ptResolution.x, m_ptResolution.y };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, true);
	SetWindowPos(m_hWnd, nullptr, 300, 10, rect.right - rect.left, rect.bottom - rect.top, 0);

	m_hDC = GetDC(m_hWnd);

	//������۸�
	m_hBit = CreateCompatibleBitmap(m_hDC, m_ptResolution.x, m_ptResolution.y);
	m_memDC = CreateCompatibleDC(m_hDC);

	HBITMAP hOldBit = (HBITMAP)SelectObject(m_memDC, m_hBit);
	DeleteObject(hOldBit); //DC�� �����Ǹ� 1�ȼ�¥�� ��Ʈ���� �������־ �ٷλ���

	//Manager �ʱ�ȭ
	CTimeMgr::GetInst()->init();
	CKeyMgr::GetInst()->init();
	GMap::GetInst()->init();

	g_player = new GPlayer(Vec2(9,9), Vec2(5, 5));


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
	CKeyMgr::GetInst()->Update();


	g_player->Update();

}

void CCore::Render()
{
	//memDC���� ���� �׸���
	Rectangle(m_memDC, -1,-1,m_ptResolution.x+1,m_ptResolution.y+1);


	g_player->Draw();
	POINT pos{ 390,510 };
	GMap::GetInst()->DrawGrid(CCore::GetInst()->GetmemDC(), pos,760,1000,76,100);




	BitBlt(m_hDC, 0, 0, m_ptResolution.x, m_ptResolution.y
		, m_memDC, 0, 0, SRCCOPY); //�ҽ� ��Ʈ�ʿ��� ������Ʈ������ ����

	
}

void CCore::GameOver()
{
	TCHAR buf[10];
	//wsprintf(buf, TEXT("%d ��"), g_spawner.m_iScore);

	TextOut(CCore::GetInst()->GetmemDC(), 0, 0, buf, _tcslen(buf));


	MessageBox(CCore::GetInst()->GetMainHwnd(),buf, L"���� ����!", MB_OK);


	
	DestroyWindow(GetMainHwnd());
}


