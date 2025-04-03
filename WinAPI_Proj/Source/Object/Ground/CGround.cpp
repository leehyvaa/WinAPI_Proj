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

const float COLLISION_TOP_THRESHOLD = 20.f;
const float COLLISION_BOT_THRESHOLD = 15.f;
const float COLLISION_SIDE_THRESHOLD = 20.f;
const float WALL_CLIMB_TOP_OFFSET = 70.f;
const float WALL_CLIMB_BOT_OFFSET = 85.f;



CGround::CGround()
    : m_eCollideType(TILE_COLLIDE_TYPE::NONE), m_eGroundType(GROUND_TYPE::NORMAL)
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

        Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(Vec2(GetWorldPos().x + 2, GetWorldPos().y + 2));
        Vec2 vScale = Vec2(GetScale().x - 4.f, GetScale().y - 4.f);

        Rectangle(_dc, static_cast<int>(vRenderPos.x), static_cast<int>(vRenderPos.y), static_cast<int>(vRenderPos.x + vScale.x), static_cast<int>(vRenderPos.y + vScale.y));
    }

    GameObject::Component_Render(_dc);
}
COLLISION_SIDE CGround::DetectCollisionSide(CCollider* _pOther)
{
    const float margin = 5.f; // 경계 영역 마진

    GameObject *pOtherObj = _pOther->GetObj();
    SPlayer *pPlayer = static_cast<SPlayer *>(pOtherObj);
    Vec2 vObjColPos = _pOther->GetFinalPos();
    Vec2 vObjColScale = _pOther->GetScale();
    Vec2 vGroundPos = GetCollider()->GetFinalPos();
    Vec2 vGroundScale = GetCollider()->GetScale();
    Vec2 vObjPos = pOtherObj->GetWorldPos();
    
    bool top = vObjColPos.y < vGroundPos.y + margin;
    bool bottom = vObjColPos.y > vGroundPos.y + vGroundScale.y - margin;
    bool left = vObjColPos.x < vGroundPos.x + margin;
    bool right = vObjColPos.x > vGroundPos.x + vGroundScale.x - margin;

    // 모서리 우선 처리
    if (top && left) return COLLISION_SIDE::TOP;
    if (top && right) return COLLISION_SIDE::TOP;
    if (bottom && left) return COLLISION_SIDE::BOTTOM;
    if (bottom && right) return COLLISION_SIDE::BOTTOM;

    if (top) return COLLISION_SIDE::TOP;
    if (bottom) return COLLISION_SIDE::BOTTOM;
    if (left) return COLLISION_SIDE::LEFT;
    if (right) return COLLISION_SIDE::RIGHT;

    return COLLISION_SIDE::NONE;
}



void CGround::NormalCollisionEnter(CCollider* _pOther)
{
    GameObject *pOtherObj = _pOther->GetObj();
    SPlayer *pPlayer = static_cast<SPlayer *>(pOtherObj);
    Vec2 vObjColPos = _pOther->GetFinalPos();
    Vec2 vObjColScale = _pOther->GetScale();
    Vec2 vGroundColPos = GetCollider()->GetFinalPos();
    Vec2 vGroundColScale = GetCollider()->GetScale();
    Vec2 vObjPos = pOtherObj->GetWorldPos();

    
    // 위에서 충돌
    if (vObjPos.x + vObjColScale.x/2>= GetWorldPos().x &&
        vObjPos.x - vObjColScale.x/2<= GetWorldPos().x + GetScale().x &&
        vObjPos.y <= GetWorldPos().y+COLLISION_TOP_THRESHOLD)
    {
        float fLen = abs(vObjColPos.y - vGroundColPos.y);
        float fValue = (vObjColScale.y / 2.f + vGroundColScale.y / 2.f) - fLen;
        vObjPos.y -= (fValue);
        
        //플레이어가 하강 중이고, 점프 상태가 아닐 때 착지 처리
        if (pOtherObj->GetRigidBody()->GetVelocity().y > 0.f && pPlayer->GetState() != PLAYER_STATE::JUMP)
            static_cast<SPlayer *>(pOtherObj)->SetOnGround(true);
    }

    
    // 왼쪽 충돌
    if (vObjPos.y > GetWorldPos().y &&
        vObjPos.x <= GetWorldPos().x + COLLISION_SIDE_THRESHOLD)
    {
        pPlayer->SetDir(1); 
        float fLen = abs(vObjColPos.x - vGroundColPos.x);
        float fValue = (vObjColScale.x / 2.f + vGroundColScale.x / 2.f) - fLen;
        vObjPos.x -= fValue;
        pOtherObj->GetRigidBody()->SetVelocity(Vec2(0.f, pOtherObj->GetRigidBody()->GetVelocity().y));
        
        // 벽 최상단, 자동 점프로 전환
        if (vObjPos.y <= GetWorldPos().y + WALL_CLIMB_TOP_OFFSET)
            pPlayer->SetWallClimbing(false);
        else if (vObjPos.y <= GetWorldPos().y + GetScale().y + WALL_CLIMB_BOT_OFFSET)// 벽타기 구간
            pPlayer->SetWallClimbing(true);
    }
    
    
    // 오른쪽 충돌
    if (vObjPos.y > GetWorldPos().y &&
        vObjPos.x >= GetWorldPos().x + GetScale().x - COLLISION_SIDE_THRESHOLD)
    {
        pPlayer->SetDir(-1); 
        float fLen = abs(vObjColPos.x - vGroundColPos.x);
        float fValue = (vObjColScale.x / 2.f + vGroundColScale.x / 2.f) - fLen;
        vObjPos.x += fValue;
        pOtherObj->GetRigidBody()->SetVelocity(Vec2(0.f, pOtherObj->GetRigidBody()->GetVelocity().y));

        if (vObjPos.y <= GetWorldPos().y + WALL_CLIMB_TOP_OFFSET)
            pPlayer->SetWallClimbing(false);
        else if (vObjPos.y <= GetWorldPos().y + GetScale().y + WALL_CLIMB_BOT_OFFSET)
            pPlayer->SetWallClimbing(true);
    }
		

    //아래에서 충돌했을때
    if (vObjPos.x >= GetWorldPos().x &&
        vObjPos.x <= GetWorldPos().x + GetScale().x &&
        vObjPos.y-vObjColScale.y >= GetWorldPos().y +GetScale().y - COLLISION_BOT_THRESHOLD)
    {
        float fLen = abs(vObjColPos.y - vGroundColPos.y);
        float fValue = (vObjColScale.y / 2.f + vGroundColScale.y / 2.f) - fLen;
        vObjPos.y += fValue;

        //static_cast<SPlayer *>(pOtherObj)->SetOnGround(true);
        pPlayer->SetMoveEnergy(0.f);
        pOtherObj->GetRigidBody()->SetVelocityY(0.f);

    }

    pOtherObj->SetWorldPos(vObjPos);
}




void CGround::NormalCollision(CCollider* _pOther)
{
    GameObject *pOtherObj = _pOther->GetObj();
    SPlayer *pPlayer = static_cast<SPlayer *>(pOtherObj);
    Vec2 vObjColPos = _pOther->GetFinalPos();
    Vec2 vObjColScale = _pOther->GetScale();
    Vec2 vGroundColPos = GetCollider()->GetFinalPos();
    Vec2 vGroundColScale = GetCollider()->GetScale();
    Vec2 vObjPos = pOtherObj->GetWorldPos();
    
    
    //위에서 충돌
    if (vObjPos.x + vObjColScale.x/2>= GetWorldPos().x &&
        vObjPos.x - vObjColScale.x/2<= GetWorldPos().x + GetScale().x &&
        vObjPos.y <= GetWorldPos().y + COLLISION_TOP_THRESHOLD)
    {
        float fLen = abs(vObjColPos.y - vGroundColPos.y);
        float fValue = (vObjColScale.y / 2.f + vGroundColScale.y / 2.f) - fLen;
        vObjPos.y -= (fValue);
        
        // 플레이어가 하강중이고 벽타기 중이 아니면 착지 처리
        if (pOtherObj->GetRigidBody()->GetVelocity().y > 0.f && !pPlayer->IsWallClimbing())
        {
            pOtherObj->GetGravity()->SetApplyGravity(false);
            pOtherObj->GetRigidBody()->SetVelocityY(0.f);
        }
    }

    
    //왼쪽 충돌
    if (vObjPos.y > GetWorldPos().y &&
        vObjPos.x <= GetWorldPos().x + COLLISION_SIDE_THRESHOLD)
    {
        pPlayer->SetDir(1); 
        float fLen = abs(vObjColPos.x - vGroundColPos.x);
        float fValue = (vObjColScale.x / 2.f + vGroundColScale.x / 2.f) - fLen;
        vObjPos.x -= fValue;
        pOtherObj->GetRigidBody()->SetVelocity(Vec2(0.f, pOtherObj->GetRigidBody()->GetVelocity().y));

        // 벽 최상단, 자동점프 구간
        if (vObjPos.y <= GetWorldPos().y + WALL_CLIMB_TOP_OFFSET)
            pPlayer->SetWallClimbing(false);
        else if (vObjPos.y <= GetWorldPos().y + GetScale().y + WALL_CLIMB_BOT_OFFSET) // 벽타기 구간
            pPlayer->SetWallClimbing(true);
    }
  

    // 오른쪽 충돌
    if (vObjPos.y > GetWorldPos().y &&
        vObjPos.x >= GetWorldPos().x + GetScale().x - COLLISION_SIDE_THRESHOLD)
    {
        pPlayer->SetDir(-1);
        float fLen = abs(vObjColPos.x - vGroundColPos.x);
        float fValue = (vObjColScale.x / 2.f + vGroundColScale.x / 2.f) - fLen;
        vObjPos.x += fValue;
        pOtherObj->GetRigidBody()->SetVelocity(Vec2(0.f, pOtherObj->GetRigidBody()->GetVelocity().y));

        // 벽 최상단, 자동점프 구간
        if (vObjPos.y <= GetWorldPos().y + WALL_CLIMB_TOP_OFFSET)
            pPlayer->SetWallClimbing(false);
        else if (vObjPos.y <= GetWorldPos().y + GetScale().y + WALL_CLIMB_BOT_OFFSET) // 벽타기 구간
            pPlayer->SetWallClimbing(true); // 벽타기 상태 설정
    }
    

    
    //아래에서 충돌했을때
    if (vObjPos.x >= GetWorldPos().x &&
        vObjPos.x <= GetWorldPos().x + GetScale().x &&
        vObjPos.y-vObjColScale.y >= GetWorldPos().y + GetScale().y - COLLISION_BOT_THRESHOLD)
    {
        float fLen = abs(vObjColPos.y - vGroundColPos.y);
        float fValue = (vObjColScale.y / 2.f + vGroundColScale.y / 2.f) - fLen;
        vObjPos.y += (fValue);
        //static_cast<SPlayer *>(pOtherObj)->SetOnGround(true); //이 부분 고려해봐야함
        pPlayer->SetMoveEnergy(0.f);
        pOtherObj->GetRigidBody()->SetVelocityY(0.f);
    }

    pOtherObj->SetWorldPos(vObjPos);
}



void CGround::OnCollisionEnter(CCollider *_pOther)
{
    GameObject *pOtherObj = _pOther->GetObj();
    Vec2 vObjPos = pOtherObj->GetWorldPos();
    Vec2 vObjColScale = _pOther->GetScale();

    
    if (pOtherObj->GetGroup() == GROUP_TYPE::PLAYER)
    {
        SPlayer *pPlayer = static_cast<SPlayer *>(pOtherObj);

        if (m_eGroundType == GROUND_TYPE::NORMAL)
        {
            switch (m_eCollideType)
            {
            case TILE_COLLIDE_TYPE::SOLID:
                // 전체 충돌 처리
                    NormalCollisionEnter(_pOther);
                break;
            }
        }
    }
}

void CGround::OnCollision(CCollider *_pOther)
{
    GameObject *pOtherObj = _pOther->GetObj();
    Vec2 vObjPos = pOtherObj->GetWorldPos();
    Vec2 vObjColScale = _pOther->GetScale();

    
    if (pOtherObj->GetGroup() == GROUP_TYPE::PLAYER)
    {
        if (m_eGroundType == GROUND_TYPE::NORMAL)
        {
            switch (m_eCollideType)
            {

            case TILE_COLLIDE_TYPE::SOLID:
                // 전체 충돌 처리 (필요 시 구현)
                NormalCollision(_pOther);
                break;
            }
        }
    }
}




void CGround::OnCollisionExit(CCollider *_pOther)
{
    GameObject *pOtherObj = _pOther->GetObj();
    if (pOtherObj->GetGroup() == GROUP_TYPE::PLAYER)
    {
        COLLISION_SIDE side = DetectCollisionSide(_pOther);
        pOtherObj->GetGravity()->SetApplyGravity(true);
        static_cast<SPlayer *>(pOtherObj)->SetOnGround(false);
        static_cast<SPlayer *>(pOtherObj)->SetWallClimbing(false);
        
        // switch (side)
        // {
        // case COLLISION_SIDE::TOP:
        //     {
        //         pOtherObj->GetGravity()->SetGround(false);
        //         static_cast<SPlayer *>(pOtherObj)->SetOnGround(false);
        //         static_cast<SPlayer *>(pOtherObj)->SetWallClimbing(false);
        //     }break;
        // case COLLISION_SIDE::LEFT:
        //     {
        //         static_cast<SPlayer *>(pOtherObj)->SetWallClimbing(false);
        //         static_cast<SPlayer *>(pOtherObj)->SetOnGround(false);
        //     }break;
        // case COLLISION_SIDE::RIGHT:
        //     {
        //         static_cast<SPlayer *>(pOtherObj)->SetWallClimbing(false);
        //         static_cast<SPlayer *>(pOtherObj)->SetOnGround(false);
        //     }break;
        // case COLLISION_SIDE::BOTTOM:
        //     {
        //     }break;
        // }
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