#include "pch.h"
#include "CGround.h"
#include "CCollider.h"
#include "CGravity.h"
#include "CCore.h"
#include "SelectGDI.h"
#include "CSceneMgr.h"
#include "CScene.h"
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
	if (CSceneMgr::GetInst()->GetCurScene()->GetDrawGroundType())
	{
		PEN_TYPE ePen = PEN_TYPE::BLUE;


		if (GetName() == L"Ground")
		{
			ePen = PEN_TYPE::BLUE;
		}
		else if (GetName() == L"NonGround")
		{
			ePen = PEN_TYPE::RED;
		}
		SelectGDI b(_dc, BRUSH_TYPE::HOLLOW);
		SelectGDI p(_dc, ePen);

		Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(Vec2(GetPos().x + 2, GetPos().y + 2));
		Vec2 vScale = Vec2(GetScale().x - 4.f, GetScale().y - 4.f);



		Rectangle(_dc, (int)(vRenderPos.x)
			, (int)(vRenderPos.y)
			, (int)(vRenderPos.x + vScale.x)
			, (int)(vRenderPos.y + vScale.y));
	}

	



	GameObject::Component_Render(_dc);
}

void CGround::Save(FILE* _file)
{
	wstring wstr = GetName();
	string str;

	fprintf(_file, "[Ground]\n");
	
	fprintf(_file, "[GroundType]\n");
	str = string(wstr.begin(), wstr.end());
	fprintf(_file, str.c_str());
	fprintf(_file, "\n");



	fprintf(_file, "[GroundPos]\n");
	fprintf(_file, "%f %f\n", GetPos().x,GetPos().y);
	fprintf(_file, "[GroundScale]\n");
	fprintf(_file, "%f %f\n", GetScale().x, GetScale().y);
	fprintf(_file, "\n");



	
}

void CGround::Load(FILE* _file)
{
	char szBuff[256] = {};
	string str;

	FScanf(szBuff, _file);//[Ground]

	FScanf(szBuff, _file);//[GroundType]
	FScanf(szBuff, _file); //TypeName
	str = szBuff;
	wstring strGroundType = wstring(str.begin(), str.end());
	fprintf(_file, "\n");
	SetName(strGroundType);


	FScanf(szBuff, _file);//[GroundPos]

	float x;
	float y;
	fscanf_s(_file, "%f %f", &x,&y);
	SetPos(Vec2(x, y));
	FScanf(szBuff, _file);
	
	FScanf(szBuff, _file); // [GroundScale]

	fscanf_s(_file, "%f %f", &x, &y);
	SetScale(Vec2(x, y));
	FScanf(szBuff, _file);

	FScanf(szBuff, _file);


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
		vObjPos.y -= (fValue);

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
