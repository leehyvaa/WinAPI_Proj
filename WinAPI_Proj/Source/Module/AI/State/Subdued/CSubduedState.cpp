#include "pch.h"
#include "CSubduedState.h"

#include "AI.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "SPlayer.h"
#include "CMonster.h"
#include "CAnimation.h"
#include "CAnimator.h"
#include "CGravity.h"
#include "CKeyMgr.h"
#include "CRigidBody.h"
#include "CTimeMgr.h"
#include "CCollider.h"
#include "Monster/CShooterHead.h"

CSubduedState::CSubduedState()
    : CState(MON_STATE::SUBDUED)
    , m_vFixedPosition(Vec2(0.f, 0.f))
    , m_bIsBeingSubdued(true)
    , m_fExecutionTimer(0.f)
    , m_bIsExecuted(false)
{
}

CSubduedState::~CSubduedState()
{
}

void CSubduedState::Update()
{
    CMonster* pMonster = GetMonster();

    // 처형된 순간부터 처형 애니메이션 시작
    if (m_bIsExecuted)
    {
        m_fExecutionTimer += fDT;
        
        // 처형 애니메이션 시간이 지나면 몬스터 삭제
        if (m_fExecutionTimer >= 2.0f) // 2초 후 삭제
        {
            ChangeAIState(GetAI(), MON_STATE::DEAD);
            return;
        }
        
        // 움직임 완전 정지
        if (pMonster->GetRigidBody())
            pMonster->GetRigidBody()->SetVelocity(Vec2(0.f, 0.f));
        return;
    }

    // 마우스 해제시 처형 실행
    if (KEY_AWAY(KEY::LBUTTON))
    {
        StartExecution();
        return;
    }

    // 제압시 물리 설정
    if (m_bIsBeingSubdued && !m_bIsExecuted)
    {
        if (pMonster->GetRigidBody())
            pMonster->GetRigidBody()->SetVelocity(Vec2(0.f, 0.f));
        
        if (pMonster->GetGravity())
            pMonster->GetGravity()->SetApplyGravity(false);
    }
}

void CSubduedState::Enter()
{
    m_bIsBeingSubdued = true;
    m_fExecutionTimer = 0.f;
    m_bIsExecuted = false;
    
    CMonster* pMonster = GetMonster();
    if (pMonster)
    {
        // 애니메이터가 존재하는지 확인 후 사용
        if (pMonster->GetAnimator())
        {
            pMonster->GetAnimator()->Play(L"RIFLEMAN_EXC_START", false);
            pMonster->GetAnimator()->FindAnimation(L"RIFLEMAN_EXC_START")->SetEndFrameEvent([this]() {
                if (GetMonster() && GetMonster()->GetAnimator())
                {
                    GetMonster()->GetAnimator()->Play(L"RIFLEMAN_EXC_BACK", true);
                }
            });
        }

        CShooterHead* pHead = pMonster->GetHead();
        if (pHead && pHead->GetAnimator())
        {
            pHead->GetAnimator()->Reset();
        }

        if (pMonster->GetRigidBody())
            pMonster->GetRigidBody()->SetVelocity(Vec2(0.f, 0.f));
    }
}

void CSubduedState::Exit()
{
    m_bIsBeingSubdued = false;
    m_fExecutionTimer = 0.f;
    
    CMonster* pMonster = GetMonster();
    if (pMonster)
    {
        // 처형되지 않았을 때만 물리 상태 복원
        if (!m_bIsExecuted)
        {
            if (pMonster->GetRigidBody())
                pMonster->GetRigidBody()->SetMaxSpeed(Vec2(300.f, 1000.f));

            if (pMonster->GetGravity())
                pMonster->GetGravity()->SetApplyGravity(true);
        }
    }
    
    // 플레이어의 제압 상태도 정리
    SPlayer* pPlayer = static_cast<SPlayer*>(CSceneMgr::GetInst()->GetCurScene()->GetPlayer());
    if (pPlayer && pPlayer->GetSubduedMonster() == pMonster)
    {
        pPlayer->EndSubdue();
    }
}

void CSubduedState::StartExecution()
{
    m_bIsExecuted = true;
    m_fExecutionTimer = 0.f;
    
    CMonster* pMonster = GetMonster();
    if (!pMonster)
        return;
    
    // 처형 애니메이션 재생 (추후 애니메이션 추가 시 활성화)
    // if (pMonster->GetAnimator())
    // {
    //     pMonster->GetAnimator()->Play(L"RIFLEMAN_EXECUTED", false);
    // }
    
    // 임시로 IDLE 애니메이션 사용
    if (pMonster->GetAnimator())
        pMonster->GetAnimator()->Play(L"RIFLEMAN_IDLE", false);
    
    // 처형 중 완전 정지
    if (pMonster->GetRigidBody())
    {
        pMonster->GetRigidBody()->SetVelocity(Vec2(0.f, 0.f));
        pMonster->GetRigidBody()->SetMaxSpeed(Vec2(0.f, 0.f));
    }

    // 처형 중인 몬스터의 콜라이더를 비활성화하여 갈고리가 통과하도록 함
    if (pMonster->GetCollider())
    {
        pMonster->GetCollider()->SetActive(false);
    }
}
