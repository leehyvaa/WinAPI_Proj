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
const float WALL_CLIMB_BOT_OFFSET = 90.f;

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

        Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(Vec2(GetPos().x + 2, GetPos().y + 2));
        Vec2 vScale = Vec2(GetScale().x - 4.f, GetScale().y - 4.f);

        Rectangle(_dc, static_cast<int>(vRenderPos.x), static_cast<int>(vRenderPos.y), static_cast<int>(vRenderPos.x + vScale.x), static_cast<int>(vRenderPos.y + vScale.y));
    }

    GameObject::Component_Render(_dc);
}

// 공통 충돌 계산 함수
float CGround::CalculateCollisionOffset(const Vec2 &vObjColPos, const Vec2 &vObjColScale,
                                        const Vec2 &vGroundColPos, const Vec2 &vGroundColScale,
                                        bool isHorizontal)
{
    float fLen = isHorizontal ? abs(vObjColPos.x - vGroundColPos.x) : abs(vObjColPos.y - vGroundColPos.y);
    float fHalfSum = isHorizontal ? (vObjColScale.x + vGroundColScale.x) / 2.f : (vObjColScale.y + vGroundColScale.y) / 2.f;
    return fHalfSum - fLen;
}

// 위에서 충돌했을때(노말 그라운드)
void CGround::NormalTopCollisionEnter(CCollider *_pOther)
{
    GameObject *pOtherObj = _pOther->GetObj();
    SPlayer *pPlayer = static_cast<SPlayer *>(pOtherObj);

    // 플레이어가 상승 중이면(수직속도가 음수), 점프 상태(JUMP)라면 착지 처리를 하지 않음
    if (pOtherObj->GetRigidBody()->GetVelocity().y < 0.f || pPlayer->GetState() == PLAYER_STATE::JUMP)
        return;

    Vec2 vObjColPos = _pOther->GetFinalPos();
    Vec2 vObjColScale = _pOther->GetScale();
    Vec2 vGroundColPos = GetCollider()->GetFinalPos();
    Vec2 vGroundColScale = GetCollider()->GetScale();
    Vec2 vObjPos = pOtherObj->GetPos();
    
    
    // 플레이어가 땅의 상단에 위치하고 있는지 확인
    if (vObjPos.x + vObjColScale.x/2>= GetPos().x &&
        vObjPos.x - vObjColScale.x/2<= GetPos().x + GetScale().x &&
        vObjPos.y <= GetPos().y + COLLISION_TOP_THRESHOLD)
    {
        float fOffset = CalculateCollisionOffset(vObjColPos, vObjColScale, vGroundColPos, vGroundColScale, false);
        vObjPos.y -= fOffset;
        //pOtherObj->GetRigidBody()->SetVelocityY(0.f);
        pOtherObj->GetGravity()->SetGround(true);
        static_cast<SPlayer *>(pOtherObj)->SetOnGround(true);
    }
    pOtherObj->SetPos(vObjPos);
}

// 밑에서 충돌했을때(노말 그라운드)
void CGround::NormalBotCollisionEnter(CCollider *_pOther)
{
    GameObject *pOtherObj = _pOther->GetObj();
    Vec2 vObjColPos = _pOther->GetFinalPos();
    Vec2 vObjColScale = _pOther->GetScale();
    Vec2 vGroundColPos = GetCollider()->GetFinalPos();
    Vec2 vGroundColScale = GetCollider()->GetScale();
    Vec2 vObjPos = pOtherObj->GetPos();

    // 플레이어가 땅의 하단에 위치하고 있는지 확인
    if (vObjPos.x >= GetPos().x &&
        vObjPos.x <= GetPos().x + GetScale().x &&
        vObjPos.y >= GetPos().y + GetScale().y - COLLISION_BOT_THRESHOLD)
    {
        float fOffset = CalculateCollisionOffset(vObjColPos, vObjColScale, vGroundColPos, vGroundColScale, false);
        vObjPos.y += fOffset;

        pOtherObj->GetRigidBody()->SetVelocityY(0.f);
        //static_cast<SPlayer *>(pOtherObj)->SetOnGround(true);
    }
    pOtherObj->SetPos(vObjPos);
}

// 오른쪽에서 충돌했을때(노말 그라운드)
void CGround::NormalRightCollisionEnter(CCollider *_pOther)
{
    GameObject *pOtherObj = _pOther->GetObj();
    SPlayer *pPlayer = static_cast<SPlayer *>(pOtherObj);
    Vec2 vObjColPos = _pOther->GetFinalPos();
    Vec2 vObjColScale = _pOther->GetScale();
    Vec2 vGroundPos = GetCollider()->GetFinalPos();
    Vec2 vGroundScale = GetCollider()->GetScale();
    Vec2 vObjPos = pOtherObj->GetPos();
    // 충돌 조건
    if (vObjPos.y > GetPos().y &&
        vObjPos.x >= GetPos().x + GetScale().x - COLLISION_SIDE_THRESHOLD)
    {
        if (vObjPos.y <= GetPos().y + WALL_CLIMB_TOP_OFFSET)
        {
            // 벽타기 가능 구간 (위쪽)
            vObjPos.x += 2.f;
            static_cast<SPlayer *>(pOtherObj)->SetWallClimbing(true); // 벽타기 상태 설정
        }
        else
        {
            // 플레이어의 콜라이더가 벽 바깥으로 나오도록 보정
            float fLen = abs(vObjColPos.x - vGroundPos.x);
            float fValue = (vObjColScale.x / 2.f + vGroundScale.x / 2.f) - fLen;
            vObjPos.x += fValue;
            pPlayer->SetWallClimbing(false);
            // 충돌 후 수평 속도를 0으로 초기화시켜 더 이상 벽으로 밀리지 않게 함
            pOtherObj->GetRigidBody()->SetVelocity(Vec2(0.f, pOtherObj->GetRigidBody()->GetVelocity().y));
        }
    }
    pOtherObj->SetPos(vObjPos);
}

// 왼쪽에서 충돌했을때(노말 그라운드)
void CGround::NormalLeftCollisionEnter(CCollider *_pOther)
{
    GameObject *pOtherObj = _pOther->GetObj();
    SPlayer *pPlayer = static_cast<SPlayer *>(pOtherObj);
    Vec2 vObjColPos = _pOther->GetFinalPos();
    Vec2 vObjColScale = _pOther->GetScale();
    Vec2 vGroundPos = GetCollider()->GetFinalPos();
    Vec2 vGroundScale = GetCollider()->GetScale();
    Vec2 vObjPos = pOtherObj->GetPos();

    // 왼쪽 충돌 조건
    if (vObjPos.y > GetPos().y &&
        vObjPos.x <= GetPos().x + COLLISION_SIDE_THRESHOLD)
    {
        // 벽을 박차고 위로 뛰어올라가는 구간 (벽의 최상단)
        if (vObjPos.y <= GetPos().y + WALL_CLIMB_TOP_OFFSET)
        {
            vObjPos.x -= 2.f;
            pPlayer->SetWallClimbing(false);
        }
        else
        {
            // 플레이어의 콜라이더가 벽 바깥으로 나오도록 보정
            float fLen = abs(vObjColPos.x - vGroundPos.x);
            float fValue = (vObjColScale.x / 2.f + vGroundScale.x / 2.f) - fLen;
            vObjPos.x -= fValue;

            pPlayer->SetWallClimbing(true);

            // 충돌 후 수평 속도를 0으로 초기화하여, 더 이상 왼쪽으로 밀리지 않게 함
            pOtherObj->GetRigidBody()->SetVelocity(Vec2(0.f, pOtherObj->GetRigidBody()->GetVelocity().y));
        }
    }
    pOtherObj->SetPos(vObjPos);
}

void CGround::OnCollisionEnter(CCollider *_pOther)
{
    GameObject *pOtherObj = _pOther->GetObj();
    Vec2 vObjPos = pOtherObj->GetPos();
    Vec2 vObjColScale = _pOther->GetScale();

    
    if (pOtherObj->GetGroup() == GROUP_TYPE::PLAYER)
    {
        SPlayer *pPlayer = static_cast<SPlayer *>(pOtherObj);

        if (m_eGroundType == GROUND_TYPE::NORMAL)
        {
            switch (m_eCollideType)
            {
            case TILE_COLLIDE_TYPE::TOP_PLATFORM:
                // 상단 충돌 처리 (착지 가능)
                if (vObjPos.y <= GetPos().y + COLLISION_TOP_THRESHOLD)
                    NormalTopCollisionEnter(_pOther);
                break;
            case TILE_COLLIDE_TYPE::SLOPE_LEFT:
                // 좌측 빗면 처리 (벽면에 붙음)
                if (vObjPos.y > GetPos().y )
                    NormalLeftCollisionEnter(_pOther);

                // 착지 상태가 아닌 경우에만 우측 충돌 처리
                // if (!pPlayer->IsOnGround())
                //     NormalLeftCollisionEnter(_pOther);
                break;
            case TILE_COLLIDE_TYPE::SLOPE_RIGHT:
                // 우측 빗면 처리 (벽면에 붙음)
                if (vObjPos.y > GetPos().y)
                    NormalRightCollisionEnter(_pOther);

                // 착지 상태가 아닌 경우에만 우측 충돌 처리
                // if (!pPlayer->IsOnGround())
                //     NormalRightCollisionEnter(_pOther);
                break;
            case TILE_COLLIDE_TYPE::BOT_PLATFORM:
                // 하단 충돌 처리 (머리 충돌)
                if (vObjPos.x >= GetPos().x &&
                    vObjPos.x <= GetPos().x + GetScale().x &&
                    vObjPos.y >= GetPos().y + GetScale().y - COLLISION_BOT_THRESHOLD)
                    NormalBotCollisionEnter(_pOther);
                break;
            case TILE_COLLIDE_TYPE::SOLID:
                // 전체 충돌 처리
                // HandleFullCollision(pOtherObj);
                break;
            }
        }
    }
}

// 위에서 계속 충돌중일때(노말 그라운드)
void CGround::NormalTopCollision(CCollider *_pOther)
{
    GameObject *pOtherObj = _pOther->GetObj();
    SPlayer *pPlayer = static_cast<SPlayer *>(pOtherObj);

    // 만약 플레이어가 상승 중이면 (수직속도가 음수) 충돌 업데이트 시에도 착지 처리를 무시
    if (pOtherObj->GetRigidBody()->GetVelocity().y < 0.f)
        return;

    // 벽타기 상태에서는 윗면 충돌 처리를 제한
    if (pPlayer->IsWallClimbing())
        return;

    Vec2 vObjColPos = _pOther->GetFinalPos();
    Vec2 vObjColScale = _pOther->GetScale();
    Vec2 vGroundColPos = GetCollider()->GetFinalPos();
    Vec2 vGroundColScale = GetCollider()->GetScale();
    Vec2 vObjPos = pOtherObj->GetPos();
    
    
    if (vObjPos.x + vObjColScale.x/2>= GetPos().x &&
        vObjPos.x - vObjColScale.x/2<= GetPos().x + GetScale().x &&
        vObjPos.y <= GetPos().y + COLLISION_TOP_THRESHOLD &&
        m_eCollideType == TILE_COLLIDE_TYPE::SLOPE_RIGHT)
    {
        float fOffset = CalculateCollisionOffset(vObjColPos, vObjColScale, vGroundColPos, vGroundColScale, false);
        vObjPos.y -= fOffset;
        pOtherObj->GetRigidBody()->SetVelocityY(0.f);
        pOtherObj->GetGravity()->SetGround(true);
        pPlayer->SetOnGround(true); // 착지 상태 설정
    }
    
    pOtherObj->SetPos(vObjPos);
}

// 밑에서 계속 충돌중일때(노말 그라운드)
void CGround::NormalBotCollision(CCollider *_pOther)
{
    GameObject *pOtherObj = _pOther->GetObj();
    Vec2 vObjColPos = _pOther->GetFinalPos();
    Vec2 vObjColScale = _pOther->GetScale();
    Vec2 vGroundColPos = GetCollider()->GetFinalPos();
    Vec2 vGroundColScale = GetCollider()->GetScale();
    Vec2 vObjPos = pOtherObj->GetPos();

    // 플레이어가 땅의 하단에 위치하고 있는지 확인
    if (vObjPos.x >= GetPos().x &&
        vObjPos.x <= GetPos().x + GetScale().x &&
        vObjPos.y >= GetPos().y + GetScale().y - COLLISION_BOT_THRESHOLD)
    {
        float fOffset = CalculateCollisionOffset(vObjColPos, vObjColScale, vGroundColPos, vGroundColScale, false);
        vObjPos.y += fOffset;

        pOtherObj->GetRigidBody()->SetVelocityY(0.f);
        //static_cast<SPlayer *>(pOtherObj)->SetOnGround(true); //이 부분 고려해봐야함
    }
    pOtherObj->SetPos(vObjPos);
}

// 오른쪽에서 계속 충돌중일때(노말 그라운드)
void CGround::NormalRightCollision(CCollider *_pOther)
{
    GameObject *pOtherObj = _pOther->GetObj();
    SPlayer *pPlayer = static_cast<SPlayer *>(pOtherObj);
    

    Vec2 vObjColPos = _pOther->GetFinalPos();
    Vec2 vObjColScale = _pOther->GetScale();
    Vec2 vGroundColPos = GetCollider()->GetFinalPos();
    Vec2 vGroundColScale = GetCollider()->GetScale();
    Vec2 vObjPos = pOtherObj->GetPos();

    if (vObjPos.y > GetPos().y &&
        vObjPos.x >= GetPos().x + GetScale().x - COLLISION_SIDE_THRESHOLD)
    {
        if (vObjPos.y <= GetPos().y + WALL_CLIMB_TOP_OFFSET)
        {
            // 옆면 최상단 충돌: 플레이어 쪽에서 자동 점프
            vObjPos.x += 2.f;
            pPlayer->SetWallClimbing(false);
            pPlayer->SetDir(-1);
        }
        else if (vObjPos.y <= GetPos().y + GetScale().y - WALL_CLIMB_BOT_OFFSET)
        {
            // 매달리기 가능 구간
            pPlayer->SetWallClimbing(true); // 벽타기 상태 설정
            pPlayer->SetDir(-1); // 방향 설정
            float fLen = abs(vObjColPos.x - vGroundColPos.x);
            float fValue = (vObjColScale.x / 2.f + vGroundColScale.x / 2.f) - fLen;
            vObjPos.x += fValue;
        }
    }
    pOtherObj->SetPos(vObjPos);
}

// 왼쪽에서 계속 충돌중일때(노말 그라운드)
void CGround::NormalLeftCollision(CCollider *_pOther)
{
    GameObject *pOtherObj = _pOther->GetObj();
    SPlayer *pPlayer = static_cast<SPlayer *>(pOtherObj);
    

    Vec2 vObjColPos = _pOther->GetFinalPos();
    Vec2 vObjColScale = _pOther->GetScale();
    Vec2 vGroundColPos = GetCollider()->GetFinalPos();
    Vec2 vGroundColScale = GetCollider()->GetScale();
    Vec2 vObjPos = pOtherObj->GetPos();

    if (vObjPos.y > GetPos().y &&
        vObjPos.x <= GetPos().x + COLLISION_SIDE_THRESHOLD)
    {
        if (vObjPos.y <= GetPos().y + WALL_CLIMB_TOP_OFFSET)
        {
            // 옆면 최상단 충돌: 플레이어 쪽에서 자동 점프
            vObjPos.x -= 2.f;
            pPlayer->SetWallClimbing(false);
            pPlayer->SetDir(1); 
        }
        else if (vObjPos.y <= GetPos().y + GetScale().y - WALL_CLIMB_BOT_OFFSET)
        {
            // 매달리기 가능 구간
            float fLen = abs(vObjColPos.x - vGroundColPos.x);
            float fValue = (vObjColScale.x / 2.f + vGroundColScale.x / 2.f) - fLen;
            vObjPos.x -= fValue;
            pPlayer->SetWallClimbing(true); // 벽타기 상태 설정
            pPlayer->SetDir(1); // 방향 설정
        }
    }
    pOtherObj->SetPos(vObjPos);
}

void CGround::OnCollision(CCollider *_pOther)
{
    GameObject *pOtherObj = _pOther->GetObj();
    Vec2 vObjPos = pOtherObj->GetPos();
    Vec2 vObjColScale = _pOther->GetScale();

    
    if (pOtherObj->GetGroup() == GROUP_TYPE::PLAYER)
    {
        if (m_eGroundType == GROUND_TYPE::NORMAL)
        {
            switch (m_eCollideType)
            {
            case TILE_COLLIDE_TYPE::TOP_PLATFORM:
                {
                    // 상단 충돌 처리 (착지 상태)
                    if (vObjPos.y <= GetPos().y + COLLISION_TOP_THRESHOLD)
                        NormalTopCollision(_pOther);
                }break;
            case TILE_COLLIDE_TYPE::SLOPE_LEFT:
                {
                    if (vObjPos.y > GetPos().y)
                        NormalLeftCollision(_pOther);
                }break;
            case TILE_COLLIDE_TYPE::SLOPE_RIGHT:
                {
                    if (vObjPos.y > GetPos().y)
                        NormalRightCollision(_pOther);
                }break;

            case TILE_COLLIDE_TYPE::BOT_PLATFORM:
                {
                        // 하단 충돌 처리 (머리 충돌)
                    if (vObjPos.x >= GetPos().x &&
                        vObjPos.x <= GetPos().x + GetScale().x &&
                        vObjPos.y >= GetPos().y + GetScale().y - COLLISION_BOT_THRESHOLD)
                        NormalBotCollision(_pOther);
                }break;
            case TILE_COLLIDE_TYPE::SOLID:
                // 전체 충돌 처리 (필요 시 구현)
                // HandleFullCollision(pOtherObj);
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
        if (m_eGroundType == GROUND_TYPE::NORMAL)
        {
            switch (m_eCollideType)
            {
            case TILE_COLLIDE_TYPE::TOP_PLATFORM:
                {
                    pOtherObj->GetGravity()->SetGround(false);
                    static_cast<SPlayer *>(pOtherObj)->SetOnGround(false);
                }break;
            case TILE_COLLIDE_TYPE::SLOPE_LEFT:
                {
                    static_cast<SPlayer *>(pOtherObj)->SetWallClimbing(false);
                }break;
            case TILE_COLLIDE_TYPE::SLOPE_RIGHT:
                {
                    static_cast<SPlayer *>(pOtherObj)->SetWallClimbing(false);
                }break;

            case TILE_COLLIDE_TYPE::BOT_PLATFORM:
                {
                }break;
            }
        }
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