#include "pch.h"
#include "CGround.h"
#include "CCollider.h"
#include "CCollisionMgr.h"
#include "CGravity.h"
#include "CCore.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "SPlayer.h"
#include "CRigidBody.h"
#include "CCamera.h"
#include <d2d1.h>

#include "CAnimation.h"
#include "CAnimator.h"
#include "CResMgr.h"



CGround::CGround()
    : m_eCollideType(TILE_COLLIDE_TYPE::NONE), m_eGroundType(GROUND_TYPE::NORMAL)
{
    CreateCollider();
    SetGroup(GROUP_TYPE::GROUND);
    // 기본 CGround는 애니메이터를 생성하지 않음 (필요시 상속 클래스에서 생성)
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

void CGround::Render(ID2D1RenderTarget* _pRenderTarget)
{
    if (!_pRenderTarget)
        return;

    if (CSceneMgr::GetInst()->GetCurScene()->GetDrawGroundType())
    {
        D2D1_COLOR_F color = D2D1::ColorF(D2D1::ColorF::Blue);
        
        if (GetGroundType() == GROUND_TYPE::NORMAL)
        {
            color = D2D1::ColorF(D2D1::ColorF::Blue);
        }
        else if (GetGroundType() == GROUND_TYPE::UNWALKABLE)
        {
            color = D2D1::ColorF(D2D1::ColorF::Purple);
        }
        else if (GetGroundType() == GROUND_TYPE::DAMAGEZONE)
        {
            color = D2D1::ColorF(D2D1::ColorF::Orange);
        }
        else if (GetGroundType() == GROUND_TYPE::DEADZONE)
        {
            color = D2D1::ColorF(D2D1::ColorF::Red);
        }

        // 정적 브러시 캐싱
        static ID2D1SolidColorBrush* s_pGroundBrush = nullptr;
        static D2D1_COLOR_F s_lastColor = {};
        
        if (!s_pGroundBrush || memcmp(&s_lastColor, &color, sizeof(D2D1_COLOR_F)) != 0)
        {
            if (s_pGroundBrush)
                s_pGroundBrush->Release();
            
            _pRenderTarget->CreateSolidColorBrush(color, &s_pGroundBrush);
            s_lastColor = color;
        }

        if (s_pGroundBrush)
        {
            Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(Vec2(GetWorldPos().x + 2, GetWorldPos().y + 2));
            Vec2 vScale = Vec2(GetScale().x - 4.f, GetScale().y - 4.f);

            D2D1_RECT_F rect = D2D1::RectF(
                vRenderPos.x,
                vRenderPos.y,
                vRenderPos.x + vScale.x,
                vRenderPos.y + vScale.y
            );

            _pRenderTarget->DrawRectangle(rect, s_pGroundBrush, 1.0f);
        }
    }

    GameObject::Render(_pRenderTarget);
}







void CGround::OnCollisionEnter(CCollider *_pOther)
{
    // 기본 CGround는 충돌 진입 처리를 하지 않음
    // 상속 클래스에서 구체적인 충돌 진입 처리 구현
}

void CGround::OnCollision(CCollider* _pOther)
{
    // 기본 CGround는 충돌 처리를 하지 않음
    // 상속 클래스에서 구체적인 충돌 처리 구현
}



void CGround::OnCollisionExit(CCollider *_pOther)
{
    // 기본 CGround는 충돌 종료 처리를 하지 않음
    // 상속 클래스에서 구체적인 충돌 종료 처리 구현
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