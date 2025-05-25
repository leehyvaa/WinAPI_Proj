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
#include "Subdued/CSubduedState.h"
#include "SPlayer.h"
#include "CSceneMgr.h" 
#include "CScene.h" 
#include "CBullet.h" 
#include "CObjectPool.h"
#include "Dead/CDeadState.h"

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
    GetAnimator()->FindAnimation(L"RIFLEMAN_SHOT_BODY")->Save(L"animation\\rifleman_shot_body.anim");
    GetAnimator()->FindAnimation(L"RIFLEMAN_AIMING_BODY")->Save(L"animation\\rifleman_aiming_body.anim");
    GetAnimator()->FindAnimation(L"RIFLEMAN_AIMING_BODY_STOP")->Save(L"animation\\rifleman_aiming_body_stop.anim");
    GetAnimator()->FindAnimation(L"RIFLEMAN_SPAWNING")->Save(L"animation\\rifleman_spawning.anim");


    // 회전시킬 헤드,총 이미지 오브젝트 추가
    m_pHead = static_cast<CShooterHead*>(CObjectPool::GetInst()->GetPoolObject(L"ShooterHeadPool"));
    if (m_pHead) {
        m_pHead->SetName(L"MonsterHead");
        // m_pHead->SetWorldPos(GetWorldPos()); // SetParent 이후에 위치가 결정되므로 주석 처리 또는 제거 가능
        m_pHead->SetLocalPos(Vec2(0.f, -80.f));
        m_pHead->SetParent(this);
        CreateObject(m_pHead, GROUP_TYPE::MONSTER_HEAD); // 씬에 추가
        m_pHead->SetActive(true); // 풀에서 가져온 객체는 기본적으로 비활성이므로 활성화
    }
    
    // AI State 세팅
    m_pAI->AddState(new CSpawningState);
    m_pAI->AddState(new CIdleState);
    m_pAI->AddState(new CAimingState);
    m_pAI->AddState(new CSubduedState);
    m_pAI->AddState(new CDeadState);
    
    m_pAI->SetCurState(MON_STATE::IDLE);
    m_pAI->ChangeState(MON_STATE::SPAWNING);
    
    // 총알 오브젝트 풀 생성
    CObjectPool::GetInst()->CreatePool<CBullet>(L"ShooterBullet", 10);
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
    
    if(nullptr != m_pAI)
    {
        m_pAI->Update();
		
        // DEAD 상태일 때 몬스터 삭제 처리
        if (m_pAI->GetCurState() == MON_STATE::DEAD)
        {
            if (m_pHead && m_pHead->IsActive()) { // 헤드가 존재하고 활성 상태일 때만 반환
                CObjectPool::GetInst()->ReturnObject(m_pHead);
                if (m_pHead->IsManagedByPool()) { // 풀에서 관리되는 객체만 SetDead 처리
                    m_pHead->SetDead(true);
                }
            }
            m_pHead = nullptr; // 참조 제거

            CObjectPool::GetInst()->ReturnObject(this);
            if (IsManagedByPool()) { // 풀에서 관리되는 객체만 SetDead 처리
                SetDead(true);
            }
        }
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
    CMonster::Reset(); // 부모 클래스의 Reset 호출
    if (m_pAI) {
        m_pAI->SetCurState(MON_STATE::IDLE); // 또는 SPAWNING 등 초기 상태
        m_pAI->ChangeState(MON_STATE::SPAWNING); // 초기 상태로 강제 전환
    }
    if (m_pHead) {
        m_pHead->Reset();
        m_pHead->SetActive(true); // 헤드도 활성화
    }
    SetManagedByPool(true); // 풀에서 관리됨을 명시
    // 체력 등 몬스터 특성 초기화
    m_tInfo.fHP = 100.f; // 예시: 초기 체력 설정
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
