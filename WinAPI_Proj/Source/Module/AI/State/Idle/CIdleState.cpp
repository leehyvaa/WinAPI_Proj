#include "pch.h"
#include "CIdleState.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "SPlayer.h"
#include "CMonster.h"
CIdleState::CIdleState()
	:CState(MON_STATE::IDLE)
{
}

CIdleState::~CIdleState()
{
}


void CIdleState::Update()
{
	//Player의 위치 체크
	SPlayer*  pPlayer = static_cast<SPlayer*>(CSceneMgr::GetInst()->GetCurScene()->GetPlayer());
	Vec2 vPlayerPos = pPlayer->GetWorldPos();

	//일정 범위 안에 들어오면 추적상태로 전환
	CMonster* pMonster = GetMonster();
	Vec2 vMonPos = pMonster->GetWorldPos();

	Vec2 vDiff = vPlayerPos - vMonPos;
	float fLen = vDiff.Length();

	if (fLen < pMonster->GetInfo().fDetectRange)
	{
		ChangeAIState(GetAI(), MON_STATE::TRACE);
	}


}

void CIdleState::Enter()
{
}

void CIdleState::Exit()
{
}


