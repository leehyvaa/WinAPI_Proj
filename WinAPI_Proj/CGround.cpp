#include "pch.h"
#include "CGround.h"
#include "CCollider.h"
#include "CGravity.h"
#include "CCore.h"
#include "SelectGDI.h"
#include "CSceneMgr.h"

CGround::CGround()
{
	CreateCollider();

}

CGround::~CGround()
{
}



void CGround::Start()
{
	GetCollider()->SetScale(Vec2(GetScale()));
	GetCollider()->SetOffsetPos(Vec2(GetScale().x / 2, GetScale().y / 2));

}

void CGround::Update()
{
}

void CGround::Render(HDC _dc)
{
	//if(CSceneMgr::GetCurScene()==SCENE_TYPE::TOOL)

	//if (GetName() == L"GROUND")
	//{
	//}


	PEN_TYPE ePen = PEN_TYPE::BLUE;
	SelectGDI p(_dc, ePen);
	SelectGDI b(_dc, BRUSH_TYPE::HOLLOW);

	Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(Vec2(GetPos().x +2,GetPos().y +2));
	Vec2 vScale = Vec2(GetScale().x - 4.f, GetScale().y - 4.f);
	

	

	Rectangle(_dc, (int)(vRenderPos.x)
		, (int)(vRenderPos.y)
		, (int)(vRenderPos.x + vScale.x)
		, (int)(vRenderPos.y + vScale.y));

	GameObject::Component_Render(_dc);
}

void CGround::OnCollisionEnter(CCollider* _pOther)
{
	GameObject* pOtherObj = _pOther->GetObj();
	if (pOtherObj->GetName() == L"Player")
	{
		pOtherObj->GetGravity()->SetGround(true);

		Vec2 vObjPos = _pOther->GetFinalPos();
		Vec2 vObjScale = _pOther->GetScale();

		Vec2 vPos = GetCollider()->GetFinalPos();
		Vec2 vScale = GetCollider()->GetScale();

		float fLen = abs(vObjPos.y - vPos.y);
		float fValue = (vObjScale.y / 2.f + vScale.y / 2.f) - fLen;

		vObjPos = pOtherObj->GetPos();
		vObjPos.y-= (fValue);
	
		pOtherObj->SetPos(vObjPos);
	}
	
}

void CGround::OnCollision(CCollider* _pOther)
{
	GameObject* pOtherObj = _pOther->GetObj();
	if (pOtherObj->GetName() == L"Player")
	{
		pOtherObj->GetGravity()->SetGround(true);

		Vec2 vObjPos = _pOther->GetFinalPos();
		Vec2 vObjScale = _pOther->GetScale();

		Vec2 vPos = GetCollider()->GetFinalPos();
		Vec2 vScale = GetCollider()->GetScale();

		float fLen = abs(vObjPos.y - vPos.y);
		float fValue = (vObjScale.y / 2.f + vScale.y / 2.f) - fLen;


		vObjPos = pOtherObj->GetPos();
		vObjPos.y -= (fValue);

		pOtherObj->SetPos(vObjPos);

	}
}

void CGround::OnCollisionExit(CCollider* _pOther)
{
	GameObject* pOtherObj = _pOther->GetObj();
	if (pOtherObj->GetName() == L"Player")
	{
		pOtherObj->GetGravity()->SetGround(false);

		
	}
}