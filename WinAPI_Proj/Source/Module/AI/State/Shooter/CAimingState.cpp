#include "pch.h"
#include "CAimingState.h"

#include "AI.h"
#include "CAnimation.h"
#include "CAnimator.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "SPlayer.h"
#include "CMonster.h"
#include "CTimeMgr.h"
#include "Monster/CShooterHead.h"
#include "Monster/CShooterMonster.h"
#include "CBullet.h"
#include "CObjectPool.h"
#include "CCore.h"

// Direct2D 헤더
#include <d2d1.h>
#pragma comment(lib, "d2d1.lib")

CAimingState::CAimingState() : CState(MON_STATE::AIMING)
                               , m_fShotDelay(1.0f) // 1초마다 발사
                               , m_fShotTimer(0.f)
{
    
}

CAimingState::~CAimingState()
{
}

void CAimingState::Enter()
{
    // 몸체 애니메이션 설정
    GetMonster()->GetAnimator()->Play(L"RIFLEMAN_AIMING_BODY", false);
    GetMonster()->GetAnimator()->FindAnimation(L"RIFLEMAN_AIMING_BODY")->SetEndFrameEvent([this]() {
               GetMonster()->GetAnimator()->Play(L"RIFLEMAN_AIMING_BODY_STOP", true);
    });
    

    // 머리 애니메이션 설정
    if (!dynamic_cast<CShooterMonster*>(GetMonster())->GetHead())
    {
        CShooterHead* pHead = static_cast<CShooterHead*>(CObjectPool::GetInst()->GetPoolObject(L"ShooterHeadPool"));
        dynamic_cast<CShooterMonster*>(GetMonster())->SetHead(pHead);

        pHead->GetAnimator()->Play(L"RIFLEMAN_AIMING_HEAD_TOP", false);
        pHead->GetAnimator()->FindAnimation(L"RIFLEMAN_AIMING_HEAD_TOP")->SetEndFrameEvent([this]() {
               dynamic_cast<CShooterMonster*>(GetMonster())->GetHead()->GetAnimator()->Play(L"RIFLEMAN_AIMING_HEAD_TOP_STOP", true);
        });
        pHead->SetParent(GetMonster());
        pHead->SetWorldPos(GetMonster()->GetWorldPos()+Vec2(0.f, -80.f));
        pHead->SetName(L"MonsterHead");
        CreateObject(pHead, GROUP_TYPE::MONSTER_HEAD); // 씬에 추가
        
    }
    m_fShotTimer = 0.f; // 상태 진입 시 타이머 초기화
}

void CAimingState::Update()
{
    SPlayer* pPlayer = dynamic_cast<SPlayer*>(CSceneMgr::GetInst()->GetCurScene()->GetPlayer());
    CShooterMonster* pMonster = dynamic_cast<CShooterMonster*>(GetMonster());

    if (pPlayer && pMonster && pMonster->GetHead())
    {
        // 몬스터 헤드가 플레이어를 바라보도록 업데이트
        pMonster->GetHead()->LookAt(pPlayer->GetWorldPos());

        // 총알 발사 타이머 업데이트
        m_fShotTimer += fDT;
        if (m_fShotTimer >= m_fShotDelay)
        {
            FireBullet();
            m_fShotTimer = 0.f; // 타이머 리셋
        }
    }
    else
    {
        // 플레이어가 없거나 몬스터 헤드가 없으면 IDLE 상태로 전환
        //ChangeAIState(GetAI(), MON_STATE::IDLE);
    }
}

void CAimingState::FireBullet()
{
    CBullet* pBullet = dynamic_cast<CBullet*>(CObjectPool::GetInst()->GetPoolObject(L"ShooterBullet"));
    if (pBullet)
    {
        CShooterMonster* pMonster = dynamic_cast<CShooterMonster*>(GetMonster());
        if (pMonster && pMonster->GetHead())
        {
            // 총알의 초기 위치를 몬스터 헤드의 위치로 설정
            if (GetAI()->GetOwner()->GetIsFacingRight())
                pBullet->SetWorldPos(pMonster->GetHead()->GetWorldPos()+Vec2(50.f,5.0f));
            else
                pBullet->SetWorldPos(pMonster->GetHead()->GetWorldPos()+Vec2(-50.f,5.0f));

                
            // 총알의 속성을 소유자 몬스터에 따라 설정 (속도, 데미지, 텍스처 등)
            pBullet->SetBulletInfo(pMonster);
            
            // 씬에 총알 추가
            CreateObject(pBullet, GROUP_TYPE::PROJ_MONSTER);
        }
        else
        {
            // 총알을 가져왔지만 발사할 수 없는 경우 다시 풀로 반환
            CObjectPool::GetInst()->ReturnObject(pBullet);
        }
    }
}


void CAimingState::Exit()
{
}

void CAimingState::Render(HDC _dc)
{
    // Direct2D 활성화 시 GDI 렌더링 스킵
    if (CCore::GetInst()->GetD2DRenderTarget())
        return;
        
    SPlayer* pPlayer = dynamic_cast<SPlayer*>(CSceneMgr::GetInst()->GetCurScene()->GetPlayer());
    CShooterMonster* pMonster = dynamic_cast<CShooterMonster*>(GetMonster());
    
    if (!pPlayer || !pMonster || !pMonster->GetHead())
        return;
    
    // GDI 모드에서는 기본 펜과 브러시 사용
    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0)); // 빨간색
    HBRUSH hBrush = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
    
    HPEN hOldPen = (HPEN)SelectObject(_dc, hPen);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(_dc, hBrush);

    Vec2 renderPos = pMonster->GetHead()->GetWorldPos();
    if (GetAI()->GetOwner()->GetIsFacingRight())
        renderPos = renderPos + Vec2(40.f,5.0f);
    else
        renderPos = renderPos + Vec2(-40.f,5.0f);
    
    Vec2 dir = pPlayer->GetWorldPos()+Vec2(0.f,-55.f) - renderPos;
    renderPos = CCamera::GetInst()->GetRenderPos(renderPos);
    dir.Normalize();
    MoveToEx(_dc, renderPos.x, renderPos.y,nullptr);
    
    LineTo(_dc, renderPos.x + dir.x * 1500 ,renderPos.y + dir.y*1500);
}

void CAimingState::RenderD2D(ID2D1RenderTarget* _pRenderTarget)
{
    if (!_pRenderTarget)
        return;
        
    SPlayer* pPlayer = dynamic_cast<SPlayer*>(CSceneMgr::GetInst()->GetCurScene()->GetPlayer());
    CShooterMonster* pMonster = dynamic_cast<CShooterMonster*>(GetMonster());
    
    if (!pPlayer || !pMonster || !pMonster->GetHead())
        return;

    // 빨간색 브러시 생성 (정적으로 캐싱)
    static ID2D1SolidColorBrush* s_pRedBrush = nullptr;
    if (!s_pRedBrush)
    {
        _pRenderTarget->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::Red, 0.8f),
            &s_pRedBrush
        );
    }

    if (!s_pRedBrush)
        return;

    // 조준선 시작점 계산
    Vec2 renderPos = pMonster->GetHead()->GetWorldPos();
    if (GetAI()->GetOwner()->GetIsFacingRight())
        renderPos = renderPos + Vec2(40.f, 5.0f);
    else
        renderPos = renderPos + Vec2(-40.f, 5.0f);
    
    // 조준 방향 계산
    Vec2 dir = pPlayer->GetWorldPos() + Vec2(0.f, -55.f) - renderPos;
    renderPos = CCamera::GetInst()->GetRenderPos(renderPos);
    dir.Normalize();
    
    // 조준선 끝점 계산
    Vec2 endPos = renderPos + dir * 1500.0f;
    
    // Direct2D로 조준선 그리기
    _pRenderTarget->DrawLine(
        D2D1::Point2F(renderPos.x, renderPos.y),
        D2D1::Point2F(endPos.x, endPos.y),
        s_pRedBrush,
        2.0f
    );
}
