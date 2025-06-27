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
                    //NormalCollisionEnter(_pOther);
                break;
            }
        }
    }
}

void CGround::OnCollision(CCollider* _pOther)
{
    GameObject* pOtherObj = _pOther->GetObj();
    if (pOtherObj->GetGroup() != GROUP_TYPE::PLAYER &&
        pOtherObj->GetGroup() != GROUP_TYPE::MONSTER) return;

    //SPlayer* pPlayer = static_cast<SPlayer*>(pOtherObj);
    CCollider* pOtherCollider = _pOther;
    CCollider* pGroundCollider = GetCollider();

    Vec2 mtvDirection;
    float mtvDepth;
    
    // Player를 Ground로부터 밀어내는 MTV 계산
    bool bColliding = CCollisionMgr::GetInst()->
        CalculateCollisionInfo(pOtherCollider, pGroundCollider, mtvDirection, mtvDepth);

    if (bColliding)
    {
        Vec2 vObjPos = pOtherObj->GetWorldPos();
        // 땅 충돌체의 최종 월드 좌표 (중심점)
        Vec2 vGroundColPos = pGroundCollider->GetFinalPos();
        CRigidBody* pOtherRigidBody = pOtherObj->GetRigidBody();

        // 땅의 위쪽 방향 벡터 (Y+가 아래쪽이므로 위는 -Y)
        Vec2 groundUpNormal = Vec2(0.f, -1.f);
        // MTV가 위쪽 방향(groundUpNormal)과 얼마나 일치하는지 (1.0에 가까울수록 위쪽)
        float verticalDot = mtvDirection.Dot(groundUpNormal);
        
        // MTV가 오른쪽 방향(Vec2(1,0))과 얼마나 일치하는지 (1.0에 가까울수록 오른쪽)
        Vec2 rightNormal = Vec2(1.f, 0.f);

        float horizontalDot = mtvDirection.Dot(rightNormal);

        // 충돌 방향 분류를 위한 임계값 (필요에 따라 조정)
        const float directionThreshold = 0.707f; // 약 45도 각도 기준
        
        // 수직 충돌 (MTV가 위/아래 방향에 더 가까울 때)
        if (abs(verticalDot) > directionThreshold)
        {
            // 플레이어 중심이 땅 중심보다 위에 있는지 확인
            if (vObjPos.y < vGroundColPos.y) // 플레이어가 땅보다 위에 있음 -> 윗면 충돌 (Top Collision)
            {
                // 플레이어를 위로 밀어낸다.
                // MTV는 플레이어를 땅에서 밀어내는 방향이므로, 윗면 충돌 시 위쪽을 향함.
                vObjPos += mtvDirection * mtvDepth;
                pOtherObj->SetWorldPos(vObjPos);

                // 착지 처리 (플레이어가 아래로 이동 중이거나 정지 상태일 때)
                if (pOtherRigidBody && pOtherRigidBody->GetVelocity().y >= 0.f)
                {
                    SPlayer* pPlayer = static_cast<SPlayer*>(pOtherObj);
                    pOtherRigidBody->SetVelocityY(0.f); // Y 속도 0
                    pOtherObj->GetGravity()->SetApplyGravity(false); // 중력 비활성화
                    if (pOtherObj->GetGroup() == GROUP_TYPE::PLAYER)
                    {
                        pPlayer->SetOnGround(true); // 땅 위에 있음
                        pPlayer->SetWallClimbing(false); // 벽타기 상태 해제
                    }
                }
                // 플레이어가 위로 점프하다가 윗면 모서리에 걸린 경우,
                // 위치 보정만 하고 착지 처리는 하지 않음 (계속 상승 가능)
            }
            else // 플레이어가 땅보다 아래에 있음 -> 아랫면 충돌
            {
                // 플레이어를 아래로 밀어낸다.
                Vec2 pushDirection = Vec2(0.f, 1.f); // 아래 방향
                vObjPos += pushDirection * mtvDepth;
                pOtherObj->SetWorldPos(vObjPos);

                // 상승 중이었다면 Y축 속도를 0으로
                if (pOtherRigidBody && pOtherRigidBody->GetVelocity().y < 0.f)
                    pOtherRigidBody->SetVelocityY(0.f);

                if (pOtherObj->GetGroup() == GROUP_TYPE::PLAYER)
                {
                    SPlayer* pPlayer = static_cast<SPlayer*>(pOtherObj);
                    pPlayer->SetMoveEnergy(0.f); // 운동 에너지 초기화

                    // 땅 위가 아님 상태 설정
                    pPlayer->SetOnGround(false);
                    pPlayer->SetWallClimbing(false);
                }
                
            }
        }
        // 수평 충돌 (MTV가 왼쪽/오른쪽 방향에 가까움)
        else if (abs(horizontalDot) > directionThreshold)
        {
            // 플레이어를 옆으로 밀어낸다.
            // MTV는 플레이어를 땅에서 밀어내는 방향이므로, 측면 충돌 시 옆쪽을 향함.
            vObjPos += mtvDirection * mtvDepth;
            pOtherObj->SetWorldPos(vObjPos);

            // 벽에 부딪히는 수평 속도 제거
            if (pOtherRigidBody)
            {
                Vec2 velocity = pOtherRigidBody->GetVelocity();
                // MTV 방향(법선)으로의 속도 성분 계산
                Vec2 normalVelocity = mtvDirection * velocity.Dot(mtvDirection);
                // 원래 속도에서 법선 방향 속도 제거
                pOtherRigidBody->SetVelocity(velocity - normalVelocity);
            }

            
            if (pOtherObj->GetGroup() == GROUP_TYPE::PLAYER)
            {
                SPlayer* pPlayer = static_cast<SPlayer*>(pOtherObj);

                // 벽 상호작용 로직 (벽타기 등)
                float wallClimbTopY = GetWorldPos().y + WALL_CLIMB_TOP_OFFSET;
                float wallClimbBottomY = GetWorldPos().y + GetScale().y + WALL_CLIMB_BOT_OFFSET;
                bool canClimb = (vObjPos.y > wallClimbTopY && vObjPos.y < wallClimbBottomY);

                // UNWALKABLE 타입의 벽(트리거 벽 포함)은 벽타기 불가
                if (m_eGroundType == GROUND_TYPE::UNWALKABLE)
                {
                    canClimb = false;
                }

                if (horizontalDot > 0.5f)
                { // MTV가 오른쪽을 향함 -> 왼쪽 벽 충돌
                    if (canClimb) pPlayer->SetWallClimbing(true);
                    else pPlayer->SetWallClimbing(false);

                    if (pPlayer->GetState() != PLAYER_STATE::SWING)
                        pPlayer->SetIsFacingRight(false);
                }
                else if (horizontalDot < -0.5f)
                { // MTV가 왼쪽을 향함 -> 오른쪽 벽 충돌
                    if (canClimb) pPlayer->SetWallClimbing(true);
                    else pPlayer->SetWallClimbing(false);
                    if (pPlayer->GetState() != PLAYER_STATE::SWING)
                        pPlayer->SetIsFacingRight(true);
                }
                else
                {
                    pPlayer->SetWallClimbing(false);
                }

                // 땅 위가 아님 상태 설정
                pPlayer->SetOnGround(false);
            }
        }
    }
}



void CGround::OnCollisionExit(CCollider *_pOther)
{
    GameObject *pOtherObj = _pOther->GetObj();
    if (pOtherObj->GetGroup() == GROUP_TYPE::PLAYER)
    {
        pOtherObj->GetGravity()->SetApplyGravity(true);
        static_cast<SPlayer *>(pOtherObj)->SetOnGround(false);
        static_cast<SPlayer *>(pOtherObj)->SetWallClimbing(false);
        
      
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