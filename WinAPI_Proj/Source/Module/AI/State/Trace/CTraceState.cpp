﻿#include "pch.h"
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
	//타겟된 Player를 쫒아간다.
	SPlayer* pPlayer = static_cast<SPlayer*>(CSceneMgr::GetInst()->GetCurScene()->GetPlayer());
	Vec2 vPlayerPos = pPlayer->GetWorldPos();

	Vec2 vMonPos = GetMonster()->GetWorldPos();
	
	Vec2 vMonDir = vPlayerPos - vMonPos;
	if (vMonDir.Length() != 0.f)
	{
		vMonDir.Normalize();

		vMonPos += vMonDir * GetMonster()->GetInfo().fSpeed * fDT;

		GetMonster()->SetWorldPos(vMonPos);
	}
	
}

void CTraceState::Enter()
{
}

void CTraceState::Exit()
{
}

