#include "pch.h"
#include "PlayerArm.h"
#include "SPlayer.h"
#include "CResMgr.h"
#include "CAnimator.h"
#include "CAnimation.h"
#include "CCollider.h"
#include "CHook.h"

PlayerArm::PlayerArm()
	: m_ePrevClimbState(PLAYER_CLIMB_STATE::NONE)
{
	SetGroup(GROUP_TYPE::PLAYER_ARM);
	CreateAnimator();
	CreateCollider();
	GetCollider()->SetOffsetPos(Vec2(0.f, 0.f));
	GetCollider()->SetScale(Vec2(10.f, 10.f));
    

#pragma region 플레이어 Arm 애니메이션
	// 텍스쳐 로딩
	CTexture *pArmTexRight = CResMgr::GetInst()->LoadTexture(L"ArmTex_Right", L"texture\\player\\Arm_Right.png");

	// 애니메이션 로딩
	// GetAnimator()->LoadAnimation(L"animation\\playerArm_right_idle.anim");

	// RIGHT 애니메이션 생성
	GetAnimator()->CreateAnimation(L"SNB_ARM_RIGHT_IDLE", pArmTexRight,
								   Vec2(0.f, 900.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.25f, 8, 0.85f, Vec2(-20.f, 28.f));
	GetAnimator()->CreateAnimation(L"SNB_ARM_RIGHT_RUN", pArmTexRight,
								   Vec2(0.f, 1400.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.07f, 20, 0.85f, Vec2(-18.f, 18.f));
	GetAnimator()->CreateAnimation(L"SNB_ARM_RIGHT_JUMP", pArmTexRight,
								   Vec2(0.f, 1000.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.25f, 6, 0.85f, Vec2(-20.f, 28.f));
	GetAnimator()->CreateAnimation(L"SNB_ARM_RIGHT_FALLING", pArmTexRight,
								   Vec2(0.f, 600.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.25f, 3, 0.85f, Vec2(-39.f, 33.f));
	GetAnimator()->CreateAnimation(L"SNB_ARM_RIGHT_LAND", pArmTexRight,
								   Vec2(0.f, 1100.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.25f, 3, 0.85f, Vec2(-20.f, 28.f));
	GetAnimator()->CreateAnimation(L"SNB_ARM_RIGHT_CLIMBUP", pArmTexRight,
								   Vec2(0.f, 2800.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.06f, 10, 0.85f, Vec2(-5.f, 13.f));
	GetAnimator()->CreateAnimation(L"SNB_ARM_RIGHT_CLIMBDOWN", pArmTexRight,
								   Vec2(0.f, 2700.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.06f, 7, 0.85f, Vec2(-5.f, 18.f));
	GetAnimator()->CreateAnimation(L"SNB_ARM_RIGHT_CLIMBSTOP", pArmTexRight,
								   Vec2(0.f, 2900.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.2f, 11, 0.85f, Vec2(0.f, 3.f));
    GetAnimator()->CreateAnimation(L"SNB_ARM_RIGHT_SHOT", pArmTexRight,
                               Vec2(0.f, 2300.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.2f, 3, 0.85f, Vec2(0.f, 3.f));
	GetAnimator()->CreateAnimation(L"SNB_ARM_RIGHT_SWING", pArmTexRight,
								   Vec2(0.f, 2300.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.2f, 3, 0.85f, Vec2(0.f, -10.f));
    GetAnimator()->CreateAnimation(L"SNB_ARM_RIGHT_EXC_BACK", pArmTexRight,
                                   Vec2(0.f, 800.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.2f, 8, 0.6f, Vec2(0.f, 0.f));

    
	// RIGHT 애니메이션 저장
	GetAnimator()->FindAnimation(L"SNB_ARM_RIGHT_IDLE")->Save(L"animation\\playerArm_right_idle.anim");
	GetAnimator()->FindAnimation(L"SNB_ARM_RIGHT_RUN")->Save(L"animation\\playerArm_right_run.anim");
	GetAnimator()->FindAnimation(L"SNB_ARM_RIGHT_JUMP")->Save(L"animation\\playerArm_right_jump.anim");
	GetAnimator()->FindAnimation(L"SNB_ARM_RIGHT_FALLING")->Save(L"animation\\playerArm_right_falling.anim");
	GetAnimator()->FindAnimation(L"SNB_ARM_RIGHT_LAND")->Save(L"animation\\playerArm_right_land.anim");
	GetAnimator()->FindAnimation(L"SNB_ARM_RIGHT_CLIMBUP")->Save(L"animation\\playerArm_right_climbup.anim");
	GetAnimator()->FindAnimation(L"SNB_ARM_RIGHT_CLIMBDOWN")->Save(L"animation\\playerArm_right_climbdown.anim");
	GetAnimator()->FindAnimation(L"SNB_ARM_RIGHT_CLIMBSTOP")->Save(L"animation\\playerArm_right_climbstop.anim");
	GetAnimator()->FindAnimation(L"SNB_ARM_RIGHT_SWING")->Save(L"animation\\playerArm_right_swing.anim");
	GetAnimator()->FindAnimation(L"SNB_ARM_RIGHT_SHOT")->Save(L"animation\\playerArm_right_shot.anim");
	GetAnimator()->FindAnimation(L"SNB_ARM_RIGHT_EXC_BACK")->Save(L"animation\\playerArm_right_exc_back.anim");

	GetAnimator()->Play(L"SNB_ARM_RIGHT_RUN", true);
#pragma endregion
}

PlayerArm::~PlayerArm()
{
}

void PlayerArm::Update()
{
    SPlayer* player = static_cast<SPlayer*>(GetParent());
    if (player)
    {
        m_bIsFacingRight = player->GetIsFacingRight();
        m_eCurState = player->GetState();
    }

    
   
    
	Update_ClimbAnimation();
	Update_Animation();
	GetAnimator()->Update();

	m_bIsFacingRightPrev = m_bIsFacingRight;
	m_ePrevState = m_eCurState;
}

void PlayerArm::RenderD2D(ID2D1RenderTarget* _pRenderTarget)
{
	if (!_pRenderTarget)
		return;
		
	GameObject::RenderD2D(_pRenderTarget);
}

void PlayerArm::Update_Animation()
{
	if (m_ePrevState == m_eCurState && m_bIsFacingRightPrev == m_bIsFacingRight)
		return;
    SPlayer* player = static_cast<SPlayer*>(GetParent());
    if (!player) return;
    CHook* currentHook = player->GetPlayerHook();
    
	switch (m_eCurState)
	{
	case PLAYER_STATE::IDLE:
			GetAnimator()->Play(L"SNB_ARM_RIGHT_IDLE", true);
		break;
	case PLAYER_STATE::RUN:
			GetAnimator()->Play(L"SNB_ARM_RIGHT_RUN", true);
		break;
	case PLAYER_STATE::EXECUTE:
			GetAnimator()->Play(L"SNB_ARM_RIGHT_EXC_BACK", true);
	    break;
	case PLAYER_STATE::JUMP:
			GetAnimator()->Play(L"SNB_ARM_RIGHT_JUMP", true);
		break;
	case PLAYER_STATE::FALL:
	        GetAnimator()->Play(L"SNB_ARM_RIGHT_FALLING", true);
	    break;
	case PLAYER_STATE::CLIMB:
			GetAnimator()->Play(L"SNB_ARM_RIGHT_CLIMBSTOP", true);
		break;
	case PLAYER_STATE::SHOT:
	    {
	        if (GetIsFacingRight())
	        {
	            Vec2 dir = (currentHook->GetWorldPos() + currentHook->GetDir() * 100.f) - GetWorldPos();
	            dir.Normalize();
	            Vec2 targetPos = GetWorldPos() + dir * 100.f;
	            LookAt(targetPos);
	            SetLocalRotation(GetLocalRotation() -90.f);
	        }
	        else
	        {
	            Vec2 dir = (currentHook->GetWorldPos() + currentHook->GetDir() * 100.f) - GetWorldPos();
	            dir.Normalize();
	            Vec2 targetPos = GetWorldPos() + dir * 100.f;
	            LookAt(targetPos);
	            SetLocalRotation(GetLocalRotation()  + 90.f);
	        }
	        GetAnimator()->Play(L"SNB_ARM_RIGHT_SHOT", true);
	    }
	    break;
	case PLAYER_STATE::SWING:

			GetAnimator()->Play(L"SNB_ARM_RIGHT_SWING", true);
		break;
	case PLAYER_STATE::DAMAGED:

		break;
	case PLAYER_STATE::DEAD:

		break;
	default:
		break;
	}
}

void PlayerArm::Update_ClimbAnimation()
{
    SPlayer* player = static_cast<SPlayer*>(GetParent());

	// 플레이어로부터 현재 클라임 상태를 가져옴
	PLAYER_CLIMB_STATE currentClimbState = player->GetClimbState();

	// 이전 클라임 상태와 다르면 애니메이션 전환
	if (m_ePrevClimbState != currentClimbState)
	{
		if (m_eCurState == PLAYER_STATE::CLIMB)
		{
			switch (currentClimbState)
			{
			case PLAYER_CLIMB_STATE::NONE:
					GetAnimator()->Play(L"SNB_ARM_RIGHT_CLIMBSTOP", true);
				break;
			case PLAYER_CLIMB_STATE::UP:
					GetAnimator()->Play(L"SNB_ARM_RIGHT_CLIMBUP", true);
				break;
			case PLAYER_CLIMB_STATE::DOWN:
					GetAnimator()->Play(L"SNB_ARM_RIGHT_CLIMBDOWN", true);
				break;
			default:
				break;
			}
		}
		m_ePrevClimbState = currentClimbState;
	}
}
