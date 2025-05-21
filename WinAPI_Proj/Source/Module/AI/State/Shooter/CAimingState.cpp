#include "pch.h"
#include "CAimingState.h"

#include "CAnimation.h"
#include "CAnimator.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "SPlayer.h"
#include "CMonster.h"
#include "CTimeMgr.h"
#include "Monster/CShooterHead.h"
#include "Monster/CShooterMonster.h"

CAimingState::CAimingState() : CState(MON_STATE::AIMING)
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
}

void CAimingState::Update()
{
}



void CAimingState::Exit()
{
}
