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

//오브젝트를 복사생성자로 만들었을때의 딥카피
CCollider::CCollider(const CCollider& _origin)
	:m_pOwner(nullptr)
	,m_vOffsetPos(_origin.m_vOffsetPos)
	,m_vScale(_origin.m_vScale)
	,m_iID(g_iNextID++)
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
