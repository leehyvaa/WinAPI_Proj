#include "pch.h"
#include "CCore.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"

#include "SPlayer.h"
#include "CTexture.h"
#include "CResMgr.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CAnimation.h"
#include "CRigidBody.h"
#include "CGravity.h"
SPlayer::SPlayer()
	: m_fSpeed(1000)
	, m_iDir(1)
	, m_iPrevDir(1)
	, m_eCurState(PLAYER_STATE::IDLE)
	, m_ePrevState(PLAYER_STATE::RUN)
	, m_bOnGround(false)
	
{
	//m_pTex = CResMgr::GetInst()->LoadTexture(L"PlayerTex", L"texture\\sigong.bmp");
	

	//67 -13분 캐릭터 상태변환후 애니메이션 전환
	CreateCollider();
	GetCollider()->SetScale(Vec2(32.f, 102.f));
	GetCollider()->SetOffsetPos(Vec2(0.f, -(GetCollider()->GetScale().y)/2.f));
	


	CreateRigidBody();
	CreateAnimator();


#pragma region 플레이어 애니메이션
	//텍스쳐 로딩
	CTexture* pTexRight = CResMgr::GetInst()->LoadTexture(L"PlayerTex_Right", L"texture\\player\\Player_Right.bmp");
	CTexture* pTexLeft = CResMgr::GetInst()->LoadTexture(L"PlayerTex_Left", L"texture\\player\\Player_Left.bmp");

	

	//애니메이션 로딩
	//GetAnimator()->LoadAnimation(L"animation\\player_right_idle.anim");



	//RIGHT 애니메이션 생성
	GetAnimator()->CreateAnimation(L"SNB_RIGHT_IDLE", pTexRight,
		Vec2(0.f, 900.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f),0.25f, 8, 3.f,Vec2(0.f,-57.f));
	GetAnimator()->CreateAnimation(L"SNB_RIGHT_RUN", pTexRight,
		Vec2(0.f, 1400.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.07f, 20, 3.f,Vec2(0.f,-57.f));
	GetAnimator()->CreateAnimation(L"SNB_RIGHT_JUMP", pTexRight,
		Vec2(0.f, 1000.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.25f, 6, 3.f, Vec2(0.f, -57.f));
	GetAnimator()->CreateAnimation(L"SNB_RIGHT_FALLING", pTexRight,
		Vec2(0.f, 600.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.25f, 3, 3.f, Vec2(0.f, -57.f));
	GetAnimator()->CreateAnimation(L"SNB_RIGHT_LAND", pTexRight,
		Vec2(0.f, 1100.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.25f, 3, 3.f, Vec2(0.f, -57.f));
	GetAnimator()->CreateAnimation(L"SNB_RIGHT_CLIMBUP", pTexRight,
		Vec2(0.f, 2800.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.25f, 3, 3.f, Vec2(0.f, -57.f));
	GetAnimator()->CreateAnimation(L"SNB_RIGHT_SLIDING", pTexRight,
		Vec2(0.f, 3000.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.25f, 3, 3.f, Vec2(0.f, -57.f));

	//LEFT 애니메이션 생성
	GetAnimator()->CreateAnimation(L"SNB_LEFT_IDLE", pTexLeft,
		Vec2(0.f, 900.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.25f, 8, 3.f, Vec2(0.f, -57.f));
	GetAnimator()->CreateAnimation(L"SNB_LEFT_RUN", pTexLeft,
		Vec2(0.f, 1400.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.07f, 20, 3.f, Vec2(0.f, -57.f));
	GetAnimator()->CreateAnimation(L"SNB_LEFT_JUMP", pTexLeft,
		Vec2(0.f, 1000.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.25f, 6, 3.f, Vec2(0.f, -57.f));
	GetAnimator()->CreateAnimation(L"SNB_LEFT_FALLING", pTexLeft,
		Vec2(0.f, 600.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.25f, 3, 3.f, Vec2(0.f, -57.f));
	GetAnimator()->CreateAnimation(L"SNB_LEFT_LAND", pTexLeft,
		Vec2(0.f, 1100.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.25f, 3, 3.f, Vec2(0.f, -57.f));
	GetAnimator()->CreateAnimation(L"SNB_LEFT_CLIMBUP", pTexLeft,
		Vec2(0.f, 2800.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.25f, 3, 3.f, Vec2(0.f, -57.f));
	GetAnimator()->CreateAnimation(L"SNB_LEFT_SLIDING", pTexLeft,
		Vec2(0.f, 3000.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.25f, 3, 3.f, Vec2(0.f, -57.f));


	
	//RIGHT 애니메이션 저장
	GetAnimator()->FindAnimation(L"SNB_RIGHT_IDLE")->Save(L"animation\\player_right_idle.anim");
	GetAnimator()->FindAnimation(L"SNB_RIGHT_RUN")->Save(L"animation\\player_right_run.anim");
	GetAnimator()->FindAnimation(L"SNB_RIGHT_JUMP")->Save(L"animation\\player_right_jump.anim");
	GetAnimator()->FindAnimation(L"SNB_RIGHT_FALLING")->Save(L"animation\\player_right_falling.anim");
	GetAnimator()->FindAnimation(L"SNB_RIGHT_LAND")->Save(L"animation\\player_right_land.anim");
	GetAnimator()->FindAnimation(L"SNB_RIGHT_CLIMBUP")->Save(L"animation\\player_right_climbup.anim");
	GetAnimator()->FindAnimation(L"SNB_RIGHT_SLIDING")->Save(L"animation\\player_right_sliding.anim");


	//LEFT 애니메이션 저장
	GetAnimator()->FindAnimation(L"SNB_LEFT_IDLE")->Save(L"animation\\player_left_idle.anim");
	GetAnimator()->FindAnimation(L"SNB_LEFT_RUN")->Save(L"animation\\player_left_run.anim");
	GetAnimator()->FindAnimation(L"SNB_LEFT_JUMP")->Save(L"animation\\player_left_jump.anim");
	GetAnimator()->FindAnimation(L"SNB_LEFT_FALLING")->Save(L"animation\\player_left_falling.anim");
	GetAnimator()->FindAnimation(L"SNB_LEFT_LAND")->Save(L"animation\\player_left_land.anim");
	GetAnimator()->FindAnimation(L"SNB_LEFT_CLIMBUP")->Save(L"animation\\player_left_climbup.anim");
	GetAnimator()->FindAnimation(L"SNB_LEFT_SLIDING")->Save(L"animation\\player_left_sliding.anim");




	GetAnimator()->Play(L"SNB_RIGHT_RUN", true);
#pragma endregion

	


	//애니메이션 오프셋 개별로 넣기
	//CAnimation* pAnim = GetAnimator()->FindAnimation(L"WALK_RIGHT");
	//for (UINT i = 0; i < pAnim->GetMaxFrame(); i++)
	//{
	//	pAnim->GetFrame(i).vOffset = Vec2(0.f, -20.f);
	//}


	CreateGravity();

}


SPlayer::~SPlayer()
{ 
}



void SPlayer::Update()
{
	
	Update_State();
	Update_Move();
	Update_Animation();


	if (KEY_TAP(KEY::E))
		system("cls");



	//if (KEY_TAP(KEY::SPACE))
	//	CreateWire();

	if (KEY_TAP(KEY::C))
	{
		cout << GetRigidBody()->GetSpeed() << endl;
		cout << GetRigidBody()->GetVelocity().x << endl;
		cout << GetRigidBody()->GetVelocity().y << endl;

	}



	GetAnimator()->Update();

	m_ePrevState = m_eCurState;
	m_iPrevDir = m_iDir;
}

void SPlayer::Render(HDC _dc)
{
	//Vec2 vPos(GetPos().x, GetPos().y);
	//Vec2 vScale(GetScale().x, GetScale().y);


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









	//알파블렌드를 사용한 랜더링
	//CTexture* pTex = CResMgr::GetInst()->LoadTexture(L"PlayerTex1", L"texture\\Sail_Fish.bmp");

	//Vec2 vPos = GetPos();
	//vPos = CCamera::GetInst()->GetRenderPos(vPos);
	//float width = (float)pTex->Width();
	//float height = (float)pTex->Height();


	//BLENDFUNCTION bf = {};
	//bf.BlendOp = AC_SRC_OVER;
	//bf.BlendFlags = 0;
	//bf.AlphaFormat = AC_SRC_ALPHA;
	//bf.SourceConstantAlpha = 127; //전역적으로 적용되는 알파

	//AlphaBlend(_dc
	//	, int(vPos.x - width / 2.f)
	//	, int(vPos.y - height / 2.f)
	//	, int(width), int(height)
	//	, pTex->GetDC()
	//	, 0, 0, int(width), int(height)
	//	, bf);




	Component_Render(_dc);
}



void SPlayer::Update_State()
{
	switch (m_eCurState)
	{
	case PLAYER_STATE::IDLE:

		break;
	case PLAYER_STATE::RUN:

		break;
	case PLAYER_STATE::ATTACK:

		break;

	case PLAYER_STATE::JUMP:
		break;

	case PLAYER_STATE::CLIMB:
		if (m_bOnGround)
			m_eCurState = PLAYER_STATE::IDLE;
		break;

	case PLAYER_STATE::DAMAGED:

		break;
	case PLAYER_STATE::DEAD:

		break;
	default:
		break;
	}



	if (KEY_HOLD(KEY::A) && m_eCurState != PLAYER_STATE::CLIMB)
	{
		m_iDir = -1;

		if(PLAYER_STATE::IDLE == m_eCurState)
			m_eCurState = PLAYER_STATE::RUN;
	}
	if (KEY_HOLD(KEY::D) && m_eCurState != PLAYER_STATE::CLIMB)
	{
		m_iDir = 1;
		if (PLAYER_STATE::IDLE == m_eCurState)
			m_eCurState = PLAYER_STATE::RUN;
	}

	if (0.f == GetRigidBody()->GetSpeed() && m_bOnGround)
	{
		m_eCurState = PLAYER_STATE::IDLE;
	}


	if (KEY_TAP(KEY::SPACE))
	{
		m_eCurState = PLAYER_STATE::JUMP;
		
		if (GetRigidBody())
		{
			GetRigidBody()->SetVelocity(Vec2(GetRigidBody()->GetVelocity().x, -500.f));
		}

	}


}

void SPlayer::Update_Move()
{
	CRigidBody* pRigid = GetRigidBody();

	switch (m_eCurState)
	{
	case PLAYER_STATE::IDLE:
	{
		HorizontalMove();
	}
		break;
	case PLAYER_STATE::RUN:
	{
		HorizontalMove();
	}
		break;
	case PLAYER_STATE::ATTACK:

		break;

	case PLAYER_STATE::JUMP:
		HorizontalMove();

		break;

	case PLAYER_STATE::CLIMB:
	{
		VirticalMove();
	}
		break;

	case PLAYER_STATE::DAMAGED:

		break;
	case PLAYER_STATE::DEAD:

		break;
	default:
		break;
	}
	






}

void SPlayer::Update_Animation()
{
	if (m_ePrevState == m_eCurState && m_iPrevDir == m_iDir)
		return;


	switch (m_eCurState)
	{
	case PLAYER_STATE::IDLE:
		if(m_iDir == -1)
			GetAnimator()->Play(L"SNB_LEFT_IDLE", true);
		else
			GetAnimator()->Play(L"SNB_RIGHT_IDLE", true);
		break;
	case PLAYER_STATE::RUN:
		if (m_iDir == -1)
			GetAnimator()->Play(L"SNB_LEFT_RUN", true);
		else
			GetAnimator()->Play(L"SNB_RIGHT_RUN", true);
		break;
	case PLAYER_STATE::ATTACK:

		break;

	case PLAYER_STATE::JUMP:
		Jump();
		break;

	case PLAYER_STATE::CLIMB:
		Climb();
		break;

	case PLAYER_STATE::DAMAGED:

		break;
	case PLAYER_STATE::DEAD:

		break;
	default:
		break;
	}
}



void SPlayer::Update_Gravity()
{
	GetRigidBody()->AddForce(Vec2(0.f, 500.f));
}

void SPlayer::OnCollisionEnter(CCollider* _pOther)
{
	GameObject* pOtherObj = _pOther->GetObj();

	if (_pOther->GetObj()->GetName() == L"Ground")
	{
		Vec2 vPos = GetPos();
		Vec2 vGroundPos = pOtherObj->GetPos();
		Vec2 vGroundScale = pOtherObj->GetScale();


		if (vPos.y <= vGroundPos.y &&
			vPos.x >= vGroundPos.x &&
			vPos.x <= vGroundPos.x + vGroundScale.x)
		{
			m_eCurState = PLAYER_STATE::IDLE;
		}

		if (vPos.y > vGroundPos.y + 70.f &&
			vPos.x <= vGroundPos.x)
		{
			m_eCurState = PLAYER_STATE::CLIMB;
		}

		if (vPos.y > vGroundPos.y + 70.f &&
			vPos.x >= vGroundPos.x + vGroundScale.x)
		{
			m_eCurState = PLAYER_STATE::CLIMB;
		}
		

	}





}

void SPlayer::OnCollisionExit(CCollider* _pOther)
{
	GameObject* pOtherObj = _pOther->GetObj();

	if (_pOther->GetObj()->GetName() == L"Ground")
	{
		if (m_eCurState == PLAYER_STATE::CLIMB)
		{
			m_bOnGround = false;
			m_eCurState = PLAYER_STATE::JUMP;
		}
		


	}
}

void SPlayer::Jump()
{
	if (m_iDir == -1)
		GetAnimator()->Play(L"SNB_LEFT_JUMP", true);
	else
		GetAnimator()->Play(L"SNB_RIGHT_JUMP", true);
}

void SPlayer::Climb()
{
	if (m_iDir == -1)
	{
		GetAnimator()->Play(L"SNB_LEFT_CLIMBUP", true);

	}
	else
	{
		GetAnimator()->Play(L"SNB_RIGHT_CLIMBUP", true);

	}
}

void SPlayer::HorizontalMove()
{
	CRigidBody* pRigid = GetRigidBody();


	if (KEY_HOLD(KEY::A))
	{
		pRigid->SetVelocity(Vec2(-500.f, pRigid->GetVelocity().y));
	}
	if (KEY_HOLD(KEY::D))
	{
		pRigid->SetVelocity(Vec2(500.f, pRigid->GetVelocity().y));
	}



	if (KEY_AWAY(KEY::A))
	{
		pRigid->SetVelocity(Vec2(0.f, pRigid->GetVelocity().y));
	}
	if (KEY_AWAY(KEY::D))
	{
		pRigid->SetVelocity(Vec2(0.f, pRigid->GetVelocity().y));
	}

}

void SPlayer::VirticalMove()
{
	CRigidBody* pRigid = GetRigidBody();


	if (KEY_HOLD(KEY::W))
	{
		pRigid->SetVelocity(Vec2(0.f, -500.f));
	}
	if (KEY_HOLD(KEY::S))
	{
		pRigid->SetVelocity(Vec2(0.f, 500.f));
	}



	if (KEY_AWAY(KEY::W))
	{
		pRigid->SetVelocity(Vec2(pRigid->GetVelocity().x, 0.f));
	}
	if (KEY_AWAY(KEY::S))
	{
		pRigid->SetVelocity(Vec2(pRigid->GetVelocity().x,0.f));
	}
}


