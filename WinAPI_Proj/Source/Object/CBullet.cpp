#include "pch.h"
#include "CBullet.h"

#include "CAnimation.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CResMgr.h"
#include "CTexture.h"
#include "CEventMgr.h"
#include "CScene.h"
#include "CSceneMgr.h"
#include "SPlayer.h" 
#include "Monster/CShooterMonster.h" 
#include "CTimeMgr.h" 
#include "Monster/CShooterHead.h"

CBullet::CBullet()
    : m_fSpeed(0.f)
    , m_fRange(0.f)
    , m_iDamage(0)
    , m_pOwnerMonster(nullptr)
    , m_fDistanceTraveled(0.f)
{
    SetGroup(GROUP_TYPE::PROJ_MONSTER);
    CreateCollider();
    CreateAnimator();

    CTexture *pTex = CResMgr::GetInst()->LoadTexture(L"RifleManTex", L"texture\\enemy\\rifleman\\RifleMan.png");

    GetAnimator()->CreateAnimation(L"RIFLEMAN_BULLET", pTex,
                                               Vec2(0.f, 1000.f), Vec2(200.f, 200.f), Vec2(200.f, 0.f), 0.25f, 12, 2.f, Vec2(0.f, 20.f));

    GetAnimator()->FindAnimation(L"RIFLEMAN_BULLET")->Save(L"animation\\rifleman_bullet.anim");
    
}

CBullet::~CBullet()
{
}

void CBullet::SetBulletInfo(GameObject* _pOwner)
{
    Reset(); // 풀에서 가져올 때마다 초기화

    m_pOwnerMonster = dynamic_cast<CMonster*>(_pOwner); // 오너 저장

    // 기본 총알 속성
    m_fSpeed = 400.f;
    m_fRange = 2000.f;
    m_iDamage = 10;

    // 소유자 타입에 따라 총알 속성 설정
    if (CShooterMonster* pShooter = dynamic_cast<CShooterMonster*>(_pOwner))
    {
        m_fSpeed = 400.f;
        m_fRange = 2000.f;
        m_iDamage = 20;
        
        GetCollider()->SetScale(Vec2(20.f, 20.f));
        GetCollider()->SetOffsetPos(Vec2(0.f, 0.f));

        // 총알 방향 설정
        SPlayer* pPlayer = dynamic_cast<SPlayer*>(CSceneMgr::GetInst()->GetCurScene()->GetPlayer());
        if (pPlayer && pShooter->GetHead())
        {
            Vec2 dir = pPlayer->GetWorldPos() - pShooter->GetHead()->GetWorldPos();
            dir.Normalize();
            SetDir(dir);
            LookAt(pPlayer->GetWorldPos() + Vec2(0.0,-60.f)); // 총알이 플레이어를 바라보도록 회전
        }
        else
        {
            // 플레이어가 없거나 헤드가 없으면 기본 방향
            SetDir(Vec2(0.f, 1.f));
            LookAt(GetWorldPos() + Vec2(0.f, 1.f));
        }
    }
    // 다른 몬스터 타입의 총알이 있다면 여기에 추가
    // else if (CAnotherMonster* pAnother = dynamic_cast<CAnotherMonster*>(_pOwner))
    // {
    //     m_fSpeed = 600.f;
    //     m_fRange = 700.f;
    //     m_iDamage = 15;
    //     GetCollider()->SetScale(Vec2(15.f, 15.f));
    // }
    GetAnimator()->Play(L"RIFLEMAN_BULLET",true);

    m_fDistanceTraveled = 0.f; // 이동 거리 초기화
    SetActive(true);
}

void CBullet::ReturnToPool()
{
    DeleteObject(this);
}

void CBullet::Update()
{
    if (!IsActive())
        return;

    Vec2 vPos = GetWorldPos();
    Vec2 vDir = GetDir();

    // 총알 이동
    float moveAmount = m_fSpeed * fDT;
    vPos.x += vDir.x * moveAmount;
    vPos.y += vDir.y * moveAmount;
    SetWorldPos(vPos);

    m_fDistanceTraveled += moveAmount;

    // 사정거리 초과 또는 화면 밖으로 나갔는지 체크
    if (m_fDistanceTraveled >= m_fRange)
    {
        ReturnToPool();
    }
}

void CBullet::Render(ID2D1RenderTarget* _pRenderTarget)
{
    if (!IsActive() || !_pRenderTarget)
        return;
    
    GameObject::Render(_pRenderTarget);
}

void CBullet::OnCollisionEnter(CCollider* _pOther)
{
    GameObject* pOtherObj = _pOther->GetObj();

    // 플레이어와 충돌 시
    if (pOtherObj->GetGroup() == GROUP_TYPE::PLAYER)
    {
        // 여기서 데미지 적용
        
        ReturnToPool();
    }
    // 지형과 충돌 시
    else if (pOtherObj->GetGroup() == GROUP_TYPE::GROUND)
    {
        ReturnToPool();
    }
}

void CBullet::Reset()
{
    GameObject::Reset();
    m_fSpeed = 0.f;
    m_fRange = 0.f;
    m_iDamage = 0;
    m_pOwnerMonster = nullptr;
    m_fDistanceTraveled = 0.f;
    
    if (GetCollider())
    {
        GetCollider()->SetActive(true);
        GetCollider()->SetScale(Vec2(20.f, 20.f));
        GetCollider()->SetOffsetPos(Vec2(0.f, 0.f));
    }
}
