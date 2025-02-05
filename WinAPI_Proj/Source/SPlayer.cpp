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
#include "PlayerArm.h"
#include "Raycast.h"
#include "CHook.h"


SPlayer::SPlayer()
	: m_fSpeed(1000)
	, m_iDir(1)
	, m_iPrevDir(1)
	, m_eCurState(PLAYER_STATE::IDLE)
	, m_ePrevState(PLAYER_STATE::RUN)
	, m_bOnGround(false)
	, playerArm(nullptr)
	, playerHook(nullptr)
	, isClimbing(false)
	, onCollisionRay(nullptr)
	, targetPos(Vec2(0.f,0.f))
	, wireRange(-1.f)
	, wireMaxRange(600.f)
	, moveEnergy(0.f)
	, posEnergy(0.f)
	, canBooster(false)
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
		Vec2(0.f, 2800.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.06f, 10, 3.f, Vec2(-13.f, -57.f));
	GetAnimator()->CreateAnimation(L"SNB_RIGHT_CLIMBDOWN", pTexRight,
		Vec2(0.f, 2700.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.06f, 7, 3.f, Vec2(-13.f, -57.f));
	GetAnimator()->CreateAnimation(L"SNB_RIGHT_CLIMBSTOP", pTexRight,
		Vec2(0.f, 2900.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.2f, 11, 3.f, Vec2(-13.f, -57.f));
	GetAnimator()->CreateAnimation(L"SNB_RIGHT_SWING", pTexRight,
		Vec2(0.f, 2300.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.2f, 15, 3.f, Vec2(-13.f, -57.f));



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
		Vec2(0.f, 2800.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.06f, 10, 3.f, Vec2(13.f, -57.f));
	GetAnimator()->CreateAnimation(L"SNB_LEFT_CLIMBDOWN", pTexLeft,
		Vec2(0.f, 2700.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.06f, 7, 3.f, Vec2(13.f, -57.f));
	GetAnimator()->CreateAnimation(L"SNB_LEFT_CLIMBSTOP", pTexLeft,
		Vec2(0.f, 2900.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.2f, 11, 3.f, Vec2(13.f, -57.f));
	GetAnimator()->CreateAnimation(L"SNB_LEFT_SWING", pTexLeft,
		Vec2(0.f, 2300.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.2f, 15, 3.f, Vec2(13.f, -57.f));


	
	//RIGHT 애니메이션 저장
	GetAnimator()->FindAnimation(L"SNB_RIGHT_IDLE")->Save(L"animation\\player_right_idle.anim");
	GetAnimator()->FindAnimation(L"SNB_RIGHT_RUN")->Save(L"animation\\player_right_run.anim");
	GetAnimator()->FindAnimation(L"SNB_RIGHT_JUMP")->Save(L"animation\\player_right_jump.anim");
	GetAnimator()->FindAnimation(L"SNB_RIGHT_FALLING")->Save(L"animation\\player_right_falling.anim");
	GetAnimator()->FindAnimation(L"SNB_RIGHT_LAND")->Save(L"animation\\player_right_land.anim");
	GetAnimator()->FindAnimation(L"SNB_RIGHT_CLIMBUP")->Save(L"animation\\player_right_climbup.anim");
	GetAnimator()->FindAnimation(L"SNB_RIGHT_CLIMBDOWN")->Save(L"animation\\player_right_climbdown.anim");
	GetAnimator()->FindAnimation(L"SNB_RIGHT_CLIMBSTOP")->Save(L"animation\\player_right_climbstop.anim");
	GetAnimator()->FindAnimation(L"SNB_RIGHT_SWING")->Save(L"animation\\player_right_swing.anim");


	//LEFT 애니메이션 저장
	GetAnimator()->FindAnimation(L"SNB_LEFT_IDLE")->Save(L"animation\\player_left_idle.anim");
	GetAnimator()->FindAnimation(L"SNB_LEFT_RUN")->Save(L"animation\\player_left_run.anim");
	GetAnimator()->FindAnimation(L"SNB_LEFT_JUMP")->Save(L"animation\\player_left_jump.anim");
	GetAnimator()->FindAnimation(L"SNB_LEFT_FALLING")->Save(L"animation\\player_left_falling.anim");
	GetAnimator()->FindAnimation(L"SNB_LEFT_LAND")->Save(L"animation\\player_left_land.anim");
	GetAnimator()->FindAnimation(L"SNB_LEFT_CLIMBUP")->Save(L"animation\\player_left_climbup.anim");
	GetAnimator()->FindAnimation(L"SNB_LEFT_CLIMBDOWN")->Save(L"animation\\player_left_climbdown.anim");
	GetAnimator()->FindAnimation(L"SNB_LEFT_CLIMBSTOP")->Save(L"animation\\player_left_climbstop.anim");
	GetAnimator()->FindAnimation(L"SNB_LEFT_SWING")->Save(L"animation\\player_left_swing.anim");




	GetAnimator()->Play(L"SNB_RIGHT_RUN", true);
#pragma endregion

	


	//애니메이션 오프셋 개별로 넣기
	//CAnimation* pAnim = GetAnimator()->FindAnimation(L"WALK_RIGHT");
	//for (UINT i = 0; i < pAnim->GetMaxFrame(); i++)
	//{
	//	pAnim->GetFrame(i).vOffset = Vec2(0.f, -20.f);
	//}


	CreateGravity();

	Raycast* pRay = new Raycast();
	pRay->SetName(L"PlayerRay");
	pRay->SetPos(GetPos());
	CreateObject(pRay, GROUP_TYPE::Ray);
	playerRay = pRay;

}


SPlayer::~SPlayer()
{ 
}



void SPlayer::Update()
{
	RayCasting();

	Update_State();
	Update_Move();

	Update_Animation();

	

	if (KEY_TAP(KEY::E))
		system("cls");



	//if (KEY_TAP(KEY::SPACE))
	//	CreateWire();

	if (KEY_TAP(KEY::C))
	{
		cout << GetPos().x << " " << GetPos().y << endl;
		cout << GetRigidBody()->GetSpeed() << endl;
		cout << GetRigidBody()->GetVelocity().x << endl;
		cout << GetRigidBody()->GetVelocity().y << endl;
		cout << static_cast<int>(m_eCurState) <<endl;
		cout << targetPos.x << " " << targetPos.y << endl;
		cout << onCollisionRay << endl;
		cout << playerArm->GetPos().x << " " << playerArm->GetPos().y << endl;
	}



	GetAnimator()->Update();

	if (playerHook != nullptr)
	{
		playerHook->SetDir(m_iDir);
		playerHook->SetState(m_eCurState);
	}
	playerArm->SetDir(m_iDir);
	playerArm->SetState(m_eCurState);
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

		if (KEY_HOLD(KEY::W))
			m_eCurState = PLAYER_STATE::CLIMBUP;
		if (KEY_HOLD(KEY::S))
			m_eCurState = PLAYER_STATE::CLIMBDOWN;

		ClimbJump();

		break;

	case PLAYER_STATE::CLIMBUP:

		if(KEY_AWAY(KEY::W))
			m_eCurState = PLAYER_STATE::CLIMB;
		if (KEY_HOLD(KEY::S))
			m_eCurState = PLAYER_STATE::CLIMBDOWN;

			ClimbJump();

		break;

	case PLAYER_STATE::CLIMBDOWN:
		if (KEY_AWAY(KEY::S))
			m_eCurState = PLAYER_STATE::CLIMB;
		if (KEY_HOLD(KEY::W))
			m_eCurState = PLAYER_STATE::CLIMBUP;
		ClimbJump();

		break;
	case PLAYER_STATE::SHOT:

		break;
	case PLAYER_STATE::SWING:
		if (KEY_AWAY(KEY::LBUTTON))
		{
			

			m_eCurState = PLAYER_STATE::IDLE;

		}

		break;
	case PLAYER_STATE::DAMAGED:

	break;
	case PLAYER_STATE::DEAD:

		break;
	default:
		break;
	}



	//방향전환
	if (KEY_HOLD(KEY::A) && !isClimbing && m_eCurState != PLAYER_STATE::SWING)
	{
		m_iDir = -1;

		if(PLAYER_STATE::IDLE == m_eCurState)
			m_eCurState = PLAYER_STATE::RUN;
	}
	if (KEY_HOLD(KEY::D) && !isClimbing && m_eCurState != PLAYER_STATE::SWING)
	{
		m_iDir = 1;
		if (PLAYER_STATE::IDLE == m_eCurState)
			m_eCurState = PLAYER_STATE::RUN;
	}


	//조작 없을시 Idle전환
	if (0.f == GetRigidBody()->GetSpeed() && m_bOnGround )
	{
		m_eCurState = PLAYER_STATE::IDLE;
	}


	//점프
	if (KEY_TAP(KEY::SPACE) && m_bOnGround)
	{
		m_eCurState = PLAYER_STATE::JUMP;

		if (GetRigidBody())
		{
			GetRigidBody()->SetVelocity(Vec2(GetRigidBody()->GetVelocity().x, -750.f));
		}
	}



	//와이어 해제
	if (KEY_AWAY(KEY::LBUTTON))
	{
		if(playerHook !=nullptr && playerHook->GetHookState() ==HOOK_STATE::GRAB)
		{
			//바로 삭제하지 않고 회수모션으로 전환 후 회수되면 삭제
			playerHook->SetHookState(HOOK_STATE::RETURN_WITH);
			GetGravity()->SetGround(false);
			
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
		break;
	case PLAYER_STATE::CLIMBUP:
		VirticalMove();
		break;
	case PLAYER_STATE::CLIMBDOWN:
		VirticalMove();
		break;
	case PLAYER_STATE::SHOT:

		break;
	case PLAYER_STATE::SWING:



		SwingMove();
		break;

	case PLAYER_STATE::DAMAGED:

		break;
	case PLAYER_STATE::DEAD:

		break;
	default:
		break;
	}
	



	//와이어 발사
	if (KEY_TAP(KEY::LBUTTON))
	{

		CreateHook();
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
		if (m_iDir == -1)
			GetAnimator()->Play(L"SNB_LEFT_CLIMBSTOP", true);
		else
			GetAnimator()->Play(L"SNB_RIGHT_CLIMBSTOP", true);

		break;
	case PLAYER_STATE::CLIMBUP:
		if (m_iDir == -1)
			GetAnimator()->Play(L"SNB_LEFT_CLIMBUP", true);
		else
			GetAnimator()->Play(L"SNB_RIGHT_CLIMBUP", true);
		break;	
	case PLAYER_STATE::CLIMBDOWN:
		if (m_iDir == -1)
			GetAnimator()->Play(L"SNB_LEFT_CLIMBDOWN", true);
		else
			GetAnimator()->Play(L"SNB_RIGHT_CLIMBDOWN", true);
		break;

	case PLAYER_STATE::SWING:
		if (m_iDir == -1)
			GetAnimator()->Play(L"SNB_LEFT_SWING", true);
		else
			GetAnimator()->Play(L"SNB_RIGHT_SWING", true);
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
		if (m_eCurState == PLAYER_STATE::SWING)
			return;

		Vec2 vPos = GetPos();
		Vec2 vGroundPos = pOtherObj->GetPos();
		Vec2 vGroundScale = pOtherObj->GetScale();


		if (vPos.y <= vGroundPos.y &&
			vPos.x >= vGroundPos.x &&
			vPos.x <= vGroundPos.x + vGroundScale.x)
		{
			m_eCurState = PLAYER_STATE::IDLE;
		}

		if (vPos.y <= vGroundPos.y + 75.f &&
			vPos.x <= vGroundPos.x + 2.f)
		{
			m_eCurState = PLAYER_STATE::JUMP;
			m_iDir = 1;
			SetPos(Vec2(GetPos().x, GetPos().y + 2.f));
		}
		if (vPos.y <= vGroundPos.y + 75.f &&
			vPos.x >= vGroundPos.x + vGroundScale.x -2.f)
		{
			m_eCurState = PLAYER_STATE::JUMP;
			m_iDir = -1;
			SetPos(Vec2(GetPos().x, GetPos().y + 2.f));
		}


		if (vPos.y > vGroundPos.y + 75.f &&
			vPos.x <= vGroundPos.x)
		{
			m_eCurState = PLAYER_STATE::CLIMB;
			isClimbing = true;
		}

		if (vPos.y > vGroundPos.y + 75.f &&
			vPos.x >= vGroundPos.x + vGroundScale.x)
		{
			m_eCurState = PLAYER_STATE::CLIMB;
			isClimbing = true;

		}
		
		if (m_eCurState == PLAYER_STATE::IDLE)
		{
			GetRigidBody()->SetVelocity(Vec2(0.f, 0.f));
		}

	}


}

void SPlayer::OnCollision(CCollider* _pOther)
{
	GameObject* pOtherObj = _pOther->GetObj();

	if (_pOther->GetObj()->GetName() == L"Ground")
	{
		if (m_eCurState == PLAYER_STATE::SWING)
			return;


		Vec2 vPos = GetPos();
		Vec2 vGroundPos = pOtherObj->GetPos();
		Vec2 vGroundScale = pOtherObj->GetScale();
		CRigidBody* pRigid = GetRigidBody();


		if (isClimbing)
		{
			if (vPos.y <= vGroundPos.y + 75.f &&
				vPos.x <= vGroundPos.x)
			{
				m_eCurState = PLAYER_STATE::JUMP;
				m_iDir = 1;
				SetPos(Vec2(GetPos().x-5.f, GetPos().y));
				pRigid->SetVelocity(Vec2(0.f, -700.f));

			}
			if (vPos.y <= vGroundPos.y + 75.f &&
				vPos.x >= vGroundPos.x + vGroundScale.x)
			{
				m_eCurState = PLAYER_STATE::JUMP;
				m_iDir = -1;
				SetPos(Vec2(GetPos().x + 5.f, GetPos().y));
				pRigid->SetVelocity(Vec2(0.f, -700.f));
			}
		}


		
		//나중에 점프 말고 다른 상태에서도 벽타기 하게끔 수정할 여지 있음
		if (m_eCurState == PLAYER_STATE::JUMP)
		{
			if (vPos.y <= vGroundPos.y + 75.f &&
				vPos.x <= vGroundPos.x + 2.f)
			{
				m_eCurState = PLAYER_STATE::JUMP;
				m_iDir = 1;
				SetPos(Vec2(GetPos().x, GetPos().y + 2.f));
			}
			if (vPos.y <= vGroundPos.y + 75.f &&
				vPos.x >= vGroundPos.x + vGroundScale.x - 2.f)
			{
				m_eCurState = PLAYER_STATE::JUMP;
				m_iDir = -1;
				SetPos(Vec2(GetPos().x, GetPos().y + 2.f));
			}
			

			if (vPos.y > vGroundPos.y + 75.f &&
				vPos.x <= vGroundPos.x)
			{
				isClimbing = true;
				m_eCurState = PLAYER_STATE::CLIMB;
				m_iDir = 1;

			}

			if (vPos.y > vGroundPos.y + 75.f &&
				vPos.x >= vGroundPos.x + vGroundScale.x)
			{
				isClimbing = true;
				m_iDir = -1;
				m_eCurState = PLAYER_STATE::CLIMB;
			}





			//밑에 코드는 굳이 땅에서 처리해야 하나 싶음
			if (m_eCurState == PLAYER_STATE::RUN && GetRigidBody()->GetSpeed() == 0.f)
			{
				m_eCurState = PLAYER_STATE::IDLE;
			}

			if(GetRigidBody()->GetSpeed() == 0.f)
				m_eCurState = PLAYER_STATE::IDLE;

		}


	}



}

void SPlayer::OnCollisionExit(CCollider* _pOther)
{
	GameObject* pOtherObj = _pOther->GetObj();

	if (_pOther->GetObj()->GetName() == L"Ground")
	{
		if (isClimbing)
		{
			m_bOnGround = false;
			isClimbing = false;
			CRigidBody* pRigid = GetRigidBody();

			if (pOtherObj->GetPos().y + pOtherObj->GetScale().y < GetPos().y)
			{
				
				//폴상태로 변환해야함

				pRigid->SetVelocity(Vec2(0.f, -50.f));
			}
			else if(pOtherObj->GetPos().y + 70.f > GetPos().y)
			{
				pRigid->SetVelocity(Vec2(0.f, -700.f));
				
			}

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



void SPlayer::ClimbJump()
{
	if (m_bOnGround)
		m_eCurState = PLAYER_STATE::IDLE;
	if (KEY_TAP(KEY::SPACE))
	{
		m_eCurState = PLAYER_STATE::JUMP;
		if (m_iDir == 1)
		{
			m_iDir = -1;
			GetRigidBody()->SetVelocity(Vec2(-400.f, -950.f));
			return;
		}
		else
		{
			m_iDir = 1;
			GetRigidBody()->SetVelocity(Vec2(400.f, -950.f));
			return;
		}

	}
}

void SPlayer::HorizontalMove()
{
	CRigidBody* pRigid = GetRigidBody();


	if (KEY_HOLD(KEY::A))
	{
		pRigid->SetVelocity(Vec2(-600.f, pRigid->GetVelocity().y));
	}
	if (KEY_HOLD(KEY::D))
	{
		pRigid->SetVelocity(Vec2(600.f, pRigid->GetVelocity().y));
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
		pRigid->SetVelocity(Vec2(0.f, 700.f));
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

void SPlayer::SwingMove()
{
	if (playerHook == nullptr)
		return;

	CRigidBody* pRigid = GetRigidBody();
	targetPos = playerHook->GetPos();

	if (playerHook->GetPos().y < playerArm->GetPos().y)
		GetGravity()->SetGround(true);
	else
		GetGravity()->SetGround(false);



	//플레이어 포지션이 원범위 안에 들어오면 (한번 들어오면 스윙 도중엔 절대 안나가게끔)
	//현재 velocity를 포물선 방향대로 변환
	//현재 포지션이랑 방향에 따라 velocity 수치를 조절
	//힘을 주는 방식으로 변환하자

	float distance = (targetPos - playerArm->GetPos()).Length();
	//플레이어의 현재 각도 구하기
	Vec2 dir = playerArm->GetPos() - targetPos;
	Vec2 up = Vec2(targetPos.x,targetPos.y-1) - targetPos;

	float angle;
	if (targetPos.x <playerArm->GetPos().x)
	{
		angle = dir.Angle(up);
	}
	else
	{
		float offset = 180.f - dir.Angle(up);

		angle = offset + 180.f;

	}

	
	Vec2 nextPos;
	Vec2 nextDir;

	double radian = (1.2f) * (3.14159 / 180.f);





	if (KEY_HOLD(KEY::A))
	{
		moveEnergy -= 25.f;
	}
	if (KEY_HOLD(KEY::D))
	{
		moveEnergy += 25.f;
	}


	if (canBooster)
	{
		if (KEY_HOLD(KEY::A) && KEY_HOLD(KEY::LSHIFT))
		{
			moveEnergy -= 3000.f;
			canBooster = false;
		}
		if (KEY_HOLD(KEY::D) && KEY_HOLD(KEY::LSHIFT))
		{
			moveEnergy += 3000.f;
			canBooster = false;

		}
	}
	






	if (angle > 180.f && angle < 360.f)
	{
		posEnergy = -abs(angle - 180.f);

		if (abs(posEnergy) > 90.f)
			posEnergy = -90.f;
			
	}
	else if (angle > 0.f && angle < 180.f)
	{
		posEnergy = abs(180.f - angle);

		if (abs(posEnergy) > 90.f)
			posEnergy = 90.f;

	}

	if (moveEnergy > 0.f)
	{
		if(abs(moveEnergy) > 600.f)
			moveEnergy -= fDT * 800;
		else if(abs(moveEnergy) > 300.f)
			moveEnergy -= fDT * 150;
		else
			moveEnergy -= fDT * 20;

	}
	else
	{
		if (abs(moveEnergy) > 600.f)
			moveEnergy += fDT * 800;
		else if (abs(moveEnergy) > 300.f)
			moveEnergy += fDT * 150;
		else
			moveEnergy += fDT * 20;
		
	}

	if (posEnergy > 0.f)
	{
		//posEnergy -= fDT * 50;
		moveEnergy -= fDT * posEnergy *30;
	}
	else
	{
		//posEnergy += fDT * 50;
		moveEnergy -= fDT * posEnergy *30;

	}

	if (moveEnergy > 0.f)
	{
		radian *= -1.2f;

	}
	nextPos.x = (playerArm->GetPos().x - targetPos.x) * cos(radian)
		- (playerArm->GetPos().y - targetPos.y) * sin(radian) + targetPos.x;
	nextPos.y = (playerArm->GetPos().x - targetPos.x) * sin(radian)
		+ (playerArm->GetPos().y - targetPos.y) * cos(radian) + targetPos.y;


	nextDir = nextPos - playerArm->GetPos();
	nextDir.Normalize();

	if (KEY_TAP(KEY::R))
	{
		cout << posEnergy << endl;
		cout << moveEnergy << endl;
	}
	


	pRigid->SetVelocity(nextDir * abs(moveEnergy) );

	if (distance > wireRange + 5.f)
	{
		// 1. 방향 벡터 계산
		Vec2 dir = (playerArm->GetPos() - targetPos).Normalize();

		// 2. 원하는 위치 계산 (최대 길이 내로 제한)
		Vec2 desiredPos = targetPos + dir * wireRange;

		// 3. 현재 위치와 원하는 위치의 차이 (보정 벡터)
		Vec2 correction = desiredPos - playerArm->GetPos();

		// 4. 스프링 힘 계산 (강성 계수 k 사용)
		float k = 1000.0f; // 값 조절로 탄성 조절
		Vec2 springForce = correction * k;

		// 5. 리지드바디에 힘 적용
		pRigid->AddForce(springForce);

		// 6. 접선 방향으로만 속도 유지
		Vec2 tangentDir = Vec2(-dir.y, dir.x);
		Vec2 currentVelocity = pRigid->GetVelocity();
		float tangentSpeed = currentVelocity.Dot(tangentDir);
		pRigid->SetVelocity(tangentDir * tangentSpeed * 0.7f);
	}
}


void SPlayer::CreateHook()
{
	if (playerHook != nullptr)
		return;

	m_eCurState = PLAYER_STATE::SHOT;

	canBooster = true;

	Vec2 vHookPos = playerArm->GetPos();
	//vHookPos.y -= GetScale().y / 2.f;

	playerHook = new CHook;
	playerHook->SetName(L"Hook");
	playerHook->SetPos(vHookPos);
	playerHook->SetScale(Vec2(11.f, 11.f));
	static_cast<GameObject*>(playerHook)->SetDir(Vec2(0.f, -1.f));
	playerHook->SetOwner(playerArm);

	CreateObject(playerHook, GROUP_TYPE::PROJ_PLAYER);
	//CreateObject 함수에 포지션, 방향, 스케일을 설정해주는 인자를 넣어야함


	if (CCamera::GetInst()->GetRealPos(MOUSE_POS).x < GetPos().x)
		m_iDir = -1;
	else
		m_iDir = 1;

	if (targetPos.IsZero())
	{
		playerHook->LookAt(CCamera::GetInst()->GetRealPos(MOUSE_POS));
	}
	else
	{
		playerHook->LookAt(targetPos);

		
		if (onCollisionRay->GetObj()->GetName() == L"Ground")
		{
			m_eCurState = PLAYER_STATE::SWING;
			Vec2 dir = targetPos - playerArm->GetPos();
			dir.Normalize();

			float distance = (targetPos - playerArm->GetPos()).Length();

			if (distance > wireMaxRange)
			{


				GetRigidBody()->SetVelocity(dir * 500);
				wireRange = wireMaxRange;
			}
			else
			{

				GetRigidBody()->SetVelocity(dir * 150);
				wireRange = distance;

			}

			if (targetPos.x < playerArm->GetPos().x)
				moveEnergy = -distance*1.5;
			else
				moveEnergy = distance* 1.5;


		}
		else if (playerRay->GetCollisionRay()->GetObj()->GetName() == L"Monster")
		{

		}
	}
}

void SPlayer::RayCasting()
{
	playerRay->SetPos(playerArm->GetPos());
	onCollisionRay = playerRay->GetCollisionRay();
	targetPos = playerRay->GetTargetPos();
}
