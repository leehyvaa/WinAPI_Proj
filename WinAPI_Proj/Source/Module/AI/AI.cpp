#include "pch.h"
#include "AI.h"
#include "CState.h"

AI::AI()
	:m_pCurState(nullptr)
	,m_pOwner(nullptr)
{
}

AI::~AI()
{
	Safe_Delete_Map(m_mapState);
}


void AI::Update()
{
    if (m_pCurState)
	    m_pCurState->Update();
}

void AI::Render(HDC _dc)
{
    if (m_pCurState)
        m_pCurState->Render(_dc);
}


void AI::AddState(CState* _pState)
{
	CState* pState = GetState(_pState->GetType());
	assert(!pState);

	m_mapState.insert(make_pair(_pState->GetType(), _pState));
	_pState->m_pAI = this;
	
}

CState* AI::GetState(MON_STATE _eState)
{
	map<MON_STATE, CState*>::iterator iter = m_mapState.find(_eState);
	if (iter == m_mapState.end())
	{
		return nullptr;
	}

	
	return iter->second;
}

MON_STATE AI::GetCurState()
{
    for (auto iter = m_mapState.begin(); iter != m_mapState.end(); ++iter)
    {
        if (iter->second == m_pCurState)
        {
            return iter->first;
        }
    }
	
	return MON_STATE::END;
}

void AI::SetCurState(MON_STATE _eState)
{
	m_pCurState = GetState(_eState);
	assert(m_pCurState);
    m_pCurState->Enter();

}

void AI::ChangeState(MON_STATE _eNextState)
{
	CState* pNextState = GetState(_eNextState);

	assert(m_pCurState != pNextState);

	m_pCurState->Exit();
	m_pCurState = pNextState;
	m_pCurState->Enter();

}
