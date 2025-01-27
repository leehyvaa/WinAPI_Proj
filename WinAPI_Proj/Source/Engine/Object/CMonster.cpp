#include "pch.h"
#include "CMonster.h"
#include "CTimeMgr.h"
#include "CCollider.h"
#include "AI.h"

CMonster::CMonster()
	:m_tInfo{}
{
	CreateCollider();
	GetCollider()->SetScale(Vec2(100.f, 100.f));
}

CMonster::~CMonster()
{
	if (nullptr != m_pAI)
		delete m_pAI;
}


void CMonster::Update()
{
	if(nullptr != m_pAI)
		m_pAI->Update();
}

void CMonster::SetAI(AI* _AI)
{
	m_pAI = _AI;
	m_pAI->m_pOwner = this;
}

void CMonster::OnCollisionEnter(CCollider* _pOther)
{
	GameObject* pOtherObj = _pOther->GetObj();

	if (pOtherObj->GetName() == L"Wire")
	{
		m_tInfo.fHP -= 1;
		if(m_tInfo.fHP <=0)
			DeleteObject(this);
	}
}
