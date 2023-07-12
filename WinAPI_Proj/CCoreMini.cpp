#include "pch.h"
#include "CCore.h"
#include "CKeyMgr.h"
#include "GObject.h"
#include "Player.h"
#include "CTimeMgr.h"
#include "CSpawner.h"
#include "CWall.h"
#include "CommandCenter.h"
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
	ReleaseDC(m_hWnd, m_hDC); //GetDC�� ���� Dc�� ������� ����
	DeleteDC(m_memDC);//createCompatable �� ���� DC�� Delete�� �����
	DeleteObject(m_hBit);
}


GObject g_obj;
Player g_player;
CSpawner g_spawner;
vector<GObject*> g_vWall;
GObject* g_Center;

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



	//g_obj.SetPos(Vec2(m_ptResolution.x / 2.f, m_ptResolution.y / 2.f));
	//g_obj.SetScale(Vec2(20.f, 20.f));
	g_player.SetPos(Vec2(m_ptResolution.x / 2.f, m_ptResolution.y - m_ptResolution.y / 8.f));
	g_player.SetScale(Vec2(20.f, 20.f));
	g_spawner.SetPos(Vec2(m_ptResolution.x / 2.f, m_ptResolution.y - m_ptResolution.y / 1.1f));
	g_spawner.SetScale(Vec2(20.f, 20.f));

	g_player.SetSpawner(g_spawner);
	g_spawner.SetPlayer(g_player);
	g_player.m_CollisionObj.push_back(&g_spawner);
	g_spawner.m_CollisionObj.push_back(&g_player);



	for (int i = 0; i < 5; i++)
	{
		CWall* wall = new CWall(Vec2(78 + i * 153, 930), Vec2(78, 20));
		g_vWall.push_back(wall);
		g_spawner.m_CollisionObj.push_back(wall);
	}

	CommandCenter* center = new CommandCenter(Vec2(384, 990), Vec2(384, 30));
	g_Center = center;
	g_spawner.m_CollisionObj.push_back(center);

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

	/*Vec2 vPos = g_obj.GetPos();

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		vPos.x += 100.f * fDT;

	vPos.y -= 10.f * fDT;
	g_obj.SetPos(vPos);*/
	CKeyMgr::GetInst()->Update();



	g_spawner.Collision(g_player);
	g_player.Update();
	g_spawner.Update();
	for (int i = 0; i < g_vWall.size(); i++)
	{
		g_vWall[i]->Draw();
	}
	g_Center->Update();
}

void CCore::Render()
{
	//memDC���� ���� �׸���
	Rectangle(m_memDC, -1, -1, m_ptResolution.x + 1, m_ptResolution.y + 1);


	g_player.Draw();
	g_spawner.Draw();
	for (int i = 0; i < g_vWall.size(); i++)
	{
		g_vWall[i]->Draw();
	}
	g_Center->Draw();


	/*Vec2 vPos = g_obj.GetPos();
	Vec2 vScale = g_obj.GetScale();

	Rectangle(m_memDC,  int(vPos.x - vScale.x / 2.f)
					, int(vPos.y - vScale.y / 2.f)
					, int(vPos.x + vScale.x / 2.f)
					, int(vPos.y + vScale.y / 2.f));*/




	BitBlt(m_hDC, 0, 0, m_ptResolution.x, m_ptResolution.y
		, m_memDC, 0, 0, SRCCOPY); //�ҽ� ��Ʈ�ʿ��� ������Ʈ������ ����


}

void CCore::GameOver()
{
	TCHAR buf[10];
	wsprintf(buf, TEXT("%d ��"), g_spawner.m_iScore);

	TextOut(CCore::GetInst()->GetmemDC(), 0, 0, buf, _tcslen(buf));


	MessageBox(CCore::GetInst()->GetMainHwnd(), buf, L"���� ����!", MB_OK);



	DestroyWindow(GetMainHwnd());
}


