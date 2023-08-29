#include "pch.h"
#include "CGround.h"
#include "CCollider.h"
#include "CGravity.h"
#include "CCore.h"
#include "SelectGDI.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "SPlayer.h"
#include "CRigidBody.h"
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
			ePen = PEN_TYPE::PURPLE;
		}
		else if (GetName() == L"DamageZone")
		{
			ePen = PEN_TYPE::ORANGE;
		}
		else if (GetName() == L"DeadZone")
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

		Vec2 vObjColPos = _pOther->GetFinalPos();
		Vec2 vObjColScale = _pOther->GetScale();

		Vec2 vPos = GetCollider()->GetFinalPos();
		Vec2 vScale = GetCollider()->GetScale();

		Vec2 vObjPos = pOtherObj->GetPos();

		/*if (pOtherObj->GetPos().y <= GetPos().y)
		{
			Vec2 pPos = pOtherObj->GetPos();
			Vec2 gPos = GetPos();

			float fLen = abs(vObjPos.y - vPos.y);
			float fValue = (vObjScale.y / 2.f + vScale.y / 2.f) - fLen;


			vObjPos.y -= (fValue);

		}*/

		//위에서 충돌
		if (vObjPos.x >= GetPos().x &&
			vObjPos.x <= GetPos().x + GetScale().x &&
			vObjPos.y <= GetPos().y+15.f)
		{
			float fLen = abs(vObjColPos.y - vPos.y);
			float fValue = (vObjColScale.y / 2.f + vScale.y / 2.f) - fLen;

			((SPlayer*)pOtherObj)->SetOnGround(true);

			vObjPos.y -= (fValue);
		}





		//좌우 충돌했을때

		if (vObjPos.y >= GetPos().y &&
			vObjPos.x <= GetPos().x +15.f)
		{
			
				if (vObjPos.y <= GetPos().y + 70.f)
				{
					vObjPos.x -= 2.f;
				}
				else
				{
					float fLen = abs(vObjColPos.x - vPos.x);
					float fValue = (vObjColScale.x / 2.f + vScale.x / 2.f) - fLen;

					vObjPos.x -= fValue;
				}
			


		

		}

		if (pOtherObj->GetPos().y >= GetPos().y &&
			pOtherObj->GetPos().x >= GetPos().x+GetScale().x -15.f)
		{

			
				if (vObjPos.y <= GetPos().y + 70.f)
				{
					vObjPos.x -= 2.f;

				}
				else
				{
					float fLen = abs(vObjColPos.x - vPos.x);
					float fValue = (vObjColScale.x / 2.f + vScale.x / 2.f) - fLen;

					vObjPos.x += fValue;
				}
			

			


		}
		


		//아래에서 충돌했을때
		if (vObjPos.x >= GetPos().x &&
			vObjPos.x <= GetPos().x + GetScale().x &&
			vObjPos.y >= GetPos().y +GetScale().y - 15.f)
		{
			float fLen = abs(vObjColPos.y - vPos.y);
			float fValue = (vObjColScale.y / 2.f + vScale.y / 2.f) - fLen;

			((SPlayer*)pOtherObj)->SetOnGround(true);

			vObjPos.y += (fValue);
		}


		pOtherObj->SetPos(vObjPos);

	}
}

void CGround::OnCollision(CCollider* _pOther)
{
	GameObject* pOtherObj = _pOther->GetObj();
	if (pOtherObj->GetName() == L"Player")
	{
		pOtherObj->GetGravity()->SetGround(true);

		Vec2 vObjColPos = _pOther->GetFinalPos();
		Vec2 vObjColScale = _pOther->GetScale();

		Vec2 vPos = GetCollider()->GetFinalPos();
		Vec2 vScale = GetCollider()->GetScale();
		
		Vec2 vObjPos = pOtherObj->GetPos();
		Vec2 gPos = GetPos();
				

		//위에서 충돌
		if (vObjPos.x >= GetPos().x &&
			vObjPos.x <= GetPos().x + GetScale().x&&
			vObjPos.y <= GetPos().y+15.f)
		{
			float fLen = abs(vObjColPos.y - vPos.y);
			float fValue = (vObjColScale.y / 2.f + vScale.y / 2.f) - fLen;

			vObjPos.y -= (fValue);
		}


		if(vObjPos.y <= GetPos().y)
			((SPlayer*)pOtherObj)->SetOnGround(true);



		if (pOtherObj->GetPos().y > GetPos().y &&
			pOtherObj->GetPos().x <= GetPos().x+15.f)
		{
			
				if (vObjPos.y >= GetPos().y + GetScale().y + 90.f)
				{
					vObjPos.x -= 2.f;
				}
				else
				{
					float fLen = abs(vObjColPos.x - vPos.x);
					float fValue = (vObjColScale.x / 2.f + vScale.x / 2.f) - fLen;

					vObjPos.x -= fValue;
				}
			


			
			

		}

		if (pOtherObj->GetPos().y > GetPos().y &&
			pOtherObj->GetPos().x >= GetPos().x + GetScale().x -15.f)
		{

			
				if (vObjPos.y >= GetPos().y + GetScale().y + 90.f)
				{
					vObjPos.x += 2.f;
				}
				else
				{
					float fLen = abs(vObjColPos.x - vPos.x);
					float fValue = (vObjColScale.x / 2.f + vScale.x / 2.f) - fLen;

					vObjPos.x += fValue;
				}
			

			
			

		}




		//아래에서 충돌했을때
		if (vObjPos.x >= GetPos().x &&
			vObjPos.x <= GetPos().x + GetScale().x &&
			vObjPos.y >= GetPos().y + GetScale().y - 15.f)
		{
			float fLen = abs(vObjColPos.y - vPos.y);
			float fValue = (vObjColScale.y / 2.f + vScale.y / 2.f) - fLen;

			((SPlayer*)pOtherObj)->SetOnGround(true);

			vObjPos.y += (fValue);
		}

		pOtherObj->SetPos(vObjPos);

	}
}

void CGround::OnCollisionExit(CCollider* _pOther)
{
	GameObject* pOtherObj = _pOther->GetObj();
	if (pOtherObj->GetName() == L"Player")
	{
		pOtherObj->GetGravity()->SetGround(false);

		((SPlayer*)pOtherObj)->SetOnGround(false);

	}
}
