#include "pch.h"
#include "CCore.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"

#include "SPlayer.h"
#include "CTexture.h"
#include "CResMgr.h"
#include "CCollider.h"

SPlayer::SPlayer()
	:m_pTex(nullptr)
{
	m_pTex = CResMgr::GetInst()->LoadTexture(L"PlayerTex", L"texture\\sigong.bmp");
	CreateCollider();
	GetCollider()->SetScale(Vec2(100.f, 100.f));
}

SPlayer::SPlayer(Vec2 _vPos, Vec2 _vScale)
	:m_pTex(nullptr)
{
	m_pTex = CResMgr::GetInst()->LoadTexture(L"PlayerTex", L"texture\\sigong.bmp");
	CreateCollider();
	GetCollider()->SetOffsetPos(Vec2());
	GetCollider()->SetScale(Vec2(50.f, 100.f));

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
	if (KEY_TAP(KEY::V))








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
