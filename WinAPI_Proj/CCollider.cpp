#include "pch.h"
#include "CCollider.h"
#include "GameObject.h"
#include "CCore.h"
#include "SelectGDI.h"

UINT CCollider::g_iNextID = 0;


CCollider::CCollider()
	:m_pOwner(nullptr)
	,m_iID(g_iNextID++)
{
}

CCollider::CCollider(const CCollider& _origin)
{
}

CCollider::~CCollider()
{
}

void CCollider::FinalUpdate()
{
	Vec2 vObjectPos = m_pOwner->GetPos();
	m_vFinalPos = vObjectPos + m_vOffsetPos;
}

void CCollider::Render(HDC _dc)
{
	SelectGDI p(_dc, PEN_TYPE::GREEN);
	SelectGDI b(_dc, BRUSH_TYPE::HOLLOW);


	Rectangle(_dc
		,(int)m_vFinalPos.x - m_vScale.x / 2.f
		,(int)m_vFinalPos.y - m_vScale.y / 2.f
		,(int)m_vFinalPos.x + m_vScale.x / 2.f
		,(int)m_vFinalPos.y + m_vScale.y / 2.f);


	
}
