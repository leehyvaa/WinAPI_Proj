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
    if (!m_bActive)
        return;
    
	Vec2 vObjectPos = m_pOwner->GetWorldPos();
	m_vFinalPos = vObjectPos + m_vOffsetPos;


    // OBB 축 계산
    m_vAxes[0] = Vec2(1.0f, 0.0f).Rotate(m_fRotation);
    m_vAxes[1] = Vec2(0.0f, 1.0f).Rotate(m_fRotation);
    
    // OBB 모서리 계산
    Vec2 halfScale = m_vScale / 2.0f;
    Vec2 extX = m_vAxes[0] * halfScale.x;
    Vec2 extY = m_vAxes[1] * halfScale.y;
    
    m_vCorners[0] = m_vFinalPos - extX - extY; // 좌상단
    m_vCorners[1] = m_vFinalPos + extX - extY; // 우상단
    m_vCorners[2] = m_vFinalPos + extX + extY; // 우하단
    m_vCorners[3] = m_vFinalPos - extX + extY; // 좌하단
    
	assert(0 <= m_iCol);
}

void CCollider::Render(HDC _dc)
{
    if (!m_bActive)
        return;
    
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
