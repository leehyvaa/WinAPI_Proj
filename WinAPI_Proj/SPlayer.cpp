#include "pch.h"
#include "CCore.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"

#include "SPlayer.h"
#include "CTexture.h"
#include "CResMgr.h"
#include "CCollider.h"
#include "CWire.h"
#include "CAnimator.h"
#include "CAnimation.h"


SPlayer::SPlayer()
	: m_fSpeed(1000)
{
	//m_pTex = CResMgr::GetInst()->LoadTexture(L"PlayerTex", L"texture\\sigong.bmp");
	
	CreateCollider();
	GetCollider()->SetOffsetPos(Vec2());
	GetCollider()->SetScale(Vec2(50.f, 50.f));

	CTexture* pTex = CResMgr::GetInst()->LoadTexture(L"PlayerTex", L"texture\\zero_run.bmp");
	CreateAnimator();
	GetAnimator()->CreateAnimation(L"WALK_RIGHT", pTex,
		Vec2(0.f, 0.f), Vec2(57.f, 52.f), Vec2(57.f, 0.f),0.1f, 16);
	GetAnimator()->Play(L"WALK_RIGHT",true);


	//�ִϸ��̼� ������ �ֱ�
	//CAnimation* pAnim = GetAnimator()->FindAnimation(L"WALK_RIGHT");
	//for (UINT i = 0; i < pAnim->GetMaxFrame(); i++)
	//{
	//	pAnim->GetFrame(i).vOffset = Vec2(0.f, -20.f);
	//}
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


	if (KEY_HOLD(KEY::A))
	{
		vPos.x= vPos.x - m_fSpeed * fDT;
	}
	if (KEY_HOLD(KEY::D))
	{
		vPos.x = vPos.x + m_fSpeed * fDT;
	}
	if (KEY_HOLD(KEY::W))
	{
		vPos.y = vPos.y - m_fSpeed * fDT;
	}
	if (KEY_HOLD(KEY::S))
	{	
		vPos.y = vPos.y + m_fSpeed * fDT;
	}



	SetPos(vPos);

	GetAnimator()->Update();
}

void SPlayer::Render(HDC _dc)
{
	//Vec2 vPos(GetPos().x, GetPos().y);
	Vec2 vScale(GetScale().x, GetScale().y);


	/*
	int iWidth = (int)m_pTex->Width();
	int iHeight = (int)m_pTex->Height();*/


	/*TransparentBlt(_dc
		, int(vPos.x - (float)(iWidth / 2))
		, int(vPos.y - (float)(iHeight / 2))
		, iWidth, iHeight
		, m_pTex->GetDC()
		, 0, 0, iWidth, iHeight, RGB(255, 0, 255));*/

	//CTexture* pTex = CResMgr::GetInst()->LoadTexture(L"PlayerTex", L"texture\\sigong.bmp");
	CTexture* pTex = CResMgr::GetInst()->LoadTexture(L"PlayerTex1", L"texture\\Sail_Fish.bmp");


	//���ĺ��带 ����� ������

	Vec2 vPos = GetPos();
	vPos = CCamera::GetInst()->GetRenderPos(vPos);
	float width = (float)pTex->Width();
	float height = (float)pTex->Height();


	BLENDFUNCTION bf = {};
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.AlphaFormat = AC_SRC_ALPHA;
	bf.SourceConstantAlpha = 127; //���������� ����Ǵ� ����

	AlphaBlend(_dc
		, int(vPos.x - width / 2.f)
		, int(vPos.y - height / 2.f)
		, int(width), int(height)
		, pTex->GetDC()
		, 0, 0, int(width), int(height)
		, bf);

	//Component_Render(_dc);
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
	//CreateObject �Լ��� ������, ����, �������� �������ִ� ���ڸ� �־����
}