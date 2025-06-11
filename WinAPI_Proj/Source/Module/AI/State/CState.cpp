#include "pch.h"
#include "CState.h"
#include "CMonster.h"
#include "AI.h"

void CState::Render(ID2D1RenderTarget* _pRenderTarget)
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
