#include "pch.h"
#include "CShooterHead.h"

#include "CAnimation.h"
#include "CAnimator.h"
#include "CMonster.h"
#include "CResMgr.h"
#include "SPlayer.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CObjectPool.h"

CShooterHead::CShooterHead()
{
    SetGroup(GROUP_TYPE::MONSTER_HEAD);
    CreateAnimator();
    CTexture *pTex = CResMgr::GetInst()->LoadTexture(L"RifleManTex", L"texture\\enemy\\rifleman\\RifleMan.png");

    GetAnimator()->CreateAnimation(L"RIFLEMAN_SHOT_HEAD_TOP", pTex,
                                           Vec2(0.f, 1400.f), Vec2(200.f, 200.f), Vec2(200.f, 0.f), 0.25f, 4, 2.f, Vec2(0.f, -10.f));
    GetAnimator()->CreateAnimation(L"RIFLEMAN_SHOT_HEAD", pTex,
                                           Vec2(0.f, 1600.f), Vec2(200.f, 200.f), Vec2(200.f, 0.f), 0.25f, 4, 2.f, Vec2(0.f, -10.f));
    GetAnimator()->CreateAnimation(L"RIFLEMAN_AIMING_HEAD_TOP", pTex,
                                           Vec2(0.f, 2000.f), Vec2(200.f, 200.f), Vec2(200.f, 0.f), 0.25f, 3, 2.f, Vec2(0.f, -10.f));
    GetAnimator()->CreateAnimation(L"RIFLEMAN_AIMING_HEAD_TOP_STOP", pTex,
                                            Vec2(400.f, 2000.f), Vec2(200.f, 200.f), Vec2(200.f, 0.f), 0.25f, 1, 2.f, Vec2(0.f, -10.f));


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
    // 플레이어 오브젝트
    SPlayer* pPlayer = dynamic_cast<SPlayer*>(CSceneMgr::GetInst()->GetCurScene()->GetPlayer());
    if (pPlayer)
    {
        Vec2 headPos = GetWorldPos(); 
        Vec2 playerPos = pPlayer->GetWorldPos();

        // 플레이어의 위치에 따라 Head의 좌우 결정
        if (playerPos.x > headPos.x)
            m_bIsFacingRight = true;
        else
            m_bIsFacingRight = false; 

        // Head에서 플레이어를 향하는 방향 벡터를 계산
        Vec2 dirToPlayer = playerPos+Vec2(0.f,-60.f) - headPos;
        dirToPlayer.Normalize();

        // 플레이어 방향 벡터와 양의 X축 사이의 각도를 라디안으로 계산 후 디그리로 변환
        float angleRad = atan2f(dirToPlayer.y, dirToPlayer.x);
        float angleDeg = angleRad * (180.f / 3.14159);


        float finalRotationDeg;

        if (m_bIsFacingRight)
        {
            // 플레이어가 오른쪽에 있을 경우 (스프라이트가 좌우 반전되지 않음)
            finalRotationDeg = angleDeg + 90.f;
        }
        else
        {
            // 플레이어가 왼쪽에 있을 경우 (스프라이트가 좌우 반전된 상태)
            finalRotationDeg = (180.f + angleDeg) - 90.f;
        }
        
        SetLocalRotation(finalRotationDeg);
    }
}

void CShooterHead::Render(ID2D1RenderTarget* _pRenderTarget)
{
    if (!_pRenderTarget)
        return;
        
    GameObject::Render(_pRenderTarget);
}

void CShooterHead::Reset()
{
    GameObject::Reset(); 
    SetLocalRotation(0.f);
    m_bIsFacingRight = false;
}
