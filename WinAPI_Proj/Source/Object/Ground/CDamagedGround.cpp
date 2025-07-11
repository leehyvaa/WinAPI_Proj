#include "pch.h"
#include "CDamagedGround.h"
#include "CCollider.h"
#include "CCollisionMgr.h"
#include "SPlayer.h"
#include "CRigidBody.h"
#include "CTimeMgr.h"
#include "CAnimator.h"
#include "CResMgr.h"
#include "CTexture.h"

CDamagedGround::CDamagedGround()
    : m_pPlayerInside(nullptr)
    , m_fDamageTimer(0.f)
    , m_fDamageInterval(1.0f)  // 1초 간격
    , m_iDamageAmount(1)       // 1 데미지
    , m_fPushForce(15000.f)    // 밀어내는 힘
{
    // CGround에서 이미 CreateCollider() 호출됨
    SetGroundType(GROUND_TYPE::DAMAGEZONE);
    CreateAnimator();

   
}

CDamagedGround::~CDamagedGround()
{
}

void CDamagedGround::Start()
{
CGround::Start();

    Vec2 vOffset = CalculateAnimationOffset();
    // 396x396 크기의 38프레임 애니메이션 텍스처를 로드합니다.
    CTexture* pTex = CResMgr::GetInst()->LoadTexture(L"DamagedGroundTex", L"texture/damagezone/damaged_ground.png");
    GetAnimator()->CreateAnimation(L"DAMAGED_GROUND_ANIM", pTex,
                                   Vec2(0.f, 0.f), Vec2(396.f, 396.f), Vec2(396.f, 0.f), 0.04f, 38, 1.f,
                                   vOffset);
    if (GetAnimator())
    {
	    GetAnimator()->Play(L"DAMAGED_GROUND_ANIM", true);
    }
}

void CDamagedGround::Update()
{
    CGround::Update();

    // 플레이어가 영역 안에 있고 타이머가 활성화된 경우
    if (m_pPlayerInside != nullptr)
    {
        m_fDamageTimer += fDT;

        // 1초가 지나면 데미지 및 밀어내기
        if (m_fDamageTimer >= m_fDamageInterval)
        {
            DamageAndPushPlayer();
            m_fDamageTimer = 0.f; // 타이머 리셋
        }
    }
}

void CDamagedGround::OnCollisionEnter(CCollider* _pOther)
{
    GameObject* pOtherObj = _pOther->GetObj();

    if (pOtherObj->GetGroup() == GROUP_TYPE::PLAYER)
    {
        SPlayer* pPlayer = static_cast<SPlayer*>(pOtherObj);
        m_pPlayerInside = pPlayer;
        m_fDamageTimer = 0.f; // 타이머 시작
    }
}

void CDamagedGround::OnCollision(CCollider* _pOther)
{
    // CDamagedGround는 플레이어를 밀어내지 않고 영역 안에 머물게 함
    // 충돌 처리는 하지 않음
}

void CDamagedGround::OnCollisionExit(CCollider* _pOther)
{
    GameObject* pOtherObj = _pOther->GetObj();

    if (pOtherObj->GetGroup() == GROUP_TYPE::PLAYER)
    {
        SPlayer* pPlayer = static_cast<SPlayer*>(pOtherObj);
        if (m_pPlayerInside == pPlayer)
        {
            m_pPlayerInside = nullptr;
            m_fDamageTimer = 0.f; // 타이머 리셋
        }
    }
}

void CDamagedGround::DamageAndPushPlayer()
{
    if (m_pPlayerInside == nullptr) return;

    // 플레이어에게 데미지
    m_pPlayerInside->TakeDamage(m_iDamageAmount);

    // 플레이어를 밀어내기
    Vec2 pushDirection = CalculatePushDirection(m_pPlayerInside);
    if (m_pPlayerInside->GetRigidBody())
    {
        m_pPlayerInside->GetRigidBody()->AddForce(pushDirection * m_fPushForce);
    }
}

Vec2 CDamagedGround::CalculatePushDirection(SPlayer* _pPlayer)
{
    if (_pPlayer == nullptr) return Vec2(0.f, 0.f);

    Vec2 playerPos = _pPlayer->GetWorldPos();
    Vec2 groundCenter = GetWorldPos() + Vec2(GetScale().x / 2.f, GetScale().y / 2.f);

    // 플레이어에서 그라운드 중심으로의 방향 벡터
    Vec2 direction = playerPos - groundCenter;

    // 방향이 0이면 위쪽으로 밀어내기
    if (direction.Length() < 0.1f)
    {
        return Vec2(0.f, -1.f);
    }

    direction.Normalize();
    return direction;
}

Vec2 CDamagedGround::CalculateAnimationOffset()
{
    // 벽의 현재 스케일 가져오기
    Vec2 vWallScale = GetScale();
    Vec2 vOffset;
    vOffset.x = vWallScale.x/2  ;
    vOffset.y = vWallScale.y/2 ;
    return vOffset;
}
