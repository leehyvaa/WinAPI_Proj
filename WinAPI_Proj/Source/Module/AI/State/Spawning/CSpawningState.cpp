#include "pch.h"
#include "CSpawningState.h"

#include "CAnimation.h"
#include "CAnimator.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "SPlayer.h"
#include "CMonster.h"
void CSpawningState::Update()
{
}

void CSpawningState::Enter()
{
    GetMonster()->GetAnimator()->Play(L"RIFLEMAN_SPAWNING", false);
    GetMonster()->GetAnimator()->FindAnimation(L"RIFLEMAN_SPAWNING")->SetEndFrameEvent([this]() {
        // 이 람다 함수는 애니메이션이 종료될 때 호출
        ChangeAIState(GetAI(), MON_STATE::IDLE);
    });
}

void CSpawningState::Exit()
{
}

CSpawningState::CSpawningState():CState(MON_STATE::SPAWNING)
{
}

CSpawningState::~CSpawningState()
{
}
