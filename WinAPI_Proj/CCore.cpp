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
#include "CScene.h"
#include "CBackGround.h"
#include "CSoundMgr.h"
#include "CSound.h"
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

	//�ػ󵵿� �°� ������ ũ�� ����
	ChangeWindowSize(m_ptResolution, false);
	
	//�޴��� ����
	m_hMenu = LoadMenu(nullptr, MAKEINTRESOURCEW(IDC_WINAPIPROJ));

	m_hDC = GetDC(m_hWnd);

	//������۸� �뵵�� �ؽ��� ������ �����.
	m_pMemTex = CResMgr::GetInst()->CreateTexture(L"BackBuffer",(UINT)m_ptResolution.x, (UINT)m_ptResolution.y);

	
	CreateBrushPen();

	//Manager �ʱ�ȭ
	CTimeMgr::GetInst()->init();
	CKeyMgr::GetInst()->init();
	CPathMgr::GetInst()->init();
	CCamera::GetInst()->init();
	CSoundMgr::GetInst()->init();
	CSceneMgr::GetInst()->init();


	//Sound �ε� �׽�Ʈ
	//CResMgr::GetInst()->LoadSound(L"BGM_01", L"sound\\BGM_Title.wav");
	//CSound* pNewSound = CResMgr::GetInst()->FindSound(L"BGM_01");

	//pNewSound->PlayToBGM(true);
	//pNewSound->Play();

	//pNewSound->SetPosition(50.f); //�����, �Ҹ� ��ġ ����
	//pNewSound->SetVolume(20.f);


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
	Clear(m_pMemTex->GetDC());
	


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
	m_arrBrush[(UINT)BRUSH_TYPE::BLACK] = (HBRUSH)GetStockObject(BLACK_BRUSH);
	m_arrBrush[(UINT)BRUSH_TYPE::RED] = CreateSolidBrush(RGB(255, 0, 0));
	m_arrBrush[(UINT)BRUSH_TYPE::MAGENTA] = CreateSolidBrush(RGB(255, 0, 255));



	m_arrPen[(UINT)PEN_TYPE::RED] = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	m_arrPen[(UINT)PEN_TYPE::GREEN] = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	m_arrPen[(UINT)PEN_TYPE::BLUE] = CreatePen(PS_SOLID, 1, RGB(80, 183, 220));
	m_arrPen[(UINT)PEN_TYPE::PURPLE] = CreatePen(PS_SOLID, 1, RGB(102, 0, 153));
	m_arrPen[(UINT)PEN_TYPE::ORANGE] = CreatePen(PS_SOLID, 1, RGB(255, 165, 0));


}

void CCore::Clear(HDC _dc)
{
	//SelectGDI gdi(m_pMemTex->GetDC(), BRUSH_TYPE::BLACK);
	//Rectangle(m_pMemTex->GetDC(), -1, -1, m_ptResolution.x + 1, m_ptResolution.y + 1);
	CScene* curScene = CSceneMgr::GetInst()->GetCurScene();
	CBackGround* backGround = curScene->GetBackGround();
	if (backGround == nullptr)
		return;


	Vec2 vRenderPos = backGround->GetPos();

	Vec2 vScale = backGround->GetScale();


	UINT iWidth = backGround->GetTexture()->Width();
	UINT iHeight = backGround->GetTexture()->Height();


	TransparentBlt(_dc
		, int(vRenderPos.x)
		, int(vRenderPos.y)
		, (int)vScale.x, (int)vScale.y
		, backGround->GetTexture()->GetDC()
		, 0, 0,
		iWidth, iHeight, RGB(255, 0, 255));



}

void CCore::DockMenu()
{
	//�޴��� ����
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




