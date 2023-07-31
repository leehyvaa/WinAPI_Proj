#include "pch.h"
#include "GameObject.h"
#include "CKeyMgr.h"
#include "CCollider.h"

GameObject::GameObject()
	: m_vPos{}
	, m_vScale{}
	,m_pCollider(nullptr)
{
}
GameObject::~GameObject()
{
	if (nullptr != m_pCollider)
		delete m_pCollider;
}


void GameObject::CreateCollider()
{
	m_pCollider = new CCollider;
	m_pCollider->m_pOwner = this;
}

void GameObject::Update()
{
	if (CKeyMgr::GetInst()->GetKeyState(KEY::W) == KEY_STATE::HOLD);
}

void GameObject::FinalUpdate()
{
	if (m_pCollider)
		m_pCollider->FinalUpdate();
}

void GameObject::Render(HDC _dc)
{

	Component_Render(_dc);
}

void GameObject::Component_Render(HDC _dc)
{
	if (nullptr != m_pCollider)
	{
		m_pCollider->Render(_dc);
	}
}



