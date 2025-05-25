#include "pch.h"
#include "CMonster.h"
#include "CTimeMgr.h"
#include "CCollider.h"
#include "AI.h"

CMonster::CMonster()
	:m_tInfo{}
{
	CreateCollider();
    CreateRigidBody();
    CreateAnimator();
	GetCollider()->SetScale(Vec2(100.f, 100.f));
    SetGroup(GROUP_TYPE::MONSTER);

    AI* pAI = new AI;
    SetAI(pAI);
}

CMonster::~CMonster()
{
	if (nullptr != m_pAI)
		delete m_pAI;
}


void CMonster::Start()
{
    GameObject::Start();
}

void CMonster::Update()
{

}

void CMonster::Render(HDC _dc)
{
    GameObject::Render(_dc);
}

void CMonster::FinalUpdata()
{
}

void CMonster::Reset()
{
    GameObject::Reset();
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

void CMonster::OnCollision(CCollider* _pOther)
{
    GameObject::OnCollision(_pOther);
}

void CMonster::OnCollisionExit(CCollider* _pOther)
{
    GameObject::OnCollisionExit(_pOther);
}
