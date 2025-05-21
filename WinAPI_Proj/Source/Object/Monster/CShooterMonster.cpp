#include "pch.h"
#include "CShooterMonster.h"

#include "AI.h"
#include "CAnimation.h"
#include "CAnimator.h"
#include "CCollider.h"
#include "CIdleState.h"
#include "CResMgr.h"
#include "CShooterHead.h"
#include "Shooter/CAimingState.h"
#include "Spawning/CSpawningState.h"

CShooterMonster::CShooterMonster()
{
    GetCollider()->SetScale(Vec2(32.f, 122.f));
    GetCollider()->SetOffsetPos(Vec2(0.f, -(GetCollider()->GetScale().y) / 2.f));
    CreateGravity();

    CTexture *pTex = CResMgr::GetInst()->LoadTexture(L"RifleManTex", L"texture\\enemy\\rifleman\\RifleMan.bmp");
    GetAnimator()->CreateAnimation(L"RIFLEMAN_IDLE", pTex,
                                       Vec2(0.f, 0.f), Vec2(200.f, 200.f), Vec2(200.f, 0.f), 0.25f, 15, 2.f, Vec2(0.f, -64.f));
    GetAnimator()->CreateAnimation(L"RIFLEMAN_EXC_START", pTex,
                                           Vec2(0.f, 200.f), Vec2(200.f, 200.f), Vec2(200.f, 0.f), 0.25f, 4, 2.f, Vec2(0.f, -64.f));
    GetAnimator()->CreateAnimation(L"RIFLEMAN_EXC_BACK", pTex,
                                           Vec2(0.f, 400.f), Vec2(200.f, 200.f), Vec2(200.f, 0.f), 0.25f, 8, 2.f, Vec2(0.f, -64.f));
    GetAnimator()->CreateAnimation(L"RIFLEMAN_DEAD", pTex,
                                           Vec2(0.f, 600.f), Vec2(200.f, 200.f), Vec2(200.f, 0.f), 0.25f, 10, 2.f, Vec2(0.f, -64.f));
    GetAnimator()->CreateAnimation(L"RIFLEMAN_DEAD_PART", pTex,
                                           Vec2(0.f, 800.f), Vec2(200.f, 200.f), Vec2(200.f, 0.f), 0.25f, 11, 2.f, Vec2(0.f, -64.f));
    GetAnimator()->CreateAnimation(L"RIFLEMAN_BULLET", pTex,
                                           Vec2(0.f, 1000.f), Vec2(200.f, 200.f), Vec2(200.f, 0.f), 0.25f, 12, 2.f, Vec2(0.f, -64.f));
    GetAnimator()->CreateAnimation(L"RIFLEMAN_SHOT_BODY", pTex,
                                           Vec2(0.f, 1200.f), Vec2(200.f, 200.f), Vec2(200.f, 0.f), 0.25f, 4, 2.f, Vec2(0.f, -64.f));
    GetAnimator()->CreateAnimation(L"RIFLEMAN_AIMING_BODY", pTex,
                                           Vec2(0.f, 1800.f), Vec2(200.f, 200.f), Vec2(200.f, 0.f), 0.25f, 3, 2.f, Vec2(0.f, -64.f));
    GetAnimator()->CreateAnimation(L"RIFLEMAN_SPAWNING", pTex,
                                           Vec2(0.f, 2200.f), Vec2(200.f, 200.f), Vec2(200.f, 0.f), 0.25f, 21, 2.f, Vec2(0.f, -64.f));



    GetAnimator()->FindAnimation(L"RIFLEMAN_IDLE")->Save(L"animation\\rifleman_idle.anim");
    GetAnimator()->FindAnimation(L"RIFLEMAN_EXC_START")->Save(L"animation\\rifleman_exc_start.anim");
    GetAnimator()->FindAnimation(L"RIFLEMAN_EXC_BACK")->Save(L"animation\\rifleman_exc_back.anim");
    GetAnimator()->FindAnimation(L"RIFLEMAN_DEAD")->Save(L"animation\\rifleman_dead.anim");
    GetAnimator()->FindAnimation(L"RIFLEMAN_DEAD_PART")->Save(L"animation\\rifleman_dead_part.anim");
    GetAnimator()->FindAnimation(L"RIFLEMAN_BULLET")->Save(L"animation\\rifleman_bullet.anim");
    GetAnimator()->FindAnimation(L"RIFLEMAN_SHOT_BODY")->Save(L"animation\\rifleman_shot_body.anim");
    GetAnimator()->FindAnimation(L"RIFLEMAN_AIMING_BODY")->Save(L"animation\\rifleman_aiming_body.anim");
    GetAnimator()->FindAnimation(L"RIFLEMAN_SPAWNING")->Save(L"animation\\rifleman_spawning.anim");
	
    m_pHead = new CShooterHead();
    m_pHead->SetName(L"MonsterHead");
    m_pHead->SetWorldPos(GetWorldPos());
    m_pHead->SetLocalPos(Vec2(0.f, -80.f));
    m_pHead->SetParent(this);
    CreateObject(m_pHead, GROUP_TYPE::MONSTER_HEAD);
    
    // 몬스터 info 구조체 설정 (ex 체력 등)
    /*
    * `SetAI()`를 통해 `AI` 객체를 설정하고, 해당 `AI` 객체에 몬스터 타입별로 필요한
    * `CState` 파생 객체들을 `AddState()`로 추가하고, `SetCurState()`로 초기 상태를 지정합니다.
        *   예를 들어, `CDefenderMonster`는 방어 관련 상태(`CBlockState` 등)를 추가할 수 있습니다.
        *   `CShooterMonster`는 원거리 공격을 위한 조준 상태(`CAimState`),
        *   발사 상태(`CFireState`) 등을 가질 수 있습니다.
        *   `CFlyingMonster`는 공중 이동 패턴이나 특별한 공격 방식에 맞는 상태들
        *   (예: `CFlyIdleState`, `CDiveAttackState`)을 가질 수 있습니다.

     */

    m_pAI->AddState(new CSpawningState);
    m_pAI->AddState(new CIdleState);
    m_pAI->AddState(new CAimingState);
    m_pAI->SetCurState(MON_STATE::IDLE);
    m_pAI->ChangeState(MON_STATE::SPAWNING);
    //Enter_State(m_eCurState);
}

CShooterMonster::~CShooterMonster()
{
}

void CShooterMonster::Update_Gravity()
{
}



void CShooterMonster::Start()
{
    CMonster::Start();
}

void CShooterMonster::Update()
{
    CMonster::Update();
}

void CShooterMonster::Render(HDC _dc)
{
    CMonster::Render(_dc);
}

void CShooterMonster::FinalUpdata()
{
    CMonster::FinalUpdata();
}

void CShooterMonster::Reset()
{
    CMonster::Reset();
}

void CShooterMonster::OnCollisionEnter(CCollider* _pOther)
{
    CMonster::OnCollisionEnter(_pOther);
}

void CShooterMonster::OnCollision(CCollider* _pOther)
{
    CMonster::OnCollision(_pOther);
}

void CShooterMonster::OnCollisionExit(CCollider* _pOther)
{
    CMonster::OnCollisionExit(_pOther);
}