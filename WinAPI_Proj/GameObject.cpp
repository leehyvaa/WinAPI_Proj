#include "GameObject.h"
#include "CKeyMgr.h"
#include "CCollider.h"
#include "CAnimator.h"
GameObject::GameObject()
	: m_vPos{}
	, m_vScale{}
	,m_pCollider(nullptr)
	,m_pAnimator(nullptr)
	,m_bAlive(true)
{
}
GameObject::GameObject(const GameObject& _origin)
	:m_strName(_origin.m_strName)
	,m_vPos(_origin.m_vPos)
	,m_vScale(_origin.m_vScale)
	,m_pCollider(nullptr)
	,m_pAnimator(nullptr)
	,m_bAlive(true)
{
	if (_origin.m_pCollider)
	{
		m_pCollider = new CCollider(*_origin.m_pCollider);
		m_pCollider->m_pOwner = this;
	}
	if (_origin.m_pAnimator)
	{
		m_pAnimator = new CAnimator(*_origin.m_pAnimator);
		m_pAnimator->m_pOwner = this;
	}
}
GameObject::~GameObject()
{
	if (nullptr != m_pCollider)
		delete m_pCollider;
	if (nullptr != m_pAnimator)
		delete m_pAnimator;
}


void GameObject::CreateCollider()
{
	m_pCollider = new CCollider;
	m_pCollider->m_pOwner = this;
}

void GameObject::CreateAnimator()
{
	m_pAnimator = new CAnimator;
	m_pAnimator->m_pOwner = this;
}

void GameObject::Update()
{
	
}

void GameObject::FinalUpdate()
{
	if (m_pCollider)
		m_pCollider->FinalUpdate();

	if (m_pAnimator)
		m_pAnimator->FinalUpdate();

}

void GameObject::Render(HDC _dc)
{

	Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(m_vPos);

	Rectangle(_dc, (int)(vRenderPos.x - m_vScale.x / 2.f)
		, (int)(vRenderPos.y - m_vScale.y / 2.f)
		, (int)(vRenderPos.x + m_vScale.x / 2.f)
		, (int)(vRenderPos.y + m_vScale.y / 2.f));


	Component_Render(_dc);
}

void GameObject::Component_Render(HDC _dc)
{
	if (nullptr != m_pCollider)
	{
		m_pCollider->Render(_dc);
	}
	if (nullptr != m_pAnimator)
	{
		m_pAnimator->Render(_dc);
	}
}



