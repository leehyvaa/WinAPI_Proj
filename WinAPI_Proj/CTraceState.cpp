#include "pch.h"
#include "CTraceState.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "SPlayer.h"
#include "CMonster.h"
#include "CTimeMgr.h"
CTraceState::CTraceState()
	:CState(MON_STATE::TRACE)
{
}

CTraceState::~CTraceState()
{
}


void CTraceState::Update()
{
	//Ÿ�ٵ� Player�� �i�ư���.
	SPlayer* pPlayer = (SPlayer*)CSceneMgr::GetInst()->GetCurScene()->GetPlayer();
	Vec2 vPlayerPos = pPlayer->GetPos();

	Vec2 vMonPos = GetMonster()->GetPos();
	
	Vec2 vMonDir = vPlayerPos - vMonPos;
	if (vMonDir.Length() != 0.f)
	{
		vMonDir.Normalize();

		vMonPos += vMonDir * GetMonster()->GetInfo().fSpeed * fDT;

		GetMonster()->SetPos(vMonPos);
	}
	
}

void CTraceState::Enter()
{
}

void CTraceState::Exit()
{
}

