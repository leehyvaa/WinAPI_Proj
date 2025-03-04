#include "pch.h"
#include "CGravity.h"
#include "GameObject.h"
#include"CRigidBody.h"

CGravity::CGravity()
	:m_pOwner(nullptr)
	,m_bApplyGravity(true)
{
}

CGravity::~CGravity()
{
}


void CGravity::SetApplyGravity(bool _b)
{
	m_bApplyGravity = _b;
	// if (m_bApplyGravity) // 중력 적용중일때 
	// {
	// 	Vec2 vV = m_pOwner->GetRigidBody()->GetVelocity();
	//     if (m_pOwner->GetRigidBody()->GetVelocity().y <= 0.f)
	// 	    m_pOwner->GetRigidBody()->SetVelocity(Vec2(vV.x, 0.f));
	// }
}

void CGravity::FinalUpdate()
{
	if (m_bApplyGravity)
	{
		m_pOwner->GetRigidBody()->SetAccelAlpha(Vec2(0.f,1800.f));
	}
}