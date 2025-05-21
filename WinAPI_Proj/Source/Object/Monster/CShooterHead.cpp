#include "pch.h"
#include "CShooterHead.h"

#include "CAnimation.h"
#include "CAnimator.h"
#include "CMonster.h"
#include "CResMgr.h"
#include "SPlayer.h" 
#include "CSceneMgr.h" 
#include "CScene.h" 
#include <cmath> 

// PI 정의
#ifndef PI
#define PI 3.14159265358979323846f
#endif

CShooterHead::CShooterHead()
{
    SetGroup(GROUP_TYPE::MONSTER_HEAD); 
    CreateAnimator();
    CTexture *pTex = CResMgr::GetInst()->LoadTexture(L"RifleManTex", L"texture\\enemy\\rifleman\\RifleMan.bmp");

    GetAnimator()->CreateAnimation(L"RIFLEMAN_SHOT_HEAD_TOP", pTex,
                                           Vec2(0.f, 1400.f), Vec2(200.f, 200.f), Vec2(200.f, 0.f), 0.25f, 4, 2.f, Vec2(0.f, -20.f));
    GetAnimator()->CreateAnimation(L"RIFLEMAN_SHOT_HEAD", pTex,
                                           Vec2(0.f, 1600.f), Vec2(200.f, 200.f), Vec2(200.f, 0.f), 0.25f, 4, 2.f, Vec2(0.f, -20.f));
    GetAnimator()->CreateAnimation(L"RIFLEMAN_AIMING_HEAD_TOP", pTex,
                                           Vec2(0.f, 2000.f), Vec2(200.f, 200.f), Vec2(200.f, 0.f), 0.25f, 3, 2.f, Vec2(0.f, -20.f));
    GetAnimator()->CreateAnimation(L"RIFLEMAN_AIMING_HEAD_TOP_STOP", pTex,
                                            Vec2(400.f, 2000.f), Vec2(200.f, 200.f), Vec2(200.f, 0.f), 0.25f, 1, 2.f, Vec2(0.f, -20.f));


    GetAnimator()->FindAnimation(L"RIFLEMAN_SHOT_HEAD_TOP")->Save(L"animation\\rifleman_shot_head_top.anim");
    GetAnimator()->FindAnimation(L"RIFLEMAN_SHOT_HEAD")->Save(L"animation\\rifleman_shot_head.anim");
    GetAnimator()->FindAnimation(L"RIFLEMAN_AIMING_HEAD_TOP")->Save(L"animation\\rifleman_aiming_head_top.anim");
    GetAnimator()->FindAnimation(L"RIFLEMAN_AIMING_HEAD_TOP_STOP")->Save(L"animation\\rifleman_aiming_head_top_stop.anim");
    
}

CShooterHead::~CShooterHead()
{
}

void CShooterHead::Update()
{
    // 플레이어 오브젝트를 찾습니다.
    SPlayer* pPlayer = dynamic_cast<SPlayer*>(CSceneMgr::GetInst()->GetCurScene()->GetPlayer());
    if (pPlayer)
    {
        Vec2 headPos = GetWorldPos(); // Head의 월드 위치를 가져옵니다.
        Vec2 playerPos = pPlayer->GetWorldPos(); // 플레이어의 월드 위치를 가져옵니다.

        // 플레이어의 수평 위치에 따라 Head의 좌우 방향을 결정합니다.
        // 이 m_bIsFacingRight 값은 CAnimation::Render에서 좌우 반전(Scale(-1,1))에 사용됩니다.
        if (playerPos.x > headPos.x)
        {
            m_bIsFacingRight = true; // 플레이어가 오른쪽에 있으면 오른쪽을 바라봅니다.
        }
        else
        {
            m_bIsFacingRight = false; // 플레이어가 왼쪽에 있으면 왼쪽을 바라봅니다.
        }

        // Head에서 플레이어를 향하는 방향 벡터를 계산합니다.
        Vec2 dirToPlayer = playerPos - headPos;
        dirToPlayer.Normalize(); // 단위 벡터로 정규화

        // 플레이어 방향 벡터와 양의 X축 사이의 각도를 라디안으로 계산합니다.
        // atan2f는 -PI ~ PI 범위의 각도를 반환합니다.
        float angleRad = atan2f(dirToPlayer.y, dirToPlayer.x);
        
        // 라디안을 도로 변환합니다.
        float angleDeg = angleRad * (180.f / PI);

        // SetLocalRotation(0)이 오른쪽(양의 X축)을 가리킨다고 가정합니다.
        // Shooter Head 스프라이트 자체는 0도 회전 시 위쪽(12시 방향, 음의 Y축)을 향하도록 그려져 있습니다.
        // 따라서 스프라이트의 "위쪽"이 목표 각도(angleDeg)를 향하도록 하려면,
        // angleDeg에 90도를 더해야 합니다.

        float finalRotationDeg;

        if (m_bIsFacingRight)
        {
            // 플레이어가 오른쪽에 있을 경우 (스프라이트가 좌우 반전되지 않을 때)
            // 목표 각도에 90도를 더하여 스프라이트의 "위쪽"이 플레이어를 향하도록 합니다.
            finalRotationDeg = angleDeg + 90.f;
        }
        else
        {
            // 플레이어가 왼쪽에 있을 경우 (스프라이트가 좌우 반전될 때)
            // CAnimation::Render에서 Scale(-1,1) 변환이 먼저 적용됩니다.
            // 이로 인해 X축이 뒤집힌 좌표계에서 회전이 이루어집니다.
            // 뒤집힌 좌표계에서 목표 각도(angleDeg)에 해당하는 각도는 (180 - angleDeg)입니다.
            // 이 각도에 스프라이트의 기본 방향 오프셋(90도)을 더해줍니다.
            finalRotationDeg = (180.f - angleDeg) + 90.f;
            // 결과적으로 (270.f - angleDeg)가 됩니다.
        }
        
        SetLocalRotation(finalRotationDeg);
    }
}

void CShooterHead::Render(HDC _dc)
{
    GameObject::Render(_dc);
}
