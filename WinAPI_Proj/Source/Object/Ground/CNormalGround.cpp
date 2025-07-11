﻿#include "pch.h"
#include "CNormalGround.h"
#include "CCollider.h"
#include "CCollisionMgr.h"
#include "CGravity.h"
#include "CCore.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "SPlayer.h"
#include "CRigidBody.h"
#include "CCamera.h"

const float COLLISION_TOP_THRESHOLD = 20.f;
const float COLLISION_BOT_THRESHOLD = 15.f;
const float COLLISION_SIDE_THRESHOLD = 20.f;
const float WALL_CLIMB_TOP_OFFSET = 70.f;
const float WALL_CLIMB_BOT_OFFSET = 85.f;


CNormalGround::CNormalGround()
{
    // CGround에서 이미 CreateCollider() 호출됨
}

CNormalGround::~CNormalGround()
{
}

void CNormalGround::OnCollisionEnter(CCollider* _pOther)
{
    GameObject* pOtherObj = _pOther->GetObj();
    Vec2 vObjPos = pOtherObj->GetWorldPos();
    Vec2 vObjColScale = _pOther->GetScale();

    if (pOtherObj->GetGroup() == GROUP_TYPE::PLAYER)
    {
        SPlayer* pPlayer = static_cast<SPlayer*>(pOtherObj);

        if (GetGroundType() == GROUND_TYPE::NORMAL)
        {
            switch (GetCollideType())
            {
            case TILE_COLLIDE_TYPE::SOLID:
                // 전체 충돌 처리
                break;
            }
        }
    }
}

void CNormalGround::OnCollision(CCollider* _pOther)
{
    GameObject* pOtherObj = _pOther->GetObj();
    if (pOtherObj->GetGroup() != GROUP_TYPE::PLAYER &&
        pOtherObj->GetGroup() != GROUP_TYPE::MONSTER) return;

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
                    pPlayer->SetOnGround(false);
                    pPlayer->SetWallClimbing(false);
                }
            }
        }
        else // 수평 충돌 (MTV가 좌/우 방향에 더 가까울 때)
        {
            // 플레이어를 좌우로 밀어낸다.
            vObjPos += mtvDirection * mtvDepth;
            pOtherObj->SetWorldPos(vObjPos);

            if (pOtherObj->GetGroup() == GROUP_TYPE::PLAYER)
            {
                SPlayer* pPlayer = static_cast<SPlayer*>(pOtherObj);

                // 벽 상호작용 로직 (벽타기 등)
                float wallClimbTopY = GetWorldPos().y + WALL_CLIMB_TOP_OFFSET;
                float wallClimbBottomY = GetWorldPos().y + GetScale().y + WALL_CLIMB_BOT_OFFSET;
                bool canClimb = (vObjPos.y > wallClimbTopY && vObjPos.y < wallClimbBottomY);

                // UNWALKABLE 타입의 벽(트리거 벽 포함)은 벽타기 불가
                if (GetGroundType() == GROUND_TYPE::UNWALKABLE)
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

void CNormalGround::OnCollisionExit(CCollider* _pOther)
{
    GameObject* pOtherObj = _pOther->GetObj();
    if (pOtherObj->GetGroup() == GROUP_TYPE::PLAYER)
    {
        pOtherObj->GetGravity()->SetApplyGravity(true);
        static_cast<SPlayer*>(pOtherObj)->SetOnGround(false);
        static_cast<SPlayer*>(pOtherObj)->SetWallClimbing(false);
    }
}

