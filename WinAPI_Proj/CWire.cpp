#include "pch.h"
#include "CWire.h"
#include "CTimeMgr.h"
#include "CCollider.h"

CWire::CWire()
	:m_fSpeed(200)
	,m_vDir(Vec2(1.f,1.f))
{
	m_vDir.normalize();

	CreateCollider();
	GetCollider()->SetOffsetPos(Vec2());
	GetCollider()->SetScale(Vec2(20.f, 20.f));
}

CWire::~CWire()
{

}

void CWire::Update()
{
	Vec2 vPos = GetPos();

	vPos.x = vPos.x + m_fSpeed *m_vDir.x * fDT;
	vPos.y = vPos.y + m_fSpeed * m_vDir.y * fDT;


	SetPos(vPos);
}

void CWire::Render(HDC _dc)
{
	Vec2 vPos = GetPos();
	Vec2 vScale = GetScale();

	Rectangle(_dc
		,(int)(vPos.x - vScale.x)
		,(int)(vPos.y - vScale.y)
		,(int)(vPos.x + vScale.x)
		,(int)(vPos.y + vScale.y));

	Component_Render(_dc);
}

void CWire::OnCollisionEnter(CCollider* _pOther)
{
	GameObject* pOtherObj = _pOther->GetObj();

	if (pOtherObj->GetName() == L"Monster")
	{
		DeleteObject(this);
	}
}
