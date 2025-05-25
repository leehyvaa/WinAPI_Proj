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
    // NULL 포인터 체크
    SPlayer* pPlayer = static_cast<SPlayer*>(CSceneMgr::GetInst()->GetCurScene()->GetPlayer());
    if (!pPlayer)
        return;

    CMonster* pMonster = GetMonster();
    if (!pMonster)
        return;

    // 처형 상태 체크 - 이미 처형되었으면 타이머 업데이트
    if (m_bIsExecuted)
    {
        m_fExecutionTimer += fDT;
        
        // 처형 애니메이션 시간이 지나면 몬스터 삭제
        if (m_fExecutionTimer >= 2.0f) // 2초 후 삭제
        {
            // 몬스터를 DEAD 상태로 전환하여 삭제 처리
            ChangeAIState(GetAI(), MON_STATE::DEAD);
            return;
        }
        
        // 처형 중에는 움직임 완전 정지
        if (pMonster->GetRigidBody())
        {
            pMonster->GetRigidBody()->SetVelocity(Vec2(0.f, 0.f));
        }
        return;
    }

    // 마우스 해제 체크 - 즉시 처형 실행 (시간 조건 없음)
    if (KEY_AWAY(KEY::LBUTTON))
    {
        StartExecution();
        return;
    }

    // 제압 중일 때 몬스터가 움직이지 못하도록 속도 초기화
    if (m_bIsBeingSubdued && !m_bIsExecuted)
    {
        if (pMonster->GetRigidBody())
        {
            pMonster->GetRigidBody()->SetVelocity(Vec2(0.f, 0.f));
        }
        
        // 중력 비활성화
        if (pMonster->GetGravity())
        {
            pMonster->GetGravity()->SetApplyGravity(false);
        }
    }
}

void CSubduedState::Enter()
{
    // 제압 상태 진입 시 초기화
    m_bIsBeingSubdued = true;
    m_fExecutionTimer = 0.f;
    m_bIsExecuted = false;
    
    CMonster* pMonster = GetMonster();
    if (pMonster)
    {
        // 제압 상태 애니메이션 재생 (추후 애니메이션 추가 시 활성화)
        // GetMonster()->GetAnimator()->Play(L"RIFLEMAN_SUBDUED", true);
        
        // 임시로 IDLE 애니메이션 사용
        if (pMonster->GetAnimator())
        {
            pMonster->GetAnimator()->Play(L"RIFLEMAN_IDLE", true);
        }
        
        // 몬스터의 물리 상태 조정 - 중력 비활성화
        if (pMonster->GetRigidBody())
        {
            pMonster->GetRigidBody()->SetVelocity(Vec2(0.f, 0.f));
        }
    }
}

void CSubduedState::Exit()
{
    // 제압 상태 종료 시 정리
    m_bIsBeingSubdued = false;
    m_fExecutionTimer = 0.f;
    
    CMonster* pMonster = GetMonster();
    if (pMonster)
    {
        // 처형되지 않았을 때만 물리 상태 복원
        if (!m_bIsExecuted)
        {
            // 물리 상태 복원
            if (pMonster->GetRigidBody())
            {
                // 기본 이동 속도 복원 (몬스터별로 다를 수 있음)
                pMonster->GetRigidBody()->SetMaxSpeed(Vec2(300.f, 1000.f));
            }
            
            // 중력 재활성화
            if (pMonster->GetGravity())
            {
                pMonster->GetGravity()->SetApplyGravity(true);
            }
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
    {
        pMonster->GetAnimator()->Play(L"RIFLEMAN_IDLE", false);
    }
    
    // 처형 중 완전 정지
    if (pMonster->GetRigidBody())
    {
        pMonster->GetRigidBody()->SetVelocity(Vec2(0.f, 0.f));
        pMonster->GetRigidBody()->SetMaxSpeed(Vec2(0.f, 0.f));
    }
    // ChangeAIState(GetAI(), MON_STATE::DEAD); // 크래시 유발 가능성 있는 코드 주석 처리
}
