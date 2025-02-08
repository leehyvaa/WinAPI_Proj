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

// 공통 충돌 계산 함수
float CGround::CalculateCollisionOffset(const Vec2& vObjColPos, const Vec2& vObjColScale,
                                        const Vec2& vGroundColPos, const Vec2& vGroundColScale,
                                        bool isHorizontal)
{
    float fLen = isHorizontal ? abs(vObjColPos.x - vGroundColPos.x) : abs(vObjColPos.y - vGroundColPos.y);
    float fHalfSum = isHorizontal ? (vObjColScale.x + vGroundColScale.x) / 2.f : (vObjColScale.y + vGroundColScale.y) / 2.f;
    return fHalfSum - fLen;
}


// 위에서 충돌했을때(노말 그라운드)
void CGround::NormalTopCollisionEnter(CCollider* _pOther)
{
    GameObject* pOtherObj = _pOther->GetObj();
    Vec2 vObjColPos = _pOther->GetFinalPos();
    Vec2 vObjColScale = _pOther->GetScale();
    Vec2 vGroundColPos = GetCollider()->GetFinalPos();
    Vec2 vGroundColScale = GetCollider()->GetScale();
    Vec2 vObjPos = pOtherObj->GetPos();

    // 플레이어가 땅의 상단에 위치하고 있는지 확인
    if (vObjPos.x >= GetPos().x &&
        vObjPos.x <= GetPos().x + GetScale().x &&
        vObjPos.y <= GetPos().y + COLLISION_TOP_THRESHOLD)
    {
        float fOffset = CalculateCollisionOffset(vObjColPos, vObjColScale, vGroundColPos, vGroundColScale, false);
        vObjPos.y -= fOffset;

        pOtherObj->GetGravity()->SetGround(true);
        static_cast<SPlayer*>(pOtherObj)->SetOnGround(true);
    }
    pOtherObj->SetPos(vObjPos);
    
}


// 밑에서 충돌했을때(노말 그라운드)
void CGround::NormalBotCollisionEnter(CCollider* _pOther)
{
    GameObject* pOtherObj = _pOther->GetObj();
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
        static_cast<SPlayer*>(pOtherObj)->SetOnGround(true);
    }
    pOtherObj->SetPos(vObjPos);

}


// 오른쪽에서 충돌했을때(노말 그라운드)
void CGround::NormalRightCollisionEnter(CCollider* _pOther)
{
    GameObject* pOtherObj = _pOther->GetObj();
    Vec2 vObjColPos = _pOther->GetFinalPos();
    Vec2 vObjColScale = _pOther->GetScale();
    Vec2 vGroundColPos = GetCollider()->GetFinalPos();
    Vec2 vGroundColScale = GetCollider()->GetScale();
    Vec2 vObjPos = pOtherObj->GetPos();

    if (vObjPos.y >= GetPos().y &&
        vObjPos.x >= GetPos().x + GetScale().x - COLLISION_SIDE_THRESHOLD)
    {
        if (vObjPos.y <= GetPos().y + WALL_CLIMB_TOP_OFFSET)
        {
            // 벽타기 가능 구간 (위쪽)
            vObjPos.x += 2.f;
            static_cast<SPlayer*>(pOtherObj)->SetWallClimbing(true); // 벽타기 상태 설정
        }
        else
        {
            // 일반 충돌 처리
            float fOffset = CalculateCollisionOffset(vObjColPos, vObjColScale, vGroundColPos, vGroundColScale, true);
            vObjPos.x += fOffset;
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
    Vec2 vGroundColPos = GetCollider()->GetFinalPos();
    Vec2 vGroundColScale = GetCollider()->GetScale();
    Vec2 vObjPos = pOtherObj->GetPos();

    if (vObjPos.y >= GetPos().y &&
        vObjPos.x <= GetPos().x + COLLISION_SIDE_THRESHOLD)
    {
        if (vObjPos.y <= GetPos().y + WALL_CLIMB_TOP_OFFSET)
        {
            // 벽타기 가능 구간 (위쪽)
            vObjPos.x -= 2.f;
            static_cast<SPlayer*>(pOtherObj)->SetWallClimbing(true); // 벽타기 상태 설정
        }
        else
        {
            // 일반 충돌 처리
            float fOffset = CalculateCollisionOffset(vObjColPos, vObjColScale, vGroundColPos, vGroundColScale, true);
            vObjPos.x -= fOffset;
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
    SPlayer* pPlayer = static_cast<SPlayer*>(pOtherObj);

    // 벽타기 상태에서는 윗면 충돌 처리를 제한
    if (pPlayer->IsWallClimbing())
        return;

    Vec2 vObjColPos = _pOther->GetFinalPos();
    Vec2 vObjColScale = _pOther->GetScale();
    Vec2 vGroundColPos = GetCollider()->GetFinalPos();
    Vec2 vGroundColScale = GetCollider()->GetScale();
    Vec2 vObjPos = pOtherObj->GetPos();

    if (vObjPos.x >= GetPos().x &&
        vObjPos.x <= GetPos().x + GetScale().x &&
        vObjPos.y <= GetPos().y + COLLISION_TOP_THRESHOLD)
    {
        float fOffset = CalculateCollisionOffset(vObjColPos, vObjColScale, vGroundColPos, vGroundColScale, false);
        vObjPos.y -= fOffset;

        pOtherObj->GetGravity()->SetGround(true);
        pPlayer->SetOnGround(true); // 착지 상태 설정
    }
    pOtherObj->SetPos(vObjPos);
  
}


// 밑에서 계속 충돌중일때(노말 그라운드)
void CGround::NormalBotCollision(CCollider* _pOther)
{
    GameObject* pOtherObj = _pOther->GetObj();
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
        static_cast<SPlayer*>(pOtherObj)->SetOnGround(true);
    }
    pOtherObj->SetPos(vObjPos);
}


// 오른쪽에서 계속 충돌중일때(노말 그라운드)
void CGround::NormalRightCollision(CCollider* _pOther)
{
    GameObject* pOtherObj = _pOther->GetObj();
    SPlayer* pPlayer = static_cast<SPlayer*>(pOtherObj);

    // 착지 상태에서는 옆면 충돌 처리를 제한
    if (pPlayer->IsOnGround())
        return;

    Vec2 vObjColPos = _pOther->GetFinalPos();
    Vec2 vObjColScale = _pOther->GetScale();
    Vec2 vGroundColPos = GetCollider()->GetFinalPos();
    Vec2 vGroundColScale = GetCollider()->GetScale();
    Vec2 vObjPos = pOtherObj->GetPos();

    if (vObjPos.y >= GetPos().y &&
        vObjPos.x >= GetPos().x + GetScale().x - COLLISION_SIDE_THRESHOLD)
    {
        if (vObjPos.y <= GetPos().y + WALL_CLIMB_TOP_OFFSET)
        {
            // 옆면 최상단 충돌: 자동 점프
            vObjPos.x += 2.f;
            pPlayer->SetWallClimbing(false);
            pPlayer->SetPlayerState(PLAYER_STATE::JUMP);
            pPlayer->SetDir(-1); // 왼쪽 방향으로 점프
        }
        else if (vObjPos.y <= GetPos().y + GetScale().y - WALL_CLIMB_BOT_OFFSET)
        {
            // 매달리기 가능 구간
            vObjPos.x += 2.f;
            pPlayer->SetWallClimbing(true); // 벽타기 상태 설정
            pPlayer->SetPlayerState(PLAYER_STATE::CLIMB);
            pPlayer->SetDir(-1); // 방향 설정
        }
        else
        {
            // 최하단: 매달리기 불가능, 일반 충돌 처리
            float fOffset = CalculateCollisionOffset(vObjColPos, vObjColScale, vGroundColPos, vGroundColScale, true);
            vObjPos.x += fOffset;
            pPlayer->SetWallClimbing(false);
        }
    }
    pOtherObj->SetPos(vObjPos);
}

// 왼쪽에서 계속 충돌중일때(노말 그라운드)
void CGround::NormalLeftCollision(CCollider* _pOther)
{
    GameObject* pOtherObj = _pOther->GetObj();
    SPlayer* pPlayer = static_cast<SPlayer*>(pOtherObj);

    // 착지 상태에서는 옆면 충돌 처리를 제한
    if (pPlayer->IsOnGround())
        return;

    Vec2 vObjColPos = _pOther->GetFinalPos();
    Vec2 vObjColScale = _pOther->GetScale();
    Vec2 vGroundColPos = GetCollider()->GetFinalPos();
    Vec2 vGroundColScale = GetCollider()->GetScale();
    Vec2 vObjPos = pOtherObj->GetPos();

    if (vObjPos.y >= GetPos().y &&
        vObjPos.x <= GetPos().x + COLLISION_SIDE_THRESHOLD)
    {
        if (vObjPos.y <= GetPos().y + WALL_CLIMB_TOP_OFFSET)
        {
            // 옆면 최상단 충돌: 자동 점프
            vObjPos.x -= 2.f;
            pPlayer->SetWallClimbing(false);
            pPlayer->SetPlayerState(PLAYER_STATE::JUMP);
            pPlayer->SetDir(1); // 오른쪽 방향으로 점프
        }
        else if (vObjPos.y <= GetPos().y + GetScale().y - WALL_CLIMB_BOT_OFFSET)
        {
            // 매달리기 가능 구간
            vObjPos.x -= 2.f;
            pPlayer->SetWallClimbing(true); // 벽타기 상태 설정
            pPlayer->SetPlayerState(PLAYER_STATE::CLIMB);
            pPlayer->SetDir(1); // 방향 설정
        }
        else
        {
            // 최하단: 매달리기 불가능, 일반 충돌 처리
            float fOffset = CalculateCollisionOffset(vObjColPos, vObjColScale, vGroundColPos, vGroundColScale, true);
            vObjPos.x -= fOffset;
            pPlayer->SetWallClimbing(false);
        }
    }
    pOtherObj->SetPos(vObjPos);
}



void CGround::OnCollision(CCollider* _pOther)
{
    GameObject* pOtherObj = _pOther->GetObj();
    SPlayer* pPlayer = static_cast<SPlayer*>(pOtherObj);

    if (pOtherObj->GetGroup() == GROUP_TYPE::PLAYER)
    {
        if (m_eGroundType == GROUND_TYPE::NORMAL)
        {
            switch (m_eCollideType)
            {
            case TILE_COLLIDE_TYPE::TOP_PLATFORM:
                // 상단 충돌 처리 (착지 상태)
                    NormalTopCollision(_pOther);
                break;

            case TILE_COLLIDE_TYPE::SLOPE_LEFT:
                {
                    // 좌측 빗면 처리 (윗면과 좌측 충돌)
                    // 윗면 충돌을 먼저 처리
                    NormalTopCollision(_pOther);

                    // 착지 상태가 아닌 경우에만 좌측 충돌 처리
                    if (!pPlayer->IsOnGround())
                    {
                        NormalLeftCollision(_pOther);
                    }
                }
                break;

            case TILE_COLLIDE_TYPE::SLOPE_RIGHT:
                {
                    // 우측 빗면 처리 (윗면과 우측 충돌)
                    // 윗면 충돌을 먼저 처리
                    NormalTopCollision(_pOther);

                    // 착지 상태가 아닌 경우에만 우측 충돌 처리
                    if (!pPlayer->IsOnGround())
                    {
                        NormalRightCollision(_pOther);
                    }
                }
                break;

            case TILE_COLLIDE_TYPE::BOT_PLATFORM:
                // 하단 충돌 처리 (머리 충돌)
                    NormalBotCollision(_pOther);
                break;

            case TILE_COLLIDE_TYPE::SOLID:
                // 전체 충돌 처리 (필요 시 구현)
                    // HandleFullCollision(pOtherObj);
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
		//static_cast<SPlayer*>(pOtherObj)->SetWallClimbing(false);

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