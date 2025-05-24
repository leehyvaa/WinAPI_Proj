#include "pch.h"
#include "CAimingState.h"

#include "AI.h"
#include "CAnimation.h"
#include "CAnimator.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "SPlayer.h"
#include "CMonster.h"
#include "CTimeMgr.h"
#include "Monster/CShooterHead.h"
#include "Monster/CShooterMonster.h"
#include "CBullet.h" // 총알 클래스 추가
#include "CObjectPool.h" // 오브젝트 풀 추가

CAimingState::CAimingState() : CState(MON_STATE::AIMING)
    , m_fShotDelay(1.0f) // 1초마다 발사
    , m_fShotTimer(0.f)
{
    
}

CAimingState::~CAimingState()
{
}

void CAimingState::Enter()
{
    // 몸체 애니메이션 설정
    GetMonster()->GetAnimator()->Play(L"RIFLEMAN_AIMING_BODY", false);
    GetMonster()->GetAnimator()->FindAnimation(L"RIFLEMAN_AIMING_BODY")->SetEndFrameEvent([this]() {
               GetMonster()->GetAnimator()->Play(L"RIFLEMAN_AIMING_BODY_STOP", true);
    });
    

    // 머리 애니메이션 설정
    dynamic_cast<CShooterMonster*>(GetMonster())->GetHead()->GetAnimator()->Play(L"RIFLEMAN_AIMING_HEAD_TOP", false);
    dynamic_cast<CShooterMonster*>(GetMonster())->GetHead()->GetAnimator()->FindAnimation(L"RIFLEMAN_AIMING_HEAD_TOP")->SetEndFrameEvent([this]() {
           dynamic_cast<CShooterMonster*>(GetMonster())->GetHead()->GetAnimator()->Play(L"RIFLEMAN_AIMING_HEAD_TOP_STOP", true);
    });

    m_fShotTimer = 0.f; // 상태 진입 시 타이머 초기화
}

void CAimingState::Update()
{
    // 플레이어 객체 가져오기
    SPlayer* pPlayer = dynamic_cast<SPlayer*>(CSceneMgr::GetInst()->GetCurScene()->GetPlayer());
    CShooterMonster* pMonster = dynamic_cast<CShooterMonster*>(GetMonster());

    if (pPlayer && pMonster && pMonster->GetHead())
    {
        // 몬스터 헤드가 플레이어를 바라보도록 업데이트
        pMonster->GetHead()->LookAt(pPlayer->GetWorldPos());

        // 총알 발사 타이머 업데이트
        m_fShotTimer += fDT;
        if (m_fShotTimer >= m_fShotDelay)
        {
            FireBullet();
            m_fShotTimer = 0.f; // 타이머 리셋
            // 발사 후 IDLE 상태로 전환 (또는 다른 상태)
            //ChangeAIState(GetAI(), MON_STATE::IDLE);
        }
    }
    else
    {
        // 플레이어가 없거나 몬스터 헤드가 없으면 IDLE 상태로 전환
        //ChangeAIState(GetAI(), MON_STATE::IDLE);
    }
}

void CAimingState::FireBullet()
{
    CBullet* pBullet = dynamic_cast<CBullet*>(CObjectPool::GetInst()->GetPoolObject(L"ShooterBullet"));
    if (pBullet)
    {
        CShooterMonster* pMonster = dynamic_cast<CShooterMonster*>(GetMonster());
        if (pMonster && pMonster->GetHead())
        {
            // 총알의 초기 위치를 몬스터 헤드의 위치로 설정
            if (GetAI()->GetOwner()->GetIsFacingRight())
                pBullet->SetWorldPos(pMonster->GetHead()->GetWorldPos()+Vec2(50.f,5.0f));
            else
                pBullet->SetWorldPos(pMonster->GetHead()->GetWorldPos()+Vec2(-50.f,5.0f));

                
            // 총알의 속성을 소유자 몬스터에 따라 설정 (속도, 데미지, 텍스처 등)
            pBullet->SetBulletInfo(pMonster);
            
            // 씬에 총알 추가
            CreateObject(pBullet, GROUP_TYPE::PROJ_MONSTER);
        }
        else
        {
            // 총알을 가져왔지만 발사할 수 없는 경우 다시 풀로 반환
            CObjectPool::GetInst()->ReturnObject(pBullet);
        }
    }
}


void CAimingState::Exit()
{
    // 상태 종료 시 필요한 정리 작업
    // 예: 애니메이션 종료 이벤트 해제 등
    // if (GetMonster()->GetAnimator()->FindAnimation(L"RIFLEMAN_AIMING_BODY"))
    // {
    //     GetMonster()->GetAnimator()->FindAnimation(L"RIFLEMAN_AIMING_BODY")->SetEndFrameEvent(nullptr);
    // }
    // if (dynamic_cast<CShooterMonster*>(GetMonster())->GetHead()->GetAnimator()->FindAnimation(L"RIFLEMAN_AIMING_HEAD_TOP"))
    // {
    //     dynamic_cast<CShooterMonster*>(GetMonster())->GetHead()->GetAnimator()->FindAnimation(L"RIFLEMAN_AIMING_HEAD_TOP")->SetEndFrameEvent(nullptr);
    // }
}
