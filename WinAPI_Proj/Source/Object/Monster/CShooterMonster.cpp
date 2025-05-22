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
#include "SPlayer.h" // SPlayer 클래스를 사용하기 위해 추가
#include "CSceneMgr.h" // CSceneMgr을 사용하기 위해 추가
#include "CScene.h" // CScene을 사용하기 위해 추가

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
                                           Vec2(0.f, 1200.f), Vec2(200.f, 200.f), Vec2(200.f, 0.f), 0.25f, 4, 2.f, Vec2(0.f, -54.f));
    GetAnimator()->CreateAnimation(L"RIFLEMAN_AIMING_BODY", pTex,
                                           Vec2(0.f, 1800.f), Vec2(200.f, 200.f), Vec2(200.f, 0.f), 0.25f, 3, 2.f, Vec2(0.f, -54.f));
    GetAnimator()->CreateAnimation(L"RIFLEMAN_AIMING_BODY_STOP", pTex,
                                               Vec2(400.f, 1800.f), Vec2(200.f, 200.f), Vec2(200.f, 0.f), 0.25f, 1, 2.f, Vec2(0.f, -54.f));
   
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
    GetAnimator()->FindAnimation(L"RIFLEMAN_AIMING_BODY_STOP")->Save(L"animation\\rifleman_aiming_body_stop.anim");
    GetAnimator()->FindAnimation(L"RIFLEMAN_SPAWNING")->Save(L"animation\\rifleman_spawning.anim");


    // 회전시킬 헤드,총 이미지 오브젝트 추가
    m_pHead = new CShooterHead();
    m_pHead->SetName(L"MonsterHead");
    m_pHead->SetWorldPos(GetWorldPos());
    m_pHead->SetLocalPos(Vec2(0.f, -80.f));
    m_pHead->SetParent(this);
    CreateObject(m_pHead, GROUP_TYPE::MONSTER_HEAD);
    
    // AI State 세팅
    m_pAI->AddState(new CSpawningState);
    m_pAI->AddState(new CIdleState);
    m_pAI->AddState(new CAimingState);
    m_pAI->SetCurState(MON_STATE::IDLE);
    m_pAI->ChangeState(MON_STATE::SPAWNING);
    
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

    // 플레이어 오브젝트
    SPlayer* pPlayer = dynamic_cast<SPlayer*>(CSceneMgr::GetInst()->GetCurScene()->GetPlayer());
    if (pPlayer)
    {
        // 플레이어의 수평 위치에 따라 몬스터 본체의 좌우 방향을 결정
        if (pPlayer->GetWorldPos().x > GetWorldPos().x)
            m_bIsFacingRight = true; 
        else
            m_bIsFacingRight = false; 
    }
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
