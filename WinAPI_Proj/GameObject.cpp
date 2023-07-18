#include "pch.h"
#include "GameObject.h"
#include "CKeyMgr.h"

GameObject::GameObject()
	: m_vPos{}
	, m_vScale{}
{
}


GameObject::~GameObject()
{
}

void GameObject::SetTarget(GameObject* _target)
{
	m_target.push_back(_target);
}

void GameObject::Update()
{
	if (CKeyMgr::GetInst()->GetKeyState(KEY::W) == KEY_STATE::HOLD);
}

void GameObject::Render(HDC _dc)
{
}



