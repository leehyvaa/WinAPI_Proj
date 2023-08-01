#include "pch.h"
#include "CMonster.h"
#include "CTimeMgr.h"
#include "CCollider.h"

CMonster::CMonster()
	:m_vCenterPos(Vec2(0.f,0.f))
	,m_fSpeed(100.f)
	,m_fMaxDistance(50.f)
	,m_iDir(1)
	,m_iHP(5)
{
	CreateCollider();
	GetCollider()->SetScale(Vec2(100.f, 100.f));
}

CMonster::~CMonster()
{

}


void CMonster::Update()
{
	Vec2 vCurPos = GetPos();
	/*vCurPos.x += fDT * m_fSpeed * m_iDir;


	float fDist = abs(m_vCenterPos.x - vCurPos.x) - m_fMaxDistance;
	if (0.f < fDist)
	{
		m_iDir *= -1;
		vCurPos.x += fDist * m_iDir;
	}*/

	SetPos(vCurPos);
}

void CMonster::OnCollisionEnter(CCollider* _pOther)
{
	GameObject* pOtherObj = _pOther->GetObj();

	if (pOtherObj->GetName() == L"Wire")
	{
		m_iHP -= 1;
		if(m_iHP<=0)
			DeleteObject(this);
	}
}
