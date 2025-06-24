#include "pch.h"
#include "CSpawningState.h"

#include "CAnimation.h"
#include "CAnimator.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "SPlayer.h"
#include "CMonster.h"
#include "CCollider.h"
void CSpawningState::Update()
{
}

void CSpawningState::Enter()
{
    CMonster* pMonster = GetMonster();
    if (pMonster)
    {
        // 스폰 중인 몬스터의 콜라이더를 비활성화하여 갈고리가 통과하도록 함
        if (pMonster->GetCollider())
        {
            pMonster->GetCollider()->SetActive(false);
        }

        if (pMonster->GetAnimator())
        {
            pMonster->GetAnimator()->Play(L"RIFLEMAN_SPAWNING", false);
            pMonster->GetAnimator()->FindAnimation(L"RIFLEMAN_SPAWNING")->SetEndFrameEvent([this]() {
                // 이 람다 함수는 애니메이션이 종료될 때 호출
                ChangeAIState(GetAI(), MON_STATE::IDLE);
            });
        }
    }
}

void CSpawningState::Exit()
{
    // 스폰이 완료되면 콜라이더를 다시 활성화
    CMonster* pMonster = GetMonster();
    if (pMonster && pMonster->GetCollider())
    {
        pMonster->GetCollider()->SetActive(true);
    }
}

CSpawningState::CSpawningState():CState(MON_STATE::SPAWNING)
{
}

CSpawningState::~CSpawningState()
{
}
