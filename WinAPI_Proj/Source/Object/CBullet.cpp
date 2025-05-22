#include "pch.h"
#include "CBullet.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CResMgr.h"
#include "CTexture.h"
#include "CEventMgr.h"
#include "SPlayer.h" // 플레이어와 충돌 처리 위함
#include "CShooterMonster.h" // CShooterMonster로부터 속성 설정 위함
#include "CTimeMgr.h" // fDT 사용 위함

CBullet::CBullet()
    : m_fSpeed(0.f)
    , m_fRange(0.f)
    , m_iDamage(0)
    , m_pBulletTex(nullptr)
    , m_pOwnerMonster(nullptr)
    , m_fDistanceTraveled(0.f)
{
    SetGroup(GROUP_TYPE::PROJECTILE);
    CreateCollider();
    CreateAnimator(); // 애니메이터는 나중에 필요시 사용
}

CBullet::~CBullet()
{
}

void CBullet::SetBulletInfo(GameObject* _pOwner)
{
    Reset(); // 풀에서 가져올 때마다 초기화

    m_pOwnerMonster = dynamic_cast<CMonster*>(_pOwner); // 소유자 몬스터 저장

    // 기본 총알 속성 (다른 몬스터 타입에서 기본값으로 사용 가능)
    m_fSpeed = 500.f;
    m_fRange = 800.f;
    m_iDamage = 10;
    m_pBulletTex = nullptr; // 기본 텍스처 없음

    // 소유자 몬스터 타입에 따라 총알 속성 설정
    if (CShooterMonster* pShooter = dynamic_cast<CShooterMonster*>(_pOwner))
    {
        m_pBulletTex = CResMgr::GetInst()->LoadTexture(L"ShooterBulletTex", L"texture\\enemy\\rifleman\\bullet.bmp");
        m_fSpeed = 800.f;
        m_fRange = 1000.f;
        m_iDamage = 20;
        
        // 충돌체 스케일 조정
        GetCollider()->SetScale(Vec2(20.f, 20.f));
        GetCollider()->SetOffsetPos(Vec2(0.f, 0.f));

        // 총알 방향 설정 (몬스터 헤드 -> 플레이어)
        SPlayer* pPlayer = dynamic_cast<SPlayer*>(CSceneMgr::GetInst()->GetCurScene()->GetPlayer());
        if (pPlayer && pShooter->GetHead())
        {
            Vec2 dir = pPlayer->GetWorldPos() - pShooter->GetHead()->GetWorldPos();
            dir.Normalize();
            SetDir(dir);
            LookAt(pPlayer->GetWorldPos()); // 총알이 플레이어를 바라보도록 회전
        }
        else
        {
            // 플레이어가 없거나 헤드가 없으면 기본 방향 (예: 아래)
            SetDir(Vec2(0.f, 1.f));
            LookAt(GetWorldPos() + Vec2(0.f, 1.f));
        }
    }
    // 다른 몬스터 타입의 총알이 있다면 여기에 else if 추가
    // else if (CAnotherMonster* pAnother = dynamic_cast<CAnotherMonster*>(_pOwner))
    // {
    //     m_pBulletTex = CResMgr::GetInst()->LoadTexture(L"AnotherBulletTex", L"path\\to\\another_bullet.bmp");
    //     m_fSpeed = 600.f;
    //     m_fRange = 700.f;
    //     m_iDamage = 15;
    //     GetCollider()->SetScale(Vec2(15.f, 15.f));
    // }

    m_fDistanceTraveled = 0.f; // 이동 거리 초기화
    SetActive(true); // 활성화
}

void CBullet::ReturnToPool()
{
    // 풀로 반환 요청 이벤트 추가
    tEvent evn = {};
    evn.eEvent = EVENT_TYPE::RETURN_TO_POOL;
    evn.lParam = (DWORD_PTR)this;
    CEventMgr::GetInst()->AddEvent(evn);
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
    if (m_fDistanceTraveled >= m_fRange ||
        vPos.x < 0 || vPos.x > CCore::GetInst()->GetResolution().x ||
        vPos.y < 0 || vPos.y > CCore::GetInst()->GetResolution().y)
    {
        ReturnToPool();
    }
}

void CBullet::Render(HDC _dc)
{
    if (!IsActive() || !m_pBulletTex)
        return;

    Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(GetWorldPos());
    float width = static_cast<float>(m_pBulletTex->Width());
    float height = static_cast<float>(m_pBulletTex->Height());

    BLENDFUNCTION bf = {};
    bf.BlendOp = AC_SRC_OVER;
    bf.BlendFlags = 0;
    bf.AlphaFormat = AC_SRC_ALPHA;
    bf.SourceConstantAlpha = 255; // 전역 알파 (255 = 불투명)

    AlphaBlend(_dc,
        static_cast<int>(vRenderPos.x - width / 2.f),
        static_cast<int>(vRenderPos.y - height / 2.f),
        static_cast<int>(width),
        static_cast<int>(height),
        m_pBulletTex->GetDC(),
        0, 0,
        static_cast<int>(width),
        static_cast<int>(height),
        bf);

    Component_Render(_dc); // 콜라이더 렌더링
}

void CBullet::OnCollisionEnter(CCollider* _pOther)
{
    GameObject* pOtherObj = _pOther->GetObj();

    // 플레이어와 충돌 시
    if (pOtherObj->GetGroup() == GROUP_TYPE::PLAYER)
    {
        // TODO: 플레이어에게 데미지 적용
        // SPlayer* pPlayer = dynamic_cast<SPlayer*>(pOtherObj);
        // if (pPlayer) {
        //     pPlayer->TakeDamage(m_iDamage);
        // }
        
        ReturnToPool(); // 충돌 후 풀로 반환
    }
    // 다른 몬스터와 충돌 방지 (자신을 발사한 몬스터 포함)
    else if (pOtherObj->GetGroup() == GROUP_TYPE::MONSTER || pOtherObj->GetGroup() == GROUP_TYPE::MONSTER_HEAD)
    {
        // 몬스터가 발사한 총알이 다른 몬스터와 충돌하는 것을 막거나,
        // 자신을 발사한 몬스터와 충돌하는 것을 막기 위해 추가 로직 필요
        // 현재는 아무것도 하지 않음 (통과)
    }
    // 지형과 충돌 시
    else if (pOtherObj->GetGroup() == GROUP_TYPE::GROUND)
    {
        ReturnToPool(); // 충돌 후 풀로 반환
    }
}

void CBullet::Reset()
{
    GameObject::Reset(); // GameObject의 Reset 호출
    m_fSpeed = 0.f;
    m_fRange = 0.f;
    m_iDamage = 0;
    m_pBulletTex = nullptr; // 텍스처 포인터 초기화 (SetBulletInfo에서 다시 설정)
    m_pOwnerMonster = nullptr;
    m_fDistanceTraveled = 0.f;

    // 충돌체 초기화
    if (GetCollider())
    {
        GetCollider()->SetActive(true);
        GetCollider()->SetScale(Vec2(20.f, 20.f)); // 기본 총알 크기
        GetCollider()->SetOffsetPos(Vec2(0.f, 0.f));
    }
    // 애니메이터 초기화는 GameObject::Reset()에서 처리됨
}
