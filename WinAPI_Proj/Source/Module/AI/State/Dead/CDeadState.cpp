#include "pch.h"
#include "CDeadState.h"
#include "CMonster.h"
#include "CCollider.h"

#include "define.h"

void CDeadState::Update()
{
}

void CDeadState::Enter()
{
	// 죽은 몬스터의 콜라이더를 비활성화하여 갈고리가 통과하도록 함
	CMonster* pMonster = GetMonster();
	if (pMonster && pMonster->GetCollider())
	{
		pMonster->GetCollider()->SetActive(false);
	}
}

void CDeadState::Exit()
{
}

CDeadState::CDeadState(): CState(MON_STATE::DEAD)
{
}

CDeadState::~CDeadState()
{
}
