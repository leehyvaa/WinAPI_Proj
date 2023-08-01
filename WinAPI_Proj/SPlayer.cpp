#include "pch.h"
#include "CCore.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"

#include "SPlayer.h"
#include "CTexture.h"
#include "CResMgr.h"
#include "CCollider.h"
#include "CWire.h"

SPlayer::SPlayer()
	:m_pTex(nullptr)
	, m_fSpeed(1000)
{
	m_pTex = CResMgr::GetInst()->LoadTexture(L"PlayerTex", L"texture\\sigong.bmp");
	CreateCollider();
	GetCollider()->SetOffsetPos(Vec2());
	GetCollider()->SetScale(Vec2(50.f, 50.f));
}


SPlayer::~SPlayer()
{
}



void SPlayer::Update()
{
	Vec2 vPos = GetPos();





	if (KEY_TAP(KEY::Q))
		cout << endl;
	if (KEY_TAP(KEY::W))
		//GMap::GetInst()->DrawMap();
		if (KEY_TAP(KEY::E))
			system("cls");


	if (KEY_TAP(KEY::SPACE))
		CreateWire();


	if (KEY_HOLD(KEY::LEFT))
	{
		vPos.x= vPos.x - m_fSpeed * fDT;
	}
	if (KEY_HOLD(KEY::RIGHT))
	{
		vPos.x = vPos.x + m_fSpeed * fDT;
	}
	if (KEY_HOLD(KEY::UP))
	{
		vPos.y = vPos.y - m_fSpeed * fDT;
	}
	if (KEY_HOLD(KEY::DOWN))
	{	
		vPos.y = vPos.y + m_fSpeed * fDT;
	}



	SetPos(vPos);
}

void SPlayer::Render(HDC _dc)
{
	Vec2 vPos(GetPos().x, GetPos().y);
	Vec2 vScale(GetScale().x, GetScale().y);


	
	int iWidth = (int)m_pTex->Width();
	int iHeight = (int)m_pTex->Height();


	TransparentBlt(_dc
		, int(vPos.x - (float)(iWidth / 2))
		, int(vPos.y - (float)(iHeight / 2))
		, iWidth, iHeight
		, m_pTex->GetDC()
		, 0, 0, iWidth, iHeight, RGB(255, 0, 255));

	Component_Render(_dc);
}

void SPlayer::CreateWire()
{
	Vec2 vWirePos = GetPos();
	vWirePos.y -= GetScale().y / 2.f;

	CWire* pWire = new CWire;
	pWire->SetName(L"Wire");
	pWire->SetPos(vWirePos);
	pWire->SetScale(Vec2(11.f, 11.f));
	pWire->SetDir(Vec2(0.f, -1.f));

	CreateObject(pWire, GROUP_TYPE::PROJ_PLAYER);
	//CreateObject 함수에 포지션, 방향, 스케일을 설정해주는 인자를 넣어야함
}