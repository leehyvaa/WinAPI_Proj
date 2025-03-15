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
#include "CGround.h"
#include "PlayerArm.h"
#include "Raycast.h"
#include "CHook.h"

SPlayer::SPlayer()
	: m_fSpeed(1000), m_iDir(1), m_iPrevDir(1), m_eCurState(PLAYER_STATE::IDLE), m_ePrevState(PLAYER_STATE::RUN), m_bOnGround(false), m_pPlayerArm(nullptr), m_pPlayerHook(nullptr), m_bClimbing(false), m_pRayHitCollider(nullptr), m_vRayHitPos(Vec2(0.f, 0.f)), m_fWireRange(-1.f), m_fWireMaxRange(700.f), m_fMoveEnergy(0.f), m_fPosEnergy(0.f), m_bCanBooster(false), m_eClimbState(PLAYER_CLIMB_STATE::NONE)
{
	// m_pTex = CResMgr::GetInst()->LoadTexture(L"PlayerTex", L"texture\\sigong.bmp");
	SetGroup(GROUP_TYPE::PLAYER);

	// 67 -13분 캐릭터 상태변환후 애니메이션 전환
	CreateCollider();
	GetCollider()->SetScale(Vec2(32.f, 102.f));
	GetCollider()->SetOffsetPos(Vec2(0.f, -(GetCollider()->GetScale().y) / 2.f));

	CreateRigidBody();
	CreateAnimator();

#pragma region 플레이어 애니메이션
	// 텍스쳐 로딩
	CTexture *pTexRight = CResMgr::GetInst()->LoadTexture(L"PlayerTex_Right", L"texture\\player\\Player_Right.bmp");
	CTexture *pTexLeft = CResMgr::GetInst()->LoadTexture(L"PlayerTex_Left", L"texture\\player\\Player_Left.bmp");

	// 애니메이션 로딩
	// GetAnimator()->LoadAnimation(L"animation\\player_right_idle.anim");

	// RIGHT 애니메이션 생성
	GetAnimator()->CreateAnimation(L"SNB_RIGHT_IDLE", pTexRight,
								   Vec2(0.f, 900.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.25f, 8, 3.f, Vec2(0.f, -57.f));
	GetAnimator()->CreateAnimation(L"SNB_RIGHT_RUN", pTexRight,
								   Vec2(0.f, 1400.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.07f, 20, 3.f, Vec2(0.f, -57.f));
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

	// LEFT 애니메이션 생성
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

	// RIGHT 애니메이션 저장
	GetAnimator()->FindAnimation(L"SNB_RIGHT_IDLE")->Save(L"animation\\player_right_idle.anim");
	GetAnimator()->FindAnimation(L"SNB_RIGHT_RUN")->Save(L"animation\\player_right_run.anim");
	GetAnimator()->FindAnimation(L"SNB_RIGHT_JUMP")->Save(L"animation\\player_right_jump.anim");
	GetAnimator()->FindAnimation(L"SNB_RIGHT_FALLING")->Save(L"animation\\player_right_falling.anim");
	GetAnimator()->FindAnimation(L"SNB_RIGHT_LAND")->Save(L"animation\\player_right_land.anim");
	GetAnimator()->FindAnimation(L"SNB_RIGHT_CLIMBUP")->Save(L"animation\\player_right_climbup.anim");
	GetAnimator()->FindAnimation(L"SNB_RIGHT_CLIMBDOWN")->Save(L"animation\\player_right_climbdown.anim");
	GetAnimator()->FindAnimation(L"SNB_RIGHT_CLIMBSTOP")->Save(L"animation\\player_right_climbstop.anim");
	GetAnimator()->FindAnimation(L"SNB_RIGHT_SWING")->Save(L"animation\\player_right_swing.anim");

	// LEFT 애니메이션 저장
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

	// 애니메이션 오프셋 개별로 넣기
	// CAnimation* pAnim = GetAnimator()->FindAnimation(L"WALK_RIGHT");
	// for (UINT i = 0; i < pAnim->GetMaxFrame(); i++)
	//{
	//	pAnim->GetFrame(i).vOffset = Vec2(0.f, -20.f);
	// }

	CreateGravity();

	Raycast *pRay = new Raycast();
	pRay->SetName(L"PlayerRay");
	pRay->SetPos(GetPos());
	CreateObject(pRay, GROUP_TYPE::Ray);
	m_pPlayerRay = pRay;
    pRay->SetOwner(this);
	pRay->SetMaxWireRange(m_fWireMaxRange);
    pRay->SetMaxMonsterSearchRange(m_fWireMaxRange);
    
	Enter_State(m_eCurState);
}

SPlayer::~SPlayer()
{
}

void SPlayer::Update()
{
	RayCasting();

	Update_State();

	Update_Animation();
	ClimbAnimationUpdate();

	if (KEY_TAP(KEY::E))
		system("cls");

	// if (KEY_TAP(KEY::SPACE))
	//	CreateWire();

	if (KEY_TAP(KEY::C))
	{
		cout << GetPos().x << " " << GetPos().y << endl;
		cout << GetRigidBody()->GetSpeed() << endl;
		cout << GetRigidBody()->GetVelocity().x << endl;
		cout << GetRigidBody()->GetVelocity().y << endl;
		cout << static_cast<int>(m_eCurState) << endl;
		cout << m_vRayHitPos.x << " " << m_vRayHitPos.y << endl;
		cout << m_pRayHitCollider << endl;
		cout << m_pPlayerArm->GetPos().x << " " << m_pPlayerArm->GetPos().y << endl;
	}

	GetAnimator()->Update();

	if (m_pPlayerHook != nullptr)
	{
		m_pPlayerHook->SetDir(m_iDir);
		m_pPlayerHook->SetState(m_eCurState);
	}
	m_pPlayerArm->SetDir(m_iDir);
	m_pPlayerArm->SetState(m_eCurState);
	m_iPrevDir = m_iDir;
}

void SPlayer::Render(HDC _dc)
{
	// Vec2 vPos(GetPos().x, GetPos().y);
	// Vec2 vScale(GetScale().x, GetScale().y);

	/*
	int iWidth = (int)m_pTex->Width();
	int iHeight = (int)m_pTex->Height();*/

	/*TransparentBlt(_dc
		, int(vPos.x - (float)(iWidth / 2))
		, int(vPos.y - (float)(iHeight / 2))
		, iWidth, iHeight
		, m_pTex->GetDC()
		, 0, 0, iWidth, iHeight, RGB(255, 0, 255));*/

	// CTexture* pTex = CResMgr::GetInst()->LoadTexture(L"PlayerTex", L"texture\\sigong.bmp");

	// 알파블렌드를 사용한 랜더링
	// CTexture* pTex = CResMgr::GetInst()->LoadTexture(L"PlayerTex1", L"texture\\Sail_Fish.bmp");

	// Vec2 vPos = GetPos();
	// vPos = CCamera::GetInst()->GetRenderPos(vPos);
	// float width = (float)pTex->Width();
	// float height = (float)pTex->Height();

	// BLENDFUNCTION bf = {};
	// bf.BlendOp = AC_SRC_OVER;
	// bf.BlendFlags = 0;
	// bf.AlphaFormat = AC_SRC_ALPHA;
	// bf.SourceConstantAlpha = 127; //전역적으로 적용되는 알파

	// AlphaBlend(_dc
	//	, int(vPos.x - width / 2.f)
	//	, int(vPos.y - height / 2.f)
	//	, int(width), int(height)
	//	, pTex->GetDC()
	//	, 0, 0, int(width), int(height)
	//	, bf);

	Component_Render(_dc);
}

void SPlayer::Enter_State(PLAYER_STATE _eState)
{
	switch (_eState)
	{
	case PLAYER_STATE::IDLE:
	    // 이거 지워야 자연스러울지도
		GetRigidBody()->SetVelocity(Vec2(0.f, 0.f));
		break;
	case PLAYER_STATE::RUN:
	    GetRigidBody()->SetMaxSpeed(Vec2(850.f, 1000.f));
		break;
	case PLAYER_STATE::ATTACK:
		break;
	case PLAYER_STATE::JUMP:
	    GetRigidBody()->SetMaxSpeed(Vec2(780.f, 1000.f));
	    if (m_bOnGround)
	        GetRigidBody()->AddForce(Vec2(0.f, -10000.f));
        else
            GetRigidBody()->AddForce(Vec2(0.f, -5000.f));
		SetPos(Vec2(GetPos().x, GetPos().y - 20.f));
		GetGravity()->SetApplyGravity(true);
		SetOnGround(false);
		break;
	case PLAYER_STATE::FALL:
	    GetRigidBody()->SetMaxSpeed(Vec2(780.f, 1000.f));
	    break;
	case PLAYER_STATE::CLIMB:
		GetRigidBody()->SetVelocity(Vec2(0.f, 0.f));
		GetGravity()->SetApplyGravity(false);
		break;
	case PLAYER_STATE::SHOT:
		break;
	case PLAYER_STATE::SWING:
	    GetGravity()->SetApplyGravity(false);
	    GetRigidBody()->SetMaxSpeed(Vec2(1000.f, 1000.f));

		break;
	case PLAYER_STATE::DAMAGED:
		break;
	case PLAYER_STATE::DEAD:
		break;
	default:
		break;
	}
}

void SPlayer::Update_State()
{
	PLAYER_STATE eNextState = m_eCurState;

	switch (m_eCurState)
	{
	case PLAYER_STATE::IDLE:
		HorizontalMove();
		if (KEY_TAP(KEY::SPACE) && m_bOnGround)
			eNextState = PLAYER_STATE::JUMP;
		else if (KEY_HOLD(KEY::A) || KEY_HOLD(KEY::D))
			eNextState = PLAYER_STATE::RUN;
	    if (!m_bOnGround && GetRigidBody()->GetVelocity().y > 0.f)
	        eNextState = PLAYER_STATE::FALL;
		// 정지 상태에서의 방향전환
		if (KEY_HOLD(KEY::A) && !m_bClimbing && m_eCurState != PLAYER_STATE::SWING)
		{
		}
		if (KEY_HOLD(KEY::D) && !m_bClimbing && m_eCurState != PLAYER_STATE::SWING)
		{
		}
		break;
	case PLAYER_STATE::RUN:
		HorizontalMove();
		// 조작 없을시 Idle전환
		if (0.f == GetRigidBody()->GetSpeed() && m_bOnGround)
			eNextState = PLAYER_STATE::IDLE;
		if (KEY_TAP(KEY::SPACE) && m_bOnGround)
			eNextState = PLAYER_STATE::JUMP;
	    if (!m_bOnGround && GetRigidBody()->GetVelocity().y > 0.f)
	        eNextState = PLAYER_STATE::FALL;
		break;
	case PLAYER_STATE::ATTACK:
		break;
	case PLAYER_STATE::JUMP:
		HorizontalMove();
	    if (GetRigidBody()->GetVelocity().y > 0.f)
	        eNextState = PLAYER_STATE::FALL;
		if (m_bOnGround && GetRigidBody()->GetVelocity().y >= 0.f)
			eNextState = PLAYER_STATE::IDLE;
		if (IsWallClimbing())
			eNextState = PLAYER_STATE::CLIMB;
		break;
	case PLAYER_STATE::FALL:
	    HorizontalMove();
	    if (m_bOnGround)
	        eNextState = PLAYER_STATE::IDLE;
	    if (IsWallClimbing())
	        eNextState = PLAYER_STATE::CLIMB;
	    break;
	case PLAYER_STATE::CLIMB:
		VirticalMove();
	    if (!m_bClimbing)
	    {
	        if (m_eClimbState == PLAYER_CLIMB_STATE::UP)
	            eNextState = PLAYER_STATE::JUMP;
	        else
	            eNextState = PLAYER_STATE::FALL;
	    }
		if (KEY_TAP(KEY::SPACE))
		{
			eNextState = PLAYER_STATE::JUMP;
			WallKickJump();
		}
		break;
	case PLAYER_STATE::SHOT:
	    if (m_pRayHitCollider != nullptr && m_pRayHitCollider->GetObj()->GetGroup() == GROUP_TYPE::GROUND)
	        eNextState = PLAYER_STATE::SWING;
		break;
	case PLAYER_STATE::SWING:
		SwingMove();
		if (KEY_AWAY(KEY::LBUTTON))
			eNextState = PLAYER_STATE::FALL;
		break;
	case PLAYER_STATE::DAMAGED:
		break;
	case PLAYER_STATE::DEAD:
		break;
	default:
		break;
	}

	// 와이어 발사
	if (KEY_TAP(KEY::LBUTTON))
	{
		CreateHook();
	    eNextState = PLAYER_STATE::SHOT;
	}

	// 와이어 해제
	if (KEY_AWAY(KEY::LBUTTON))
	{
		if (m_pPlayerHook != nullptr && m_pPlayerHook->GetHookState() == HOOK_STATE::GRAB)
		{
			// 바로 삭제하지 않고 회수모션으로 전환 후 회수되면 삭제
			m_pPlayerHook->SetHookState(HOOK_STATE::RETURN_WITH);
		}
	}

	if (eNextState != m_eCurState)
	{
		Exit_State(m_eCurState); // 기존 상태 정리
		Enter_State(eNextState); // 새 상태 초기화
		m_ePrevState = m_eCurState;
		m_eCurState = eNextState;
	}
}

void SPlayer::Exit_State(PLAYER_STATE _eState)
{
	switch (_eState)
	{
	case PLAYER_STATE::IDLE:
		break;
	case PLAYER_STATE::RUN:
		break;
	case PLAYER_STATE::ATTACK:
		break;
	case PLAYER_STATE::JUMP:
	    break;
	case PLAYER_STATE::FALL:
		break;
	case PLAYER_STATE::CLIMB:
		m_eClimbState = PLAYER_CLIMB_STATE::NONE;
		break;
	case PLAYER_STATE::SHOT:
		break;
	case PLAYER_STATE::SWING:
	    GetGravity()->SetApplyGravity(true);

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
		if (m_iDir == -1)
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
		if (m_iDir == -1)
			GetAnimator()->Play(L"SNB_LEFT_JUMP", true);
		else
			GetAnimator()->Play(L"SNB_RIGHT_JUMP", true);
		break;
	case PLAYER_STATE::FALL:
	    if (m_iDir == -1)
	        GetAnimator()->Play(L"SNB_LEFT_JUMP", true);
	    else
	        GetAnimator()->Play(L"SNB_RIGHT_JUMP", true);
	    break;
	case PLAYER_STATE::CLIMB:
		if (m_iDir == -1)
			GetAnimator()->Play(L"SNB_LEFT_CLIMBSTOP", true);
		else
			GetAnimator()->Play(L"SNB_RIGHT_CLIMBSTOP", true);
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

void SPlayer::OnCollisionEnter(CCollider *_pOther)
{
	GameObject *pOtherObj = _pOther->GetObj();

	if (pOtherObj->GetGroup() == GROUP_TYPE::GROUND)
	{
		// 현재 SWING 상태라면 충돌 처리 무시
		if (m_eCurState == PLAYER_STATE::SWING)
			return;
        if (static_cast<CGround*>(pOtherObj)->GetCollideType() == TILE_COLLIDE_TYPE::SLOPE_LEFT
            || static_cast<CGround*>(pOtherObj)->GetCollideType() == TILE_COLLIDE_TYPE::SLOPE_RIGHT)
        {
            //GetRigidBody()->SetVelocity(Vec2(0.f, GetRigidBody()->GetVelocity().y));
        }
	}
}

void SPlayer::OnCollision(CCollider *_pOther)
{
	GameObject *pOtherObj = _pOther->GetObj();



	if (_pOther->GetObj()->GetGroup() == GROUP_TYPE::GROUND)
	{
		// 현재 스윙 상태면 충돌 처리 무시
		if (m_eCurState == PLAYER_STATE::SWING)
			return;
	}
}

void SPlayer::OnCollisionExit(CCollider *_pOther)
{
	GameObject *pOtherObj = _pOther->GetObj();


}



void SPlayer::ClimbAnimationUpdate()
{
	// 클라임 상태일 때에만 애니메이션 적용
	if (m_eCurState == PLAYER_STATE::CLIMB)
	{
		switch (m_eClimbState)
		{
		case PLAYER_CLIMB_STATE::NONE:
			if (m_iDir == -1)
				GetAnimator()->Play(L"SNB_LEFT_CLIMBSTOP", true);
			else
				GetAnimator()->Play(L"SNB_RIGHT_CLIMBSTOP", true);
			break;
		case PLAYER_CLIMB_STATE::UP:
			if (m_iDir == -1)
				GetAnimator()->Play(L"SNB_LEFT_CLIMBUP", true);
			else
				GetAnimator()->Play(L"SNB_RIGHT_CLIMBUP", true);
			break;
		case PLAYER_CLIMB_STATE::DOWN:
			if (m_iDir == -1)
				GetAnimator()->Play(L"SNB_LEFT_CLIMBDOWN", true);
			else
				GetAnimator()->Play(L"SNB_RIGHT_CLIMBDOWN", true);
			break;
		default:
			break;
		}
		m_ePrevClimbState = m_eClimbState;
	}
}

// 매달린 상태에서 점프 (반대방향으로 점프 혹은 위로 뛰어서 다시 위의 벽 잡기)
void SPlayer::WallKickJump()
{
	if (m_iDir == 1)
	{
	    if (KEY_HOLD(KEY::D))
	    {
	        // 이 부분을 매끄럽게 처리하려면 점프 후에 일정 시간동안 climb로 안넘어가게 하거나 점프를 Addforce로 수정
	        //GetRigidBody()->SetVelocity(Vec2(-1000.f, -2000.f));
	        SetPos(Vec2(GetPos().x-20.f, GetPos().y-50.f));
	    }
        else
        {
            m_iDir = -1;
            GetRigidBody()->AddForce(Vec2(-4000.f, -9500.f));
            //GetRigidBody()->SetVelocity(Vec2(-400.f, -950.f));
        }
	}
	else
	{
	    if (KEY_HOLD(KEY::A))
	    {
	        SetPos(Vec2(GetPos().x+20.f, GetPos().y-50.f));
	    }
	    else
	    {
	        m_iDir = -1;
	        GetRigidBody()->AddForce(Vec2(-4000.f, -9500.f));
	        //GetRigidBody()->SetVelocity(Vec2(400.f, -950.f));
	    }
	}
}

// 좌우 달리기
void SPlayer::HorizontalMove()
{
    const float MOVE_FORCE = 1000.f;
	CRigidBody *pRigid = GetRigidBody();

	if (KEY_HOLD(KEY::A))
	{
		m_iDir = -1;
	    //pRigid->SetVelocity(Vec2(-600.f, pRigid->GetVelocity().y));
	    pRigid->AddForce(Vec2(-MOVE_FORCE,0.f));
	}
	if (KEY_HOLD(KEY::D))
	{
		m_iDir = 1;
	    //pRigid->SetVelocity(Vec2(600.f, pRigid->GetVelocity().y));
	    pRigid->AddForce(Vec2(MOVE_FORCE,0.f));
	}

	if (KEY_AWAY(KEY::A))
		pRigid->SetVelocity(Vec2(0.f, pRigid->GetVelocity().y));
	if (KEY_AWAY(KEY::D))
		pRigid->SetVelocity(Vec2(0.f, pRigid->GetVelocity().y));
}

// 벽에 매달린 상태에서 수직이동
void SPlayer::VirticalMove()
{
	CRigidBody *pRigid = GetRigidBody();
    pRigid->SetVelocity(Vec2(0.f, pRigid->GetVelocity().y));
	if (KEY_HOLD(KEY::W))
	{
		pRigid->SetVelocity(Vec2(0.f, -500.f));
		m_eClimbState = PLAYER_CLIMB_STATE::UP;
	}
	else if (KEY_HOLD(KEY::S))
	{
		pRigid->SetVelocity(Vec2(0.f, 700.f));
		m_eClimbState = PLAYER_CLIMB_STATE::DOWN;
	}
	else
	{
		m_eClimbState = PLAYER_CLIMB_STATE::NONE;
	}

	if (KEY_AWAY(KEY::W))
		pRigid->SetVelocity(Vec2(pRigid->GetVelocity().x, 0.f));
	if (KEY_AWAY(KEY::S))
		pRigid->SetVelocity(Vec2(pRigid->GetVelocity().x, 0.f));
}

// 와이어가 팽팽한 상태인지 판별
bool SPlayer::IsWireTaut()
{
    Vec2 hookPos = m_pPlayerHook->GetPos();
    
    // 거리 기반 판별
    float currentDistance = (hookPos - m_pPlayerArm->GetPos()).Length();
    float distanceRatio = currentDistance / m_fWireRange;
    bool isNearPerimeter = (distanceRatio > 0.95f); // 95% 이상이면 최외각 근처로 간주

    // 다음 위치 예측
    Vec2 currentVelocity = GetRigidBody()->GetVelocity();
    Vec2 nextPredictedPos = m_pPlayerArm->GetPos() + currentVelocity * fDT;
    float nextPredictedDistance = (hookPos - nextPredictedPos).Length();

    // 다음 위치가 와이어 범위를 초과할 것으로 예상되는지 확인
    bool willExceedRange = (nextPredictedDistance > m_fWireRange);

    // 진행 방향과 갈고리 방향의 관계 확인 (갈고리 바깥쪽으로 향하는지)
    Vec2 wireDir = m_pPlayerArm->GetPos() - hookPos;
    wireDir.Normalize();
    Vec2 velocityDir = currentVelocity;
    velocityDir.Normalize();
    float outwardMovement = wireDir.Dot(velocityDir); // 양수면 바깥쪽으로 움직임

    // 와이어가 팽팽한 상태인지 최종 판별
    bool isWireTaut = (isNearPerimeter && (willExceedRange || outwardMovement > 0));

    // 상태 유지를 위한 wasWireTaut 사용
    static bool wasWireTaut = false;
    if (!isWireTaut && wasWireTaut && distanceRatio > 0.9f)
    {
        // 아직 90% 이상 거리면 팽팽한 상태 유지 (경계 부근 떨림 방지)
        isWireTaut = true;
    }
    wasWireTaut = isWireTaut;
    return isWireTaut;
}


// Swing 상태에서 플레이어의 속도를 적용
void SPlayer::ApplySwingVelocity()
{
    Vec2 hookPos = m_pPlayerHook->GetPos();

    // 원심력이 존재해서 플레이어 이동
    // (3.14159 / 180.f)는 degree를 radian으로 변환하는 공식
    // 매 프레임마다 갈고리를 중심으로 1.2도 회전한 위치를 목표 방향으로 설정
    double radian = (0.6f) * (3.14159 / 180.f);
    if (m_fMoveEnergy > 0.f)
        radian *= -1.f;

    // 갈고리에서 플레이어 방향 계산
    Vec2 dirToPlayer = m_pPlayerArm->GetPos() - hookPos;
    dirToPlayer.Normalize();

    // m_fWireRange 거리에 있는 점 계산,이 위치는 부스터 쓸때만 써야할듯?
    Vec2 curMaxPos = hookPos + dirToPlayer * m_fWireRange;

    // 이 점을 회전시켜 다음 위치 계산
    Vec2 nextPos;
    // 이 위치는 부스터 쓸때만 써야할듯?, 갈고리 박고 4초간 덜덜 떨리는 현상 발생
    //nextPos.x = (curMaxPos.x - hookPos.x) * cos(radian) - (curMaxPos.y - hookPos.y) * sin(radian) + hookPos.x;
    //nextPos.y = (curMaxPos.x - hookPos.x) * sin(radian) + (curMaxPos.y - hookPos.y) * cos(radian) + hookPos.y;
    nextPos.x = (m_pPlayerArm->GetPos().x - hookPos.x) * cos(radian) - (m_pPlayerArm->GetPos().y - hookPos.y) *
        sin(radian) + hookPos.x;
    nextPos.y = (m_pPlayerArm->GetPos().x - hookPos.x) * sin(radian) + (m_pPlayerArm->GetPos().y - hookPos.y) *
        cos(radian) + hookPos.y;

    // 플레이어와 갈고리 사이의 각도와 현재 받는 힘에 따라 플레이어가 이동할 다음 위치 계산
    Vec2 nextDir = nextPos - m_pPlayerArm->GetPos();
    nextDir.Normalize();


    // 계산한 방향 대로 플레이어의 속도 바꾸기
    CRigidBody* pRigid = GetRigidBody();
    //pRigid->AddForce(nextDir * abs(m_fMoveEnergy) * 10.f);
    pRigid->SetVelocity(nextDir * abs(m_fMoveEnergy));


    //갈고리와 플레이어 사이의 거리가 와이어 거리를 넘어가지 않도록 제한
    if (m_fHookDistance > m_fWireRange)
    {
        // 현재 위치와 원하는 위치의 차이 (보정 벡터)
        Vec2 correction = curMaxPos - m_pPlayerArm->GetPos();

        // 스프링 힘 계산 (강성계수 k 값 조절로 탄성 조절)
        float k = 1000.0f; 
        Vec2 springForce = correction * k;

        // 리지드바디에 힘 적용
        pRigid->AddForce(springForce);

        // 접선 방향으로만 속도 유지
        Vec2 tangentDir = Vec2(-dirToPlayer.y, dirToPlayer.x);
        Vec2 currentVelocity = pRigid->GetVelocity();
        float tangentSpeed = currentVelocity.Dot(tangentDir);
        pRigid->SetVelocity(tangentDir * tangentSpeed * 0.7f );
    }
}



// Swing 상태에서 적절한 물리 효과를 세팅
void SPlayer::UpdateSwingEnergy()
{
    // 갈고리 위치와 플레이어 사이의 거리 저장
    Vec2 hookPos = m_pPlayerHook->GetPos();
    m_fHookDistance = (hookPos - m_pPlayerArm->GetPos()).Length();

    // 갈고리와 플레이어의 현재 각도 구하기
    Vec2 wireDir = m_pPlayerArm->GetPos() - hookPos;
    Vec2 up = Vec2(hookPos.x, hookPos.y - 1) - hookPos;
    float angle;
    
    // 갈고리가 플레이어의 좌우 중 어디에 있냐에 따라 각도 offset 조절
    if (hookPos.x < m_pPlayerArm->GetPos().x)
        angle = wireDir.Angle(up);
    else
    {
        float offset = 180.f - wireDir.Angle(up);
        angle = offset + 180.f;
    }

    
    // m_fPosEnergy 생성
    m_fPosEnergy = 0.f;
    if (angle > 180.f && angle < 360.f) // 플레이어가 갈고리 기준 왼쪽에 있을 때
    {
        m_fPosEnergy = -abs(angle - 180.f);
        // 위치 에너지가 75보다 크면 75으로 적용
        if (abs(m_fPosEnergy) > 75.f)
            m_fPosEnergy = -75.f;
    }
    else if (angle > 0.f && angle < 180.f) // 플레이어가 갈고리 기준 오른쪽에 있을 때
    {
        m_fPosEnergy = abs(180.f - angle);
        if (abs(m_fPosEnergy) > 75.f)
            m_fPosEnergy = 75.f;
    }
        
    if (hookPos.y > m_pPlayerArm->GetPos().y) // 플레이어가 갈고리보다 위에 있으면 m_fPosEnergy = 0
        m_fPosEnergy = 0.f;
	

    // MoveEnergy 소모, 운동 에너지가 양수면 오른쪽에서 좌로 이동
    if (m_fMoveEnergy > 0.f)
    {
        // if (abs(m_fMoveEnergy) > 600.f)
        //     m_fMoveEnergy -= fDT * 800;
        // else if (abs(m_fMoveEnergy) > 300.f)
        //     m_fMoveEnergy -= fDT * 150;
        // else
            m_fMoveEnergy -= fDT * 65;

    }
    else
    {
        // if (abs(m_fMoveEnergy) > 600.f)
        //     m_fMoveEnergy += fDT * 800;
        // else if (abs(m_fMoveEnergy) > 300.f)
        //     m_fMoveEnergy += fDT * 150;
        // else
            m_fMoveEnergy += fDT * 65;
 
    }

    // 위치 에너지를 운동 에너지로 변환
    if (m_fPosEnergy > 0.f)
        m_fMoveEnergy -= fDT * m_fPosEnergy * 47;
    else
        m_fMoveEnergy -= fDT * m_fPosEnergy * 47;
}



/* 플레이어가 와이어에 매달린 상태에서 와이어 이동
 * 플레이어의 위치가 원 내부에 있을 경우엔 자유낙하 하고
 * 원의 최외각 지역에 있으면 와이어 이동 */
void SPlayer::SwingMove()
{
    // 갈고리가 생성되지 않았으면 리턴
    if (m_pPlayerHook == nullptr)
        return;
    
    // 이전 에너지 상태 저장
    float prevMoveEnergy = m_fMoveEnergy;
    Vec2 hookPos = m_pPlayerHook->GetPos();
    
    // MoveEnergy와 PosEnergy 계산
    UpdateSwingEnergy();

    // 부스터
    if (m_bCanBooster)
    {
        if (KEY_HOLD(KEY::A) && KEY_HOLD(KEY::LSHIFT))
        {
            m_fMoveEnergy -= 2600.f;
            m_bCanBooster = false;
        }
        if (KEY_HOLD(KEY::D) && KEY_HOLD(KEY::LSHIFT))
        {
            m_fMoveEnergy += 2600.f;
            m_bCanBooster = false;
        }
    }

    
    // 와이어가 팽팽한 상태(플레이어가 원의 최외곽에 있는지)
    if (IsWireTaut())
    {
        // 기본적으로 중력 미적용
        GetGravity()->SetApplyGravity(false);

        // 플레이어가 갈고리보다 위에 있는 경우
        if (hookPos.y > m_pPlayerArm->GetPos().y)
        {
            // MoveEnergy의 힘이 500보다 작으면 원심력이 부족하다 판단하고 중력 적용
            if ((prevMoveEnergy > 500 && m_fMoveEnergy <= 500) || 
                (prevMoveEnergy < -500 && m_fMoveEnergy >= -500) ||
                (abs(m_fMoveEnergy) < 500.f)) // 작은 허용 오차 추가
            {
                // 중력 적용
                GetGravity()->SetApplyGravity(true);
            }

            // 스윙 도중에 벽 옆면에 부딪힌 상황
            if (GetRigidBody()->GetVelocity().x ==0.f)
            {
                GetGravity()->SetApplyGravity(true);
                m_fMoveEnergy = 0.f;
                if (GetRigidBody()->GetVelocity().y < 0.f)
                    GetRigidBody()->SetVelocityY(0.f);
            }
        }
        else // 플레이어가 갈고리보다 아래에 있어서 힘을 줄 수 있는 경우
        {
            // 스윙 상태에서 좌우 진자 이동을 위한 힘 추가
            if (KEY_HOLD(KEY::A))
                m_fMoveEnergy -= 20.f;
            if (KEY_HOLD(KEY::D))
                m_fMoveEnergy += 20.f;
        }

        // 중력이 미적용 중이면 스윙 속도 적용
        if (!GetGravity()->IsApplyGravity())
        {
            ApplySwingVelocity();
        }
    }
    else // 와이어가 다 늘어나지 않았으면 자유낙하
    {
        // 중력 적용
        GetGravity()->SetApplyGravity(true);
    }
}


// 마우스 방향으로 와이어 발사
void SPlayer::CreateHook()
{
	if (m_pPlayerHook != nullptr)
		return;

	m_bCanBooster = true;

	Vec2 vHookPos = m_pPlayerArm->GetPos();
	// vHookPos.y -= GetScale().y / 2.f;

    // 와이어 생성
	m_pPlayerHook = new CHook;
	m_pPlayerHook->SetName(L"Hook");
	m_pPlayerHook->SetPos(vHookPos);
	m_pPlayerHook->SetScale(Vec2(11.f, 11.f));
	static_cast<GameObject *>(m_pPlayerHook)->SetDir(Vec2(0.f, -1.f));
	m_pPlayerHook->SetParent(m_pPlayerArm);

	CreateObject(m_pPlayerHook, GROUP_TYPE::HOOK);
	// CreateObject 함수에 포지션, 방향, 스케일을 설정해주는 인자를 넣어야함

    // 와이어 발사 방향으로 플레이어 바라보기
	if (CCamera::GetInst()->GetRealPos(MOUSE_POS).x < GetPos().x)
		m_iDir = -1;
	else
		m_iDir = 1;


    // 와이어가 아무것도 맞추지 못했을 경우
	if (m_vRayHitPos.IsZero())
	{
		m_pPlayerHook->LookAt(CCamera::GetInst()->GetRealPos(MOUSE_POS));
	}
	else // 와이어가 오브젝트에 닿았을 경우
	{
		m_pPlayerHook->LookAt(m_vRayHitPos);

	    // Ray에 충돌한 물체가 GROUND일 경우
		if (m_pRayHitCollider->GetObj()->GetGroup() == GROUP_TYPE::GROUND)
		{
			Vec2 dir = m_vRayHitPos - m_pPlayerArm->GetPos();
			dir.Normalize();

			float distance = (m_vRayHitPos - m_pPlayerArm->GetPos()).Length();

			if (distance > m_fWireMaxRange)
			{
				GetRigidBody()->SetVelocity(dir * 500);
				m_fWireRange = m_fWireMaxRange;
			}
			else
			{
				m_fWireRange = distance;
			}

			if (m_vRayHitPos.x < m_pPlayerArm->GetPos().x)
				m_fMoveEnergy = -distance * 1.5;
			else
				m_fMoveEnergy = distance * 1.5;
		}
		else if (m_pPlayerRay->GetCollisionRay()->GetObj()->GetGroup() == GROUP_TYPE::MONSTER)
		{
		}
	}
}

// RayCast를 진행 후 Ray와 충돌한 충돌체를 onCollisionRay에 저장하고 충돌 지점을 targetPos에 저장 
void SPlayer::RayCasting()
{
	m_pPlayerRay->SetPos(m_pPlayerArm->GetPos());
	m_pRayHitCollider = m_pPlayerRay->GetCollisionRay();
	m_vRayHitPos = m_pPlayerRay->GetTargetPos();
}
