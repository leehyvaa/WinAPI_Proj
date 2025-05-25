#include "pch.h"
#include "CDeadState.h"

#include "define.h"

void CDeadState::Update()
{
}

void CDeadState::Enter()
{
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
