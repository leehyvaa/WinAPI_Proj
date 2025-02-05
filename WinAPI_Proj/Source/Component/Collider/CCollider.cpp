#include "pch.h"
#include "CCollider.h"
#include "GameObject.h"
#include "CCore.h"
#include "SelectGDI.h"
#include "CCamera.h"

UINT CCollider::g_iNextID = 0;


CCollider::CCollider()
	:m_pOwner(nullptr)
	,m_iID(g_iNextID++)
	,m_iCol(0)
	,m_bActive(true)
{
	
}

//오브젝트를 복사생성자로 만들었을때의 딥카피
CCollider::CCollider(const CCollider& _origin)
	:m_pOwner(nullptr)
	, m_vOffsetPos(_origin.m_vOffsetPos)
	, m_vScale(_origin.m_vScale)
	, m_iID(g_iNextID++)
	, m_iCol(0)
	, m_bActive(true)
{

}

CCollider::~CCollider()
{
}

void CCollider::FinalUpdate()
{
	Vec2 vObjectPos = m_pOwner->GetPos();
	m_vFinalPos = vObjectPos + m_vOffsetPos;



	assert(0 <= m_iCol);
}

void CCollider::Render(HDC _dc)
{
	PEN_TYPE ePen = PEN_TYPE::GREEN;

	if (m_iCol)
		ePen = PEN_TYPE::RED;

	SelectGDI p(_dc, ePen);
	SelectGDI b(_dc, BRUSH_TYPE::HOLLOW);


	Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(m_vFinalPos);

	Rectangle(_dc
		,static_cast<int>(vRenderPos.x - m_vScale.x / 2.f)
		,static_cast<int>(vRenderPos.y - m_vScale.y / 2.f)
		,static_cast<int>(vRenderPos.x + m_vScale.x / 2.f)
		,static_cast<int>(vRenderPos.y + m_vScale.y / 2.f));


	
}

void CCollider::OnCollision(CCollider* _pOther)
{
	m_pOwner->OnCollision(_pOther);
}

void CCollider::OnCollisionEnter(CCollider* _pOther)
{
	++m_iCol;
	m_pOwner->OnCollisionEnter(_pOther);
}

void CCollider::OnCollisionExit(CCollider* _pOther)
{
	--m_iCol;
	m_pOwner->OnCollisionExit(_pOther);
}
