#include "CShooterHead.h"

#include "CAnimation.h"
#include "CAnimator.h"
#include "CMonster.h"
#include "CResMgr.h"

CShooterHead::CShooterHead()
{
    SetGroup(GROUP_TYPE::MONSTER_HEAD); 
    CreateAnimator();
    CTexture *pTex = CResMgr::GetInst()->LoadTexture(L"RifleManTex", L"texture\\enemy\\rifleman\\RifleMan.bmp");

    GetAnimator()->CreateAnimation(L"RIFLEMAN_SHOT_HEAD_TOP", pTex,
                                           Vec2(0.f, 1400.f), Vec2(200.f, 200.f), Vec2(200.f, 0.f), 0.25f, 4, 2.f, Vec2(0.f, -64.f));
    GetAnimator()->CreateAnimation(L"RIFLEMAN_SHOT_HEAD", pTex,
                                           Vec2(0.f, 1600.f), Vec2(200.f, 200.f), Vec2(200.f, 0.f), 0.25f, 4, 2.f, Vec2(0.f, -64.f));
    GetAnimator()->CreateAnimation(L"RIFLEMAN_AIMING_HEAD_TOP", pTex,
                                           Vec2(0.f, 2000.f), Vec2(200.f, 200.f), Vec2(200.f, 0.f), 0.25f, 3, 2.f, Vec2(0.f, -64.f));


    GetAnimator()->FindAnimation(L"RIFLEMAN_SHOT_HEAD_TOP")->Save(L"animation\\rifleman_shot_head_top.anim");
    GetAnimator()->FindAnimation(L"RIFLEMAN_SHOT_HEAD")->Save(L"animation\\rifleman_shot_head.anim");
    GetAnimator()->FindAnimation(L"RIFLEMAN_AIMING_HEAD_TOP")->Save(L"animation\\rifleman_aiming_head_top.anim");
    
}

CShooterHead::~CShooterHead()
{
}

void CShooterHead::Update()
{
    CMonster* monster = static_cast<CMonster*>(GetParent());
    if (monster)
    {
        m_bIsFacingRight = monster->GetIsFacingRight();
    }
}

void CShooterHead::Render(HDC _dc)
{
    GameObject::Render(_dc);
}
