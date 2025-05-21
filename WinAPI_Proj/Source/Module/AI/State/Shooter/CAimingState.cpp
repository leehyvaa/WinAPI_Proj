#include "pch.h"
#include "CAimingState.h"

#include "CAnimator.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "SPlayer.h"
#include "CMonster.h"
#include "CTimeMgr.h"
#include "Monster/CShooterHead.h"
#include "Monster/CShooterMonster.h"

CAimingState::CAimingState() : CState(MON_STATE::TRACE)
{
    
}

CAimingState::~CAimingState()
{
}

void CAimingState::Enter()
{
    GetMonster()->GetAnimator()->Play(L"RIFLEMAN_AIMING", true);
    dynamic_cast<CShooterMonster*>(GetMonster())->GetHead()->GetAnimator()->Play(L"RIFLEMAN_AIMING_HEAD_TOP", true);
}

void CAimingState::Update()
{
}



void CAimingState::Exit()
{
}
