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
    :m_eCollideType(TILE_COLLIDE_TYPE::NONE)
    ,m_eGroundType(GROUND_TYPE::NORMAL)
{
	CreateCollider();
    SetGroup(GROUP_TYPE::GROUND);

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


		if (GetGroundType() == GROUND_TYPE::NORMAL)
		{
			ePen = PEN_TYPE::BLUE;
		}
		else if (GetGroundType() == GROUND_TYPE::UNWALKABLE)
		{
			ePen = PEN_TYPE::PURPLE;
		}
		else if (GetGroundType() == GROUND_TYPE::DAMAGEZONE)
		{
			ePen = PEN_TYPE::ORANGE;
		}
		else if (GetGroundType() == GROUND_TYPE::DEADZONE)
		{
			ePen = PEN_TYPE::RED;
		}

		SelectGDI b(_dc, BRUSH_TYPE::HOLLOW);
		SelectGDI p(_dc, ePen);

		Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(Vec2(GetPos().x + 2, GetPos().y + 2));
		Vec2 vScale = Vec2(GetScale().x - 4.f, GetScale().y - 4.f);



		Rectangle(_dc, static_cast<int>(vRenderPos.x)
			, static_cast<int>(vRenderPos.y)
			, static_cast<int>(vRenderPos.x + vScale.x)
			, static_cast<int>(vRenderPos.y + vScale.y));
	}

	



	GameObject::Component_Render(_dc);
}




// 위에서 충돌했을때(노말 그라운드)
void CGround::NormalTopCollisionEnter(CCollider* _pOther)
{
    
    
    
    GameObject* pOtherObj = _pOther->GetObj();
    Vec2 vObjColPos = _pOther->GetFinalPos();
    Vec2 vObjColScale = _pOther->GetScale();
    Vec2 vPos = GetCollider()->GetFinalPos();
    Vec2 vScale = GetCollider()->GetScale();
    Vec2 vObjPos = pOtherObj->GetPos();
    
    if (vObjPos.x >= GetPos().x &&
            vObjPos.x <= GetPos().x + GetScale().x &&
            vObjPos.y <= GetPos().y+20.f)
    {
        float fLen = abs(vObjColPos.y - vPos.y);
        float fValue = (vObjColScale.y / 2.f + vScale.y / 2.f) - fLen;
    
        pOtherObj->GetGravity()->SetGround(true);
        static_cast<SPlayer*>(pOtherObj)->SetOnGround(true);
    
        vObjPos.y -= (fValue);
    }
    pOtherObj->SetPos(vObjPos);
    
}


// 밑에서 충돌했을때(노말 그라운드)
void CGround::NormalBotCollisionEnter(CCollider* _pOther)
{
    GameObject* pOtherObj = _pOther->GetObj();
    Vec2 vObjColPos = _pOther->GetFinalPos();
    Vec2 vObjColScale = _pOther->GetScale();
    Vec2 vPos = GetCollider()->GetFinalPos();
    Vec2 vScale = GetCollider()->GetScale();
    Vec2 vObjPos = pOtherObj->GetPos();
    
    //아래에서 충돌했을때
    if (vObjPos.x >= GetPos().x &&
        vObjPos.x <= GetPos().x + GetScale().x &&
        vObjPos.y >= GetPos().y +GetScale().y - 15.f)
    {
        float fLen = abs(vObjColPos.y - vPos.y);
        float fValue = (vObjColScale.y / 2.f + vScale.y / 2.f) - fLen;

        static_cast<SPlayer*>(pOtherObj)->SetOnGround(true);

        vObjPos.y += (fValue);
    }
    pOtherObj->SetPos(vObjPos);
}


// 오른쪽에서 충돌했을때(노말 그라운드)
void CGround::NormalRightCollisionEnter(CCollider* _pOther)
{
    GameObject* pOtherObj = _pOther->GetObj();
    Vec2 vObjColPos = _pOther->GetFinalPos();
    Vec2 vObjColScale = _pOther->GetScale();
    Vec2 vPos = GetCollider()->GetFinalPos();
    Vec2 vScale = GetCollider()->GetScale();
    Vec2 vObjPos = pOtherObj->GetPos();

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
    pOtherObj->SetPos(vObjPos);
}


// 왼쪽에서 충돌했을때(노말 그라운드)
void CGround::NormalLeftCollisionEnter(CCollider* _pOther)
{
    GameObject* pOtherObj = _pOther->GetObj();
    Vec2 vObjColPos = _pOther->GetFinalPos();
    Vec2 vObjColScale = _pOther->GetScale();
    Vec2 vPos = GetCollider()->GetFinalPos();
    Vec2 vScale = GetCollider()->GetScale();
    Vec2 vObjPos = pOtherObj->GetPos();
    if (vObjPos.y >= GetPos().y &&
            vObjPos.x <= GetPos().x +20.f)
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
    pOtherObj->SetPos(vObjPos);
}


void CGround::OnCollisionEnter(CCollider* _pOther)
{
	GameObject* pOtherObj = _pOther->GetObj();

    if(pOtherObj->GetGroup() == GROUP_TYPE::PLAYER)
    {
        if (m_eGroundType == GROUND_TYPE::NORMAL)
        {
            switch(m_eCollideType)
            {
            case TILE_COLLIDE_TYPE::TOP_PLATFORM:
                // 상단 충돌 처리 (착지 가능)
                    NormalTopCollisionEnter(_pOther);
                break;
            case TILE_COLLIDE_TYPE::SLOPE_LEFT:
                // 좌측 빗면 처리 (벽면에 붙음)
                {
                    NormalTopCollisionEnter(_pOther);
                    NormalLeftCollisionEnter(_pOther);
                    NormalBotCollisionEnter(_pOther);
                }
                break;
            case TILE_COLLIDE_TYPE::SLOPE_RIGHT:
                // 우측 빗면 처리 (벽면에 붙음)
                {
                    NormalTopCollisionEnter(_pOther);
                    NormalRightCollisionEnter(_pOther);
                    NormalBotCollisionEnter(_pOther);
                }
                break;
            case TILE_COLLIDE_TYPE::BOT_PLATFORM:
                // 하단 충돌 처리 (머리 충돌)
                    NormalBotCollisionEnter(_pOther);
                break;
            case TILE_COLLIDE_TYPE::SOLID:
                // 전체 충돌 처리
                    //HandleFullCollision(pOtherObj);
                        break;
            }
        }
    }
}



// 위에서 계속 충돌중일때(노말 그라운드)
void CGround::NormalTopCollision(CCollider* _pOther)
{
    GameObject* pOtherObj = _pOther->GetObj();
    Vec2 vObjColPos = _pOther->GetFinalPos();
    Vec2 vObjColScale = _pOther->GetScale();
    Vec2 vPos = GetCollider()->GetFinalPos();
    Vec2 vScale = GetCollider()->GetScale();
    Vec2 vObjPos = pOtherObj->GetPos();
    
    //위에서 충돌
    if (vObjPos.x >= GetPos().x &&
        vObjPos.x <= GetPos().x + GetScale().x&&
        vObjPos.y <= GetPos().y+20.f)
    {
        float fLen = abs(vObjColPos.y - vPos.y);
        float fValue = (vObjColScale.y / 2.f + vScale.y / 2.f) - fLen;
    
        vObjPos.y -= (fValue);
    }
    
    
    if(vObjPos.y <= GetPos().y)
    {
        pOtherObj->GetGravity()->SetGround(true);
        static_cast<SPlayer*>(pOtherObj)->SetOnGround(true);
    }
    pOtherObj->SetPos(vObjPos);
}


// 밑에서 계속 충돌중일때(노말 그라운드)
void CGround::NormalBotCollision(CCollider* _pOther)
{
    GameObject* pOtherObj = _pOther->GetObj();
    Vec2 vObjColPos = _pOther->GetFinalPos();
    Vec2 vObjColScale = _pOther->GetScale();
    Vec2 vPos = GetCollider()->GetFinalPos();
    Vec2 vScale = GetCollider()->GetScale();
    Vec2 vObjPos = pOtherObj->GetPos();
    
    if (vObjPos.x >= GetPos().x &&
            vObjPos.x <= GetPos().x + GetScale().x &&
            vObjPos.y >= GetPos().y + GetScale().y - 20.f)
    {
        float fLen = abs(vObjColPos.y - vPos.y);
        float fValue = (vObjColScale.y / 2.f + vScale.y / 2.f) - fLen;

        static_cast<SPlayer*>(pOtherObj)->SetOnGround(true);

        vObjPos.y += (fValue);
    }

    pOtherObj->SetPos(vObjPos);
}


// 오른쪽에서 계속 충돌중일때(노말 그라운드)
void CGround::NormalRightCollision(CCollider* _pOther)
{
    GameObject* pOtherObj = _pOther->GetObj();
    Vec2 vObjColPos = _pOther->GetFinalPos();
    Vec2 vObjColScale = _pOther->GetScale();
    Vec2 vPos = GetCollider()->GetFinalPos();
    Vec2 vScale = GetCollider()->GetScale();
    Vec2 vObjPos = pOtherObj->GetPos();
    
    if (pOtherObj->GetPos().y > GetPos().y &&
            pOtherObj->GetPos().x >= GetPos().x + GetScale().x -20.f)
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
    pOtherObj->SetPos(vObjPos);
}

// 왼쪽에서 계속 충돌중일때(노말 그라운드)
void CGround::NormalLeftCollision(CCollider* _pOther)
{
    GameObject* pOtherObj = _pOther->GetObj();
    Vec2 vObjColPos = _pOther->GetFinalPos();
    Vec2 vObjColScale = _pOther->GetScale();
    Vec2 vPos = GetCollider()->GetFinalPos();
    Vec2 vScale = GetCollider()->GetScale();
    Vec2 vObjPos = pOtherObj->GetPos();
    
    if (pOtherObj->GetPos().y > GetPos().y &&
            pOtherObj->GetPos().x <= GetPos().x+20.f)
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
    pOtherObj->SetPos(vObjPos);
}



void CGround::OnCollision(CCollider* _pOther)
{
	GameObject* pOtherObj = _pOther->GetObj();

    if(pOtherObj->GetGroup() == GROUP_TYPE::PLAYER)
    {
        if (m_eGroundType == GROUND_TYPE::NORMAL)
        {
            switch(m_eCollideType)
            {
            case TILE_COLLIDE_TYPE::TOP_PLATFORM:
                // 상단 충돌 처리 (착지 상태)
                    NormalTopCollision(_pOther);
                break;
            case TILE_COLLIDE_TYPE::SLOPE_LEFT:
                {
                    // 좌측 빗면 처리 (벽면에 붙어있음)
                    NormalTopCollision(_pOther);
                    NormalLeftCollision(_pOther);
                }
                break;
            case TILE_COLLIDE_TYPE::SLOPE_RIGHT:
                {
                    // 우측 빗면 처리 (벽면에 붙어있음)
                    NormalTopCollision(_pOther);
                    NormalRightCollision(_pOther);
                }
                break;
            case TILE_COLLIDE_TYPE::BOT_PLATFORM:
                // 하단 충돌 처리 (머리 충돌)
                    NormalBotCollision(_pOther);
                break;
            case TILE_COLLIDE_TYPE::SOLID:
                // 전체 충돌 처리
                    //HandleFullCollision(pOtherObj);
                        break;
            }
        }
    }
}

void CGround::OnCollisionExit(CCollider* _pOther)
{
	GameObject* pOtherObj = _pOther->GetObj();
	if (pOtherObj->GetName() == L"Player")
	{
		pOtherObj->GetGravity()->SetGround(false);

		static_cast<SPlayer*>(pOtherObj)->SetOnGround(false);

	}
}





// void CGround::Save(FILE* _file)
// {
// 	wstring wstr = GetName();
// 	string str;
//
// 	fprintf(_file, "[Ground]\n");
// 	
// 	fprintf(_file, "[GroundType]\n");
// 	str = string(wstr.begin(), wstr.end());
// 	fprintf(_file, str.c_str());
// 	fprintf(_file, "\n");
//
//
//
// 	fprintf(_file, "[GroundPos]\n");
// 	fprintf(_file, "%f %f\n", GetPos().x,GetPos().y);
// 	fprintf(_file, "[GroundScale]\n");
// 	fprintf(_file, "%f %f\n", GetScale().x, GetScale().y);
// 	fprintf(_file, "\n");
//
//
//
// 	
// }
//
// void CGround::Load(FILE* _file)
// {
// 	char szBuff[256] = {};
// 	string str;
//
// 	FScanf(szBuff, _file);//[Ground]
//
// 	FScanf(szBuff, _file);//[GroundType]
// 	FScanf(szBuff, _file); //TypeName
// 	str = szBuff;
// 	wstring strGroundType = wstring(str.begin(), str.end());
// 	fprintf(_file, "\n");
// 	SetName(strGroundType);
//
//
// 	FScanf(szBuff, _file);//[GroundPos]
//
// 	float x;
// 	float y;
// 	fscanf_s(_file, "%f %f", &x,&y);
// 	SetPos(Vec2(x, y));
// 	FScanf(szBuff, _file);
// 	
// 	FScanf(szBuff, _file); // [GroundScale]
//
// 	fscanf_s(_file, "%f %f", &x, &y);
// 	SetScale(Vec2(x, y));
// 	FScanf(szBuff, _file);
//
// 	FScanf(szBuff, _file);
//
//
// }