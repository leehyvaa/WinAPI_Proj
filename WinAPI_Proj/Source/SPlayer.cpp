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
#include "CObjectPool.h"
#include "CMonster.h"
#include "AI.h"
#include "Module/AI/State/Subdued/CSubduedState.h"
#include "Object/UI/DamageEffect/CDamageEffectUI.h"


SPlayer::SPlayer()
	: m_fSpeed(1000)
    , m_eCurState(PLAYER_STATE::IDLE)
    , m_ePrevState(PLAYER_STATE::RUN)
    , m_bOnGround(false)
    , m_pPlayerArm(nullptr)
    , m_pPlayerHook(nullptr)
    , m_bClimbing(false)
    , m_pRayHitCollider(nullptr)
    , m_vRayHitPos(Vec2(0.f, 0.f))
    , m_fWireRange(-1.f)
    , m_fWireMaxRange(700.f)
    , m_fMoveEnergy(0.f)
    , m_fPosEnergy(0.f)
    , m_bCanBooster(false)
    , m_eClimbState(PLAYER_CLIMB_STATE::NONE)
    , m_pSubduedMonster(nullptr)
    , m_bIsSubduing(false)
    , m_fSubdueRange(700.f)
    , m_bIsMovingToTarget(false)
    , m_vMoveStartPos(Vec2(0.f, 0.f))
    , m_vMoveTargetPos(Vec2(0.f, 0.f))
    , m_fMoveProgress(0.f)
    , m_fMoveSpeed(2000.f)
    , m_bIsExecuteDashing(false)
    , m_iHP(0)
    , m_iMaxHP(3)
    , m_fInvincibleTime(0.f)
    , m_bDeathAnimationCompleted(false)
    , m_pDamageEffectUI(nullptr)
{
    m_iHP = m_iMaxHP;
    
	// m_pTex = CResMgr::GetInst()->LoadTexture(L"PlayerTex", L"texture\\sigong.png");
	SetGroup(GROUP_TYPE::PLAYER);

	// 67 -13분 캐릭터 상태변환후 애니메이션 전환
	CreateCollider();
	GetCollider()->SetScale(Vec2(32.f, 102.f));
	GetCollider()->SetOffsetPos(Vec2(0.f, -(GetCollider()->GetScale().y) / 2.f));

	CreateRigidBody();
	CreateAnimator();

#pragma region 플레이어 애니메이션
	// 텍스쳐 로딩
	CTexture *pTexRight = CResMgr::GetInst()->LoadTexture(L"PlayerTex_Right", L"texture\\player\\Player_Right.png");

	// 애니메이션 로딩
	// GetAnimator()->LoadAnimation(L"animation\\player_right_idle.anim");

	// RIGHT 애니메이션 생성
    GetAnimator()->CreateAnimation(L"SNB_RIGHT_DAMAGED", pTexRight,
                                   Vec2(0.f, 0.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.07f, 5, 3.f, Vec2(0.f, -57.f));
    GetAnimator()->CreateAnimation(L"SNB_RIGHT_DEATH", pTexRight,
                                      Vec2(0.f, 300.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.3f, 24, 3.f, Vec2(0.f, -32.f));
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
    GetAnimator()->CreateAnimation(L"SNB_RIGHT_EXC_BACK", pTexRight,
                                       Vec2(0.f, 800.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.1f, 8, 3.f, Vec2(0.f, -90.f));
    GetAnimator()->CreateAnimation(L"SNB_RIGHT_EXC_DASH", pTexRight,
                                           Vec2(0.f, 400.f), Vec2(100.f, 100.f), Vec2(100.f, 0.f), 0.03f, 17, 2.0f, Vec2(0.f, -90.f));


	// RIGHT 애니메이션 저장
	GetAnimator()->FindAnimation(L"SNB_RIGHT_DAMAGED")->Save(L"animation\\player_right_damaged.anim");
	GetAnimator()->FindAnimation(L"SNB_RIGHT_DEATH")->Save(L"animation\\player_right_death.anim");
	GetAnimator()->FindAnimation(L"SNB_RIGHT_IDLE")->Save(L"animation\\player_right_idle.anim");
	GetAnimator()->FindAnimation(L"SNB_RIGHT_RUN")->Save(L"animation\\player_right_run.anim");
	GetAnimator()->FindAnimation(L"SNB_RIGHT_JUMP")->Save(L"animation\\player_right_jump.anim");
	GetAnimator()->FindAnimation(L"SNB_RIGHT_FALLING")->Save(L"animation\\player_right_falling.anim");
	GetAnimator()->FindAnimation(L"SNB_RIGHT_LAND")->Save(L"animation\\player_right_land.anim");
	GetAnimator()->FindAnimation(L"SNB_RIGHT_CLIMBUP")->Save(L"animation\\player_right_climbup.anim");
	GetAnimator()->FindAnimation(L"SNB_RIGHT_CLIMBDOWN")->Save(L"animation\\player_right_climbdown.anim");
	GetAnimator()->FindAnimation(L"SNB_RIGHT_CLIMBSTOP")->Save(L"animation\\player_right_climbstop.anim");
	GetAnimator()->FindAnimation(L"SNB_RIGHT_SWING")->Save(L"animation\\player_right_swing.anim");
	GetAnimator()->FindAnimation(L"SNB_RIGHT_EXC_BACK")->Save(L"animation\\player_right_exc_back.anim");
	GetAnimator()->FindAnimation(L"SNB_RIGHT_EXC_DASH")->Save(L"animation\\player_right_exc_dash.anim");


	GetAnimator()->Play(L"SNB_RIGHT_RUN", true);
#pragma endregion

	// 애니메이션 오프셋 개별로 넣기
	// CAnimation* pAnim = GetAnimator()->FindAnimation(L"WALK_RIGHT");
	// for (UINT i = 0; i < pAnim->GetMaxFrame(); i++)
	//{
	//	pAnim->GetFrame(i).vOffset = Vec2(0.f, -20.f);
	// }

	CreateGravity();

    // 레이 생성
	Raycast *pRay = new Raycast();
	pRay->SetName(L"PlayerRay");
	pRay->SetWorldPos(GetWorldPos());
	CreateObject(pRay, GROUP_TYPE::Ray);
	m_pPlayerRay = pRay;
    pRay->SetOwner(this);
	pRay->SetMaxWireRange(m_fWireMaxRange);
    pRay->SetMaxMonsterSearchRange(m_fWireMaxRange);

    // 팔 생성
    GameObject* playerArm = new PlayerArm();
    playerArm->SetName(L"PlayerArm");
    playerArm->SetWorldPos(GetWorldPos());
    playerArm->SetLocalPos(Vec2(0.f, -80.f));
    playerArm->SetParent(this);
    SetArm(static_cast<PlayerArm*>(playerArm));
    CreateObject(playerArm, GROUP_TYPE::PLAYER_ARM);
    
  
    // 와이어 생성
    CObjectPool::GetInst()->CreatePool<CHook>(L"Hook", 1);
    
    // 데미지 이펙트 UI 생성
    m_pDamageEffectUI = new CDamageEffectUI();
    m_pDamageEffectUI->SetName(L"DamageEffectUI");
    
 Enter_State(m_eCurState);
}

SPlayer::~SPlayer()
{
    // 데미지 이펙트 UI 메모리 해제
    if (m_pDamageEffectUI)
    {
        delete m_pDamageEffectUI;
        m_pDamageEffectUI = nullptr;
    }
}


void SPlayer::Reset()
{
    GameObject::Reset();

    m_pPlayerHook = nullptr; // Hook 포인터 초기화
    m_bOnGround = false;
    m_bClimbing = false;
    m_bRidingWire = false;
    m_bCanBooster = false;
    m_bIsExecuteDashing = false;
    m_eCurState = PLAYER_STATE::IDLE;
    m_ePrevState = PLAYER_STATE::RUN;
    m_eClimbState = PLAYER_CLIMB_STATE::NONE;
    m_fWireRange = -1.f;
    m_fMoveEnergy = 0.f;
    m_fPosEnergy = 0.f;
    
    // 제압 시스템 초기화
    m_pSubduedMonster = nullptr;
    m_bIsSubduing = false;
    m_fSubdueRange = 700.f; // 갈고리 최대 범위와 동일하게 설정
    
    // 플레이어 이동 시스템 초기화
    m_bIsMovingToTarget = false;
    m_vMoveStartPos = Vec2(0.f, 0.f);
    m_vMoveTargetPos = Vec2(0.f, 0.f);
    m_fMoveProgress = 0.f;
    m_fMoveSpeed = 2000.f;
    
    // 죽음 처리 시스템 초기화
    m_bDeathAnimationCompleted = false;
    
    // 필요한 경우 Raycast 포인터도 초기화
    m_pRayHitCollider = nullptr;
    m_vRayHitPos = Vec2(0.f, 0.f);
    
    // 데미지 이펙트 상태 초기화
    if (m_pDamageEffectUI)
    {
        m_pDamageEffectUI->ResetEffect();
    }
}


void SPlayer::Update()
{
	RayCasting();
	
	// 플레이어 이동 업데이트
	UpdateMoveToTarget();
	
	// 제압 시스템 업데이트
	UpdateSubdue();

	Update_State();

	Update_Animation();
	ClimbAnimationUpdate();

	if (KEY_TAP(KEY::E))
		system("cls");

	// if (KEY_TAP(KEY::SPACE))
	//	CreateWire();

	if (KEY_TAP(KEY::C))
	{
		cout << GetWorldPos().x << " " << GetWorldPos().y << endl;
		cout << GetRigidBody()->GetSpeed() << endl;
		cout << GetRigidBody()->GetVelocity().x << endl;
		cout << GetRigidBody()->GetVelocity().y << endl;
		cout << static_cast<int>(m_eCurState) << endl;
		cout << m_vRayHitPos.x << " " << m_vRayHitPos.y << endl;
		cout << m_pRayHitCollider << endl;
		cout << m_pPlayerArm->GetWorldPos().x << " " << m_pPlayerArm->GetWorldPos().y << endl;
	}

	GetAnimator()->Update();

	if (m_pPlayerHook != nullptr)
	{
		m_pPlayerHook->SetIsFacingRight(m_bIsFacingRight);
		m_pPlayerHook->SetState(m_eCurState);
	}
	m_pPlayerArm->SetIsFacingRight(m_bIsFacingRight);
	m_pPlayerArm->SetState(m_eCurState);
	m_bIsFacingRightPrev = m_bIsFacingRight;
}

void SPlayer::Render(ID2D1RenderTarget* _pRenderTarget)
{
	GameObject::Render(_pRenderTarget);
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
	case PLAYER_STATE::EXECUTE:
		break;
	case PLAYER_STATE::JUMP:
	    GetRigidBody()->SetMaxSpeed(Vec2(780.f, 1000.f));
	    if (m_bOnGround)
	        GetRigidBody()->AddForce(Vec2(0.f, -15000.f));
        else
            GetRigidBody()->AddForce(Vec2(0.f, -7500.f));
		SetWorldPos(Vec2(GetWorldPos().x, GetWorldPos().y - 20.f));
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
	    if (GetIsFacingRight())
	        m_pPlayerArm->SetLocalRotation(-90.f);
	    else
	        m_pPlayerArm->SetLocalRotation(90.f);
	    GetGravity()->SetApplyGravity(false);
	    GetRigidBody()->SetMaxSpeed(Vec2(1000.f, 1000.f));
		break;
	case PLAYER_STATE::DAMAGED:
	    {
		    GetGravity()->SetApplyGravity(false);
	        GetRigidBody()->SetVelocity(Vec2(0.f, 0.f));
	        // 공격받은 반대 방향으로 튕겨나가는 효과
	        float knockbackPower = 60000.f;
	        if (m_bIsFacingRight)
	            GetRigidBody()->AddForce(Vec2(-knockbackPower/2, -knockbackPower));
	        else
	            GetRigidBody()->AddForce(Vec2(knockbackPower/2, -knockbackPower));
	    }
		break;
	case PLAYER_STATE::DEAD:
		GetRigidBody()->SetVelocity(Vec2(0.f, 0.f));
	    GetGravity()->SetApplyGravity(false);
	    
		break;
	default:
		break;
	}
}


void SPlayer::ChangeState(PLAYER_STATE _eNextState)
{
    if (m_eCurState == _eNextState) return;

    Exit_State(m_eCurState);
    Enter_State(_eNextState);
    m_ePrevState = m_eCurState;
    m_eCurState = _eNextState;
}


void SPlayer::Update_State()
{
    if (m_fInvincibleTime > 0.f)
        m_fInvincibleTime -= fDT; // 매 프레임마다 남은 시간 감소

    // 와이어 발사 또는 제압 시작
    if (KEY_TAP(KEY::LBUTTON))
    {
        bool canSubdue = false;

        if (m_pRayHitCollider != nullptr && m_pRayHitCollider->GetObj()->GetGroup() == GROUP_TYPE::MONSTER)
        {
            CMonster* pMonster = static_cast<CMonster*>(m_pRayHitCollider->GetObj());
            float distance = (m_vRayHitPos - GetWorldPos()).Length();

            // 제압 가능한 몬스터인지 확인
            if (distance <= m_fSubdueRange &&
                !pMonster->IsDead() &&
                pMonster->GetAI() &&
                pMonster->GetAI()->GetCurState() != MON_STATE::DEAD &&
                pMonster->GetAI()->GetCurState() != MON_STATE::SPAWNING)
            {
                // 제압 상태에서 처형 중인 경우도 체크
                if (pMonster->GetAI()->GetCurState() == MON_STATE::SUBDUED)
                {
                    CSubduedState* pSubduedState = static_cast<CSubduedState*>(pMonster->GetAI()->GetState(MON_STATE::SUBDUED));
                    if (!pSubduedState || !pSubduedState->IsExecuted())
                    {
                        canSubdue = true;
                    }
                }
                else
                {
                    canSubdue = true;
                }
            }

            if (canSubdue)
            {
                StartSubdue(pMonster);
                ChangeState(PLAYER_STATE::EXECUTE);
                return; // 상태 변경 후 즉시 종료
            }
        }

        // 제압할 수 없는 몬스터이거나, 몬스터가 아니거나, 멀리 있거나, 아무것도 감지되지 않으면 일반 와이어 발사
        CreateHook();
        ChangeState(PLAYER_STATE::SHOT);
        return; // 상태 변경 후 즉시 종료
    }

    // 와이어 해제 또는 몬스터 처형 (LBUTTON AWAY)
    if (KEY_AWAY(KEY::LBUTTON))
    {
        if (m_bIsSubduing && m_pSubduedMonster)
        {
            EndSubdue();
            GetAnimator()->Play(L"SNB_RIGHT_EXC_DASH", false);
            m_bIsExecuteDashing = true;
            // EXECUTE 상태는 이미 유지되고 있으므로 ChangeState 불필요
        }
        else if (m_pPlayerHook != nullptr && m_pPlayerHook->GetHookState() == HOOK_STATE::GRAB)
        {
            m_pPlayerHook->SetHookState(HOOK_STATE::RETURN_WITH);
            // SWING 상태에서 FALL로 전환되는 로직은 SWING 상태 내부에서 처리
        }
    }



    
	switch (m_eCurState)
	{
	case PLAYER_STATE::IDLE:
		HorizontalMove();
		if (!m_bOnGround && GetRigidBody()->GetVelocity().y > 0.f) { ChangeState(PLAYER_STATE::FALL); return; }
		if (KEY_TAP(KEY::SPACE) && m_bOnGround)                   { ChangeState(PLAYER_STATE::JUMP); return; }
		if (KEY_HOLD(KEY::A) || KEY_HOLD(KEY::D))                 { ChangeState(PLAYER_STATE::RUN); return; }
		break;

	case PLAYER_STATE::RUN:
		HorizontalMove();
		if (!m_bOnGround && GetRigidBody()->GetVelocity().y > 0.f) { ChangeState(PLAYER_STATE::FALL); return; }
		if (KEY_TAP(KEY::SPACE) && m_bOnGround)                   { ChangeState(PLAYER_STATE::JUMP); return; }
		if (0.f == GetRigidBody()->GetSpeed() && m_bOnGround)     { ChangeState(PLAYER_STATE::IDLE); return; }
		break;

	case PLAYER_STATE::EXECUTE:
		HorizontalMove();
	    if (!m_bIsSubduing && m_bIsExecuteDashing)
	    {
	        CAnimation* pCurAnim = GetAnimator()->GetCurAnimation();
	        if (pCurAnim && pCurAnim->IsFinish())
	        {
	            m_bIsExecuteDashing = false;
	            ChangeState(PLAYER_STATE::FALL);
                return;
	        }
	    }
		break;

	case PLAYER_STATE::JUMP:
		HorizontalMove();
		if (IsWallClimbing())                                     { ChangeState(PLAYER_STATE::CLIMB); return; }
	    if (GetRigidBody()->GetVelocity().y > 0.f)                { ChangeState(PLAYER_STATE::FALL); return; }
		if (m_bOnGround && GetRigidBody()->GetVelocity().y >= 0.f) { ChangeState(PLAYER_STATE::IDLE); return; }
		break;

	case PLAYER_STATE::FALL:
	    HorizontalMove();
	    if (IsWallClimbing())                                     { ChangeState(PLAYER_STATE::CLIMB); return; }
	    if (m_bOnGround)                                          { ChangeState(PLAYER_STATE::IDLE); return; }
	    break;

	case PLAYER_STATE::CLIMB:
		VirticalMove();
	    if (!m_bClimbing)
	    {
	        if (m_eClimbState == PLAYER_CLIMB_STATE::UP) { ChangeState(PLAYER_STATE::JUMP); return; }
	        else                                         { ChangeState(PLAYER_STATE::FALL); return; }
	    }
		if (KEY_TAP(KEY::SPACE))
		{
			WallKickJump();
			ChangeState(PLAYER_STATE::JUMP);
            return;
		}
		break;

	case PLAYER_STATE::SHOT:
	    if (m_pPlayerHook == nullptr)                                                               { ChangeState(PLAYER_STATE::IDLE); return; }
        // 갈고리가 실제로 벽에 박혀있을 때만 SWING 상태로 전환
        if (m_pPlayerHook != nullptr && m_pPlayerHook->GetHookState() == HOOK_STATE::GRAB)          { ChangeState(PLAYER_STATE::SWING); return; }
		break;

	case PLAYER_STATE::SWING:
		SwingMove();
		if (KEY_AWAY(KEY::LBUTTON)) { ChangeState(PLAYER_STATE::FALL); return; }
		break;

	case PLAYER_STATE::DAMAGED:
        // 피격 애니메이션이 끝났는지 확인
        if (GetAnimator()->GetCurAnimation()->IsFinish())
        {
            ChangeState(PLAYER_STATE::FALL);
            return;
        }
		break;

	case PLAYER_STATE::DEAD:
		// 죽음 애니메이션이 완료되었는지 확인
		if (GetAnimator()->GetCurAnimation() && GetAnimator()->GetCurAnimation()->IsFinish())
		{
			m_bDeathAnimationCompleted = true;
		}
		break;

	default:
		break;
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
	case PLAYER_STATE::EXECUTE:
		break;
	case PLAYER_STATE::JUMP:
	    break;
	case PLAYER_STATE::FALL:
		break;
	case PLAYER_STATE::CLIMB:
		m_eClimbState = PLAYER_CLIMB_STATE::NONE;
		break;
	case PLAYER_STATE::SHOT:
	    m_pPlayerArm->SetLocalRotation(0.f);
		break;
	case PLAYER_STATE::SWING:
	    GetGravity()->SetApplyGravity(true);
        SetLocalRotation(0.f);
	    m_pPlayerArm->SetLocalRotation(0.f);
		break;
	case PLAYER_STATE::DAMAGED:
	    GetGravity()->SetApplyGravity(true);
	    if (GetRigidBody()->GetVelocity().y <0.f)
	        GetRigidBody()->SetVelocityY(0.f);
		break;
	case PLAYER_STATE::DEAD:
		break;
	default:
		break;
	}
}

void SPlayer::Update_Animation()
{
	if (m_ePrevState == m_eCurState && m_bIsFacingRightPrev == m_bIsFacingRight)
		return;


	switch (m_eCurState)
	{
	case PLAYER_STATE::IDLE:
			GetAnimator()->Play(L"SNB_RIGHT_IDLE", true);
		break;
	case PLAYER_STATE::RUN:
			GetAnimator()->Play(L"SNB_RIGHT_RUN", true);
		break;
	case PLAYER_STATE::EXECUTE:
	    if (m_bIsSubduing && m_pSubduedMonster)
	    {
			GetAnimator()->Play(L"SNB_RIGHT_EXC_BACK", true);
	    }
		break;
	case PLAYER_STATE::JUMP:
			GetAnimator()->Play(L"SNB_RIGHT_JUMP", true);
		break;
	case PLAYER_STATE::FALL:
	        GetAnimator()->Play(L"SNB_RIGHT_FALLING", true);
	    break;
	case PLAYER_STATE::CLIMB:
			//GetAnimator()->Play(L"SNB_RIGHT_CLIMBSTOP", true);
		break;
	case PLAYER_STATE::SWING:
	    if (m_pPlayerHook != nullptr)
	    {
	        LookAt(m_pPlayerHook->GetWorldPos());
	    }
	        GetAnimator()->Play(L"SNB_RIGHT_SWING", true);
	    break;
	case PLAYER_STATE::DAMAGED:
	        GetAnimator()->Play(L"SNB_RIGHT_DAMAGED", false);
		break;
	case PLAYER_STATE::DEAD:
	        GetAnimator()->Play(L"SNB_RIGHT_DEATH", false);

		break;
	default:
		break;
	}
}


void SPlayer::ClimbAnimationUpdate()
{
    // 클라임 상태일 때에만 애니메이션 적용
    if (m_eCurState == PLAYER_STATE::CLIMB)
    {
        switch (m_eClimbState)
        {
        case PLAYER_CLIMB_STATE::NONE:
            GetAnimator()->Play(L"SNB_RIGHT_CLIMBSTOP", true);
            break;
        case PLAYER_CLIMB_STATE::UP:
            GetAnimator()->Play(L"SNB_RIGHT_CLIMBUP", true);
            break;
        case PLAYER_CLIMB_STATE::DOWN:
            GetAnimator()->Play(L"SNB_RIGHT_CLIMBDOWN", true);
            break;
        default:
            break;
        }
        m_ePrevClimbState = m_eClimbState;
    }
}


void SPlayer::Update_Gravity()
{
	GetRigidBody()->AddForce(Vec2(0.f, 700.f));
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




// 매달린 상태에서 점프 (반대방향으로 점프 혹은 위로 뛰어서 다시 위의 벽 잡기)
void SPlayer::WallKickJump()
{
	if (m_bIsFacingRight == true)
	{
	    if (KEY_HOLD(KEY::D))
	    {
	        // 이 부분을 매끄럽게 처리하려면 점프 후에 일정 시간동안 climb로 안넘어가게 하거나 점프를 Addforce로 수정
	        //GetRigidBody()->SetVelocity(Vec2(-1000.f, -2000.f));
	        SetWorldPos(Vec2(GetWorldPos().x-20.f, GetWorldPos().y-50.f));
	    }
        else
        {
            m_bIsFacingRight = false;
            GetRigidBody()->AddForce(Vec2(-6000.f, -13000.f));
            //GetRigidBody()->SetVelocity(Vec2(-400.f, -950.f));
        }
	}
	else
	{
	    if (KEY_HOLD(KEY::A))
	    {
	        SetWorldPos(Vec2(GetWorldPos().x+20.f, GetWorldPos().y-50.f));
	    }
	    else
	    {
	        m_bIsFacingRight = true;
	        GetRigidBody()->AddForce(Vec2(6000.f, -13000.f));
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
		m_bIsFacingRight = false;
	    pRigid->AddForce(Vec2(-MOVE_FORCE,0.f));
	}
	if (KEY_HOLD(KEY::D))
	{
		m_bIsFacingRight = true;
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
    Vec2 hookPos = m_pPlayerHook->GetWorldPos();
    
    // 거리 기반 판별
    float currentDistance = (hookPos - m_pPlayerArm->GetWorldPos()).Length();
    float distanceRatio = currentDistance / m_fWireRange;
    bool isNearPerimeter = (distanceRatio > 0.95f); // 95% 이상이면 최외각 근처로 간주

    // 다음 위치 예측
    Vec2 currentVelocity = GetRigidBody()->GetVelocity();
    Vec2 nextPredictedPos = m_pPlayerArm->GetWorldPos() + currentVelocity * fDT;
    float nextPredictedDistance = (hookPos - nextPredictedPos).Length();

    // 다음 위치가 와이어 범위를 초과할 것으로 예상되는지 확인
    bool willExceedRange = (nextPredictedDistance > m_fWireRange);

    // 진행 방향과 갈고리 방향의 관계 확인 (갈고리 바깥쪽으로 향하는지)
    Vec2 wireDir = m_pPlayerArm->GetWorldPos() - hookPos;
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
    Vec2 hookPos = m_pPlayerHook->GetWorldPos();

    // 원심력이 존재해서 플레이어 이동
    // (3.14159 / 180.f)는 degree를 radian으로 변환하는 공식
    // 매 프레임마다 갈고리를 중심으로 1.2도 회전한 위치를 목표 방향으로 설정
    double radian = (0.6f) * (3.14159 / 180.f);
    if (m_fMoveEnergy > 0.f)
        radian *= -1.f;

    // 갈고리에서 플레이어 방향 계산
    Vec2 dirToPlayer = m_pPlayerArm->GetWorldPos() - hookPos;
    dirToPlayer.Normalize();

    // m_fWireRange 거리에 있는 점 계산,이 위치는 부스터 쓸때만 써야할듯?
    Vec2 curMaxPos = hookPos + dirToPlayer * m_fWireRange;

    // 이 점을 회전시켜 다음 위치 계산
    Vec2 nextPos;
    // 이 위치는 부스터 쓸때만 써야할듯?, 갈고리 박고 4초간 떨리는 현상 발생
    //nextPos.x = (curMaxPos.x - hookPos.x) * cos(radian) - (curMaxPos.y - hookPos.y) * sin(radian) + hookPos.x;
    //nextPos.y = (curMaxPos.x - hookPos.x) * sin(radian) + (curMaxPos.y - hookPos.y) * cos(radian) + hookPos.y;
    nextPos.x = (m_pPlayerArm->GetWorldPos().x - hookPos.x) * static_cast<float>(cos(radian)) - (m_pPlayerArm->GetWorldPos().y - hookPos.y) *
        static_cast<float>(sin(radian)) + hookPos.x;
    nextPos.y = (m_pPlayerArm->GetWorldPos().x - hookPos.x) * static_cast<float>(sin(radian)) + (m_pPlayerArm->GetWorldPos().y - hookPos.y) *
        static_cast<float>(cos(radian)) + hookPos.y;

    // 플레이어와 갈고리 사이의 각도와 현재 받는 힘에 따라 플레이어가 이동할 다음 위치 계산
    Vec2 nextDir = nextPos - m_pPlayerArm->GetWorldPos();
    nextDir.Normalize();


    // 계산한 방향 대로 플레이어의 속도 바꾸기
    CRigidBody* pRigid = GetRigidBody();
    //pRigid->AddForce(nextDir * abs(m_fMoveEnergy) * 10.f);
    pRigid->SetVelocity(nextDir * abs(m_fMoveEnergy));


    //갈고리와 플레이어 사이의 거리가 와이어 거리를 넘어가지 않도록 제한
    if (m_fHookDistance > m_fWireRange)
    {
        // 현재 위치와 원하는 위치의 차이
        Vec2 correction = curMaxPos - m_pPlayerArm->GetWorldPos();

        // 스프링 힘 계산 , k 값 조절로 탄성 조절
        float k = 1000.0f; 
        Vec2 springForce = correction * k;


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
    Vec2 hookPos = m_pPlayerHook->GetWorldPos();
    m_fHookDistance = (hookPos - m_pPlayerArm->GetWorldPos()).Length();

    // 갈고리와 플레이어의 현재 각도 구하기
    Vec2 wireDir = m_pPlayerArm->GetWorldPos() - hookPos;
    Vec2 up = Vec2(hookPos.x, hookPos.y - 1) - hookPos;
    float angle;
    
    // 갈고리가 플레이어의 좌우 중 어디에 있냐에 따라 각도 offset 조절
    if (hookPos.x < m_pPlayerArm->GetWorldPos().x)
        angle = wireDir.Angle(up);
    else
    {
        float offset = 180.f - wireDir.Angle(up);
        angle = static_cast<float>(offset + 180.f);
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
        
    if (hookPos.y > m_pPlayerArm->GetWorldPos().y) // 플레이어가 갈고리보다 위에 있으면 m_fPosEnergy = 0
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
    Vec2 hookPos = m_pPlayerHook->GetWorldPos();
    
    
    // MoveEnergy와 PosEnergy 계산
    UpdateSwingEnergy();

    // 부스터
    if (m_bCanBooster)
    {
        if (KEY_HOLD(KEY::A) && KEY_HOLD(KEY::LSHIFT))
        {
            m_fMoveEnergy -= 3500.f;
            m_bCanBooster = false;
        }
        if (KEY_HOLD(KEY::D) && KEY_HOLD(KEY::LSHIFT))
        {
            m_fMoveEnergy += 3500.f;
            m_bCanBooster = false;
        }
    }
    
    // 와이어가 팽팽한 상태(플레이어가 원의 최외곽에 있는지)
    if (IsWireTaut())
    {
        // 기본적으로 중력 미적용
        GetGravity()->SetApplyGravity(false);

        // 플레이어가 갈고리보다 위에 있는 경우
        if (hookPos.y > m_pPlayerArm->GetWorldPos().y)
        {
            // MoveEnergy의 힘이 500보다 작으면 원심력이 부족하다 판단하고 중력 적용
            if ((prevMoveEnergy > 500 && m_fMoveEnergy <= 500) || 
                (prevMoveEnergy < -500 && m_fMoveEnergy >= -500) ||
                (abs(m_fMoveEnergy) < 800.f)) // 허용 오차 추가
            {
                // 중력 적용
                GetGravity()->SetApplyGravity(true);    
                m_fMoveEnergy=0.f;
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

    // 풀에서 Hook 가져오기
    m_pPlayerHook = dynamic_cast<CHook*>(CObjectPool::GetInst()->GetPoolObject(L"Hook"));
    m_pPlayerHook->SetOwnerArm(m_pPlayerArm);
    m_pPlayerHook->SetWorldPos(m_pPlayerArm->GetWorldPos());
    // 씬에 넣기
    CreateObject(m_pPlayerHook, GROUP_TYPE::HOOK);
    
    // 와이어 발사 방향으로 플레이어 바라보기
	if (CCamera::GetInst()->GetRealPos(MOUSE_POS).x < GetWorldPos().x)
		m_bIsFacingRight = false;
	else
		m_bIsFacingRight = true;

    
    // Ray가 아무것도 맞추지 못했을 경우
	if (m_vRayHitPos.IsZero())
	{
		m_pPlayerHook->LookAt(CCamera::GetInst()->GetRealPos(MOUSE_POS));
	}
	else // Ray의 거리가 오브젝트에 닿았을 경우
	{
		m_pPlayerHook->LookAt(m_vRayHitPos);
        m_pPlayerHook->SetTargetPos(m_vRayHitPos);
	    // Ray에 충돌한 물체가 GROUND일 경우
		if (m_pRayHitCollider->GetObj()->GetGroup() == GROUP_TYPE::GROUND)
		{
			Vec2 dir = m_vRayHitPos - m_pPlayerArm->GetWorldPos();
			dir.Normalize();

			float distance = (m_vRayHitPos - m_pPlayerArm->GetWorldPos()).Length();

			if (distance > m_fWireMaxRange)
			{
				GetRigidBody()->SetVelocity(dir * 500);
				m_fWireRange = m_fWireMaxRange;
			}
			else
			{
				m_fWireRange = distance;
			}

			if (m_vRayHitPos.x < m_pPlayerArm->GetWorldPos().x)
				m_fMoveEnergy = -distance * 1.5f;
			else
				m_fMoveEnergy = distance * 1.5f;
		}
		else if (m_pPlayerRay->GetCollisionRay()->GetObj()->GetGroup() == GROUP_TYPE::MONSTER)
		{
		}
	}
 
}

// RayCast를 진행 후 Ray와 충돌한 충돌체를 onCollisionRay에 저장하고 충돌 지점을 targetPos에 저장
void SPlayer::RayCasting()
{
	m_pPlayerRay->SetWorldPos(m_pPlayerArm->GetWorldPos());
	m_pRayHitCollider = m_pPlayerRay->GetCollisionRay();
	m_vRayHitPos = m_pPlayerRay->GetTargetPos();
}

// 몬스터 제압 시작
void SPlayer::StartSubdue(CMonster* _pMonster)
{
	if (!_pMonster || m_bIsSubduing)
		return;

	if (_pMonster->IsDead() || (_pMonster->GetAI() && (_pMonster->GetAI()->GetCurState() == MON_STATE::DEAD ||
	                                                    _pMonster->GetAI()->GetCurState() == MON_STATE::SPAWNING)))
		return;

	// 제압 상태에서 처형 중인 경우에도 제압 불가
	if (_pMonster->GetAI() && _pMonster->GetAI()->GetCurState() == MON_STATE::SUBDUED)
	{
		CSubduedState* pSubduedState = static_cast<CSubduedState*>(_pMonster->GetAI()->GetState(MON_STATE::SUBDUED));
		if (pSubduedState && pSubduedState->IsExecuted())
		{
			return; // 처형 중인 몬스터는 제압 불가
		}
	}
		
	m_pSubduedMonster = _pMonster;
	m_bIsSubduing = true;
	
	// 플레이어가 몬스터 위치로 빠르게 이동
	Vec2 monsterPos = m_pSubduedMonster->GetWorldPos();
	Vec2 targetPos = monsterPos + Vec2(0.f, 0.f); // 몬스터와 같은 위치로 이동
	StartMoveToTarget(targetPos);
	
	// 몬스터를 제압 상태로 전환
	if (m_pSubduedMonster->GetAI())
		m_pSubduedMonster->GetAI()->ChangeState(MON_STATE::SUBDUED);
	
	
	// 플레이어 방향 설정
	if (m_pSubduedMonster->GetWorldPos().x < GetWorldPos().x)
		m_bIsFacingRight = false;
	else
		m_bIsFacingRight = true;
}

// 몬스터를 제압중인 상태
void SPlayer::UpdateSubdue()
{
	if (!m_bIsSubduing || !m_pSubduedMonster)
		return;
    
	if (!m_pSubduedMonster->GetAI())
	{
		EndSubdue();
		return;
	}
	
	// 몬스터가 이미 죽었거나 처형된 상태면 제압 해제
	MON_STATE currentState = m_pSubduedMonster->GetAI()->GetCurState();
	if (currentState == MON_STATE::DEAD || currentState == MON_STATE::EXECUTED)
	{
		EndSubdue();
		return;
	}
	
	// 제압 중인 몬스터를 플레이어 근처로 이동
	Vec2 playerPos = GetWorldPos();
	Vec2 targetPos = playerPos;
	
	// 몬스터 위치를 부드럽게 이동
	Vec2 currentMonsterPos = m_pSubduedMonster->GetWorldPos();
	Vec2 moveDir = targetPos - currentMonsterPos;
	float moveSpeed = 800.f * fDT;
	
	if (moveDir.Length() > moveSpeed)
	{
		moveDir.Normalize();
		m_pSubduedMonster->SetWorldPos(currentMonsterPos + moveDir * moveSpeed);
	}
	else
	{
		m_pSubduedMonster->SetWorldPos(targetPos);
	}
}

// 제압 해제 및 처형
void SPlayer::EndSubdue()
{
    if (!m_bIsSubduing)
        return;
    
    // 처형 시 마우스 방향으로 대쉬
    Vec2 mouseWorldPos = CCamera::GetInst()->GetRealPos(MOUSE_POS);
    Vec2 dashDir = mouseWorldPos - GetWorldPos();
    dashDir.Normalize();
    
    // 대쉬 힘 적용
    float dashForce = 10000.f; // 힘의 크기는 조절 가능
    GetRigidBody()->SetVelocity(dashDir * 1500.f); // 즉시 속도 설정
    GetRigidBody()->AddForce(dashDir * dashForce); // 추가 힘 적용
    
    GetGravity()->SetApplyGravity(true);
    
    // 처형 방향으로 플레이어 바라보기
    if (mouseWorldPos.x < GetWorldPos().x)
        m_bIsFacingRight = false;
    else
        m_bIsFacingRight = true;
        
    m_bIsSubduing = false;
    m_pSubduedMonster = nullptr;
}

// 플레이어 사망 시 제압 정리
void SPlayer::CleanupSubdueOnDeath()
{
	if (m_bIsSubduing && m_pSubduedMonster)
	{
		// 플레이어가 죽으면 제압된 몬스터를 IDLE 상태로 복원
		if (m_pSubduedMonster->GetAI())
		{
			m_pSubduedMonster->GetAI()->ChangeState(MON_STATE::IDLE);
		}
		EndSubdue();
	}
}

// 목표 위치로 플레이어 빠른 이동
void SPlayer::StartMoveToTarget(const Vec2& _targetPos)
{
	m_bIsMovingToTarget = true;
	m_vMoveStartPos = GetWorldPos();
	m_vMoveTargetPos = _targetPos;
	m_fMoveProgress = 0.f;
	
	// 물리 상태 비활성화
	GetGravity()->SetApplyGravity(false);
	GetRigidBody()->SetVelocity(Vec2(0.f, 0.f));
}

// 이동 업데이트
void SPlayer::UpdateMoveToTarget()
{
	if (!m_bIsMovingToTarget)
		return;
		
	// 이동 진행도
	m_fMoveProgress += m_fMoveSpeed * fDT / (m_vMoveTargetPos - m_vMoveStartPos).Length();
    
	if (m_fMoveProgress >= 1.f)
	{
		CompleteMoveToTarget();    // 이동 완료
		return;
	}
	
	// 보간으로 플레이어 위치 업데이트
	Vec2 currentPos = m_vMoveStartPos + (m_vMoveTargetPos - m_vMoveStartPos) * m_fMoveProgress;
	SetWorldPos(currentPos);
    
	GetRigidBody()->SetVelocity(Vec2(0.f, 0.f));
}

// 이동 완료 처리
void SPlayer::CompleteMoveToTarget()
{
	m_bIsMovingToTarget = false;
	m_fMoveProgress = 0.f;
	
	// 목표 위치로 마무리 이동
	SetWorldPos(m_vMoveTargetPos);
	
	// 상태 복원
	GetGravity()->SetApplyGravity(true);
	GetRigidBody()->SetVelocity(Vec2(0.f, 0.f));
}


void SPlayer::TakeDamage(int m_iDamage)
{
    // 무적 상태인 경우
    if (m_fInvincibleTime > 0.f)
        return;
    if (m_eCurState == PLAYER_STATE::EXECUTE || m_eCurState == PLAYER_STATE::DEAD)
        return;


    
    // 체력 감소 및 무적 시간 설정
    m_iHP -= m_iDamage;
    m_fInvincibleTime = 0.75f;
    
    // 데미지 이펙트 트리거 (Unity의 OnPlayerDamaged와 동일한 기능)
    if (m_pDamageEffectUI)
    {
        m_pDamageEffectUI->OnPlayerDamaged();
    }

    // 플레이어 사망 처리
    if (m_iHP <= 0)
    {
        m_iHP = 0;
        ChangeState(PLAYER_STATE::DEAD);
    }
    else
    {
        ChangeState(PLAYER_STATE::DAMAGED);

        // 피격 시 슬로우 모션 효과
        CTimeMgr::GetInst()->StartSlowMotion(0.2f, 0.7f);
    }
}

