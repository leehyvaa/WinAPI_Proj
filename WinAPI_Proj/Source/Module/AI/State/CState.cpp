#include "pch.h"
#include "CState.h"
#include "CMonster.h"
#include "AI.h"

void CState::Render(HDC _dc)
{
}

CState::CState(MON_STATE _eState)
	:m_eState(_eState)
{
}

CState::~CState()
{
}
CMonster* CState::GetMonster()
{
	return m_pAI->GetOwner();
}
