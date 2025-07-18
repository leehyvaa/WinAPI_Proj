﻿// WinAPI_Proj\Source\Object\Ground\CSkylineCar.cpp
#include "CSkylineCar.h"
#include "pch.h"

#include "CAnimator.h"
#include "CAnimation.h"
#include "CResMgr.h"
#include "CTimeMgr.h"
#include "SPlayer.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CHook.h"
#include "CCollider.h"
#include "CCollisionMgr.h"
#include "CGravity.h"
#include "CRigidBody.h"
#include "func.h"

CSkylineCar::CSkylineCar()
    : m_eState(SKYLINE_CAR_STATE::IDLE)
    , m_iCurrentPathIndex(0)
    , m_fSpeed(300.f) // 이동 속도
    , m_fCurrentSpeed(0.f)
    , m_fAccelerationTime(0.f)
    , m_vVelocity(Vec2(0.f, 0.f))
    , m_vStartPos(Vec2(-10000.f, -10000.f)) // 눈에 안보이는 위치로 초기화
    , m_fExplosionTimer(0.f)
    , m_bIsVertical(false) // 수직/수평 상태. 기본값은 수평(false)입니다.
{
    SetActive(false); // 기본적으로 비활성화
    SetName(L"SkylineCar");
    SetGroup(GROUP_TYPE::GROUND);
    // CNormalGround에서 이미 CreateCollider() 호출됨
    SetScale(Vec2(400.f, 100.f));
    // CNormalGround는 기본적으로 애니메이터가 없으므로 생성
    CreateAnimator();
    CreateRigidBody();
    CreateGravity();
    GetGravity()->SetApplyGravity(false);

    // 충돌 판정은 Ground와 동일하게 설정
    SetGroundType(GROUND_TYPE::NORMAL);
    SetCollideType(TILE_COLLIDE_TYPE::SOLID);

    // 애니메이션 설정
    SetupAnimations();

    // 초기 애니메이션 재생 (기본은 수평 IDLE)
    GetAnimator()->Play(L"SKYLINE_WIDE_IDLE", true);
    SetWorldPos(m_vStartPos);
}

CSkylineCar::~CSkylineCar()
{
}

void CSkylineCar::Start()
{
    CNormalGround::Start();
}

void CSkylineCar::SetupAnimations()
{
    // 텍스처 로드 
    CTexture* pTexWide = CResMgr::GetInst()->LoadTexture(L"SkylineCarTex_Wide", L"texture\\car\\SkylineCar_Wide.png");
    CTexture* pTexVert = CResMgr::GetInst()->LoadTexture(L"SkylineCarTex_Vert", L"texture\\car\\SkylineCar_Vert.png");

    float fMultiple = 2.05f;
    float fDuration = 0.1f; 
    Vec2 vOffset = Vec2(200.f, 15.f); // 기본 오프셋. 필요 시 조정합니다.

    // Wide (수평) 애니메이션
    GetAnimator()->CreateAnimation(L"SKYLINE_WIDE_SPAWNING", pTexWide,
        Vec2(0.f, 0.f), Vec2(200.f, 90.f), Vec2(200.f, 0.f), fDuration, 9, fMultiple, vOffset);
    GetAnimator()->CreateAnimation(L"SKYLINE_WIDE_IDLE", pTexWide,
        Vec2(0.f, 90.f), Vec2(200.f, 90.f), Vec2(200.f, 0.f), fDuration, 6, fMultiple, vOffset);
    GetAnimator()->CreateAnimation(L"SKYLINE_WIDE_MOVING", pTexWide,
        Vec2(0.f, 180.f), Vec2(200.f, 90.f), Vec2(200.f, 0.f), fDuration, 5, fMultiple, vOffset);
    GetAnimator()->CreateAnimation(L"SKYLINE_WIDE_DEAD", pTexWide,
        Vec2(0.f, 270.f), Vec2(200.f, 90.f), Vec2(200.f, 0.f), fDuration, 1, fMultiple, vOffset);

    // Vert (수직) 애니메이션
    GetAnimator()->CreateAnimation(L"SKYLINE_VERT_SPAWNING", pTexVert,
        Vec2(0.f, 0.f), Vec2(100.f, 250.f), Vec2(100.f, 0.f), fDuration, 9, fMultiple, vOffset);
    GetAnimator()->CreateAnimation(L"SKYLINE_VERT_IDLE", pTexVert,
        Vec2(0.f, 250.f), Vec2(100.f, 250.f), Vec2(100.f, 0.f), fDuration, 6, fMultiple, vOffset);
    GetAnimator()->CreateAnimation(L"SKYLINE_VERT_MOVING", pTexVert,
        Vec2(0.f, 500.f), Vec2(100.f, 250.f), Vec2(100.f, 0.f), fDuration, 5, fMultiple, vOffset);
    GetAnimator()->CreateAnimation(L"SKYLINE_VERT_DEAD", pTexVert,
        Vec2(0.f, 750.f), Vec2(100.f, 250.f), Vec2(100.f, 0.f), fDuration, 1, fMultiple, vOffset);
}


void CSkylineCar::Update_Animation()
{
    CAnimator* pAnimator = GetAnimator();
    if (!pAnimator) return;

    // 수직/수평 상태에 따라 애니메이션 이름의 접두사를 결정
    wstring prefix = m_bIsVertical ? L"SKYLINE_VERT_" : L"SKYLINE_WIDE_";
    wstring state_name;
    bool bRepeat = true;

    switch (m_eState)
    {
    case SKYLINE_CAR_STATE::IDLE:
        state_name = L"IDLE";
        break;
    case SKYLINE_CAR_STATE::MOVING:
        state_name = L"MOVING";
        break;
    case SKYLINE_CAR_STATE::EXPLODING: 
        state_name = L"DEAD";
        bRepeat = true;
        break;
    case SKYLINE_CAR_STATE::SPAWNING:
        state_name = L"SPAWNING";
        bRepeat = false;
        break;
    default:
        return; 
    }
    
    wstring nextAnimName = prefix + state_name;

    // 현재 재생 중인 애니메이션이 다음에 재생할 애니메이션과 다를 경우에만 변경
    if (pAnimator->GetCurAnimation() == nullptr || pAnimator->GetCurAnimation()->GetName() != nextAnimName)
    {
        pAnimator->Play(nextAnimName, bRepeat);
    }
}

void CSkylineCar::Update()
{
    // 상태에 따른 업데이트 로직
    switch (m_eState)
    {
    case SKYLINE_CAR_STATE::IDLE:       Update_Idle();       break;
    case SKYLINE_CAR_STATE::MOVING:     Update_Moving();     break;
    case SKYLINE_CAR_STATE::EXPLODING:  Update_Exploding();  break;
    case SKYLINE_CAR_STATE::SPAWNING:   Update_Spawning();   break;
    }

    // 애니메이션 업데이트
    Update_Animation();
}

void CSkylineCar::Update_Idle()
{
    // IDLE 상태에서는 아무것도 하지 않음. OnCollisionEnter에서 상태 변경을 기다림.
    m_vVelocity = Vec2(0.f, 0.f);
}

void CSkylineCar::Update_Moving()
{
    if (m_vecPath.empty() || m_iCurrentPathIndex >= m_vecPath.size())
    {
        m_eState = SKYLINE_CAR_STATE::EXPLODING; // 경로가 없으면 바로 폭발
        m_fExplosionTimer = 0.f; // 폭발 타이머 초기화
        m_vVelocity = Vec2(0.f, 0.f);
        return;
    }

    // 가속 처리
    const float ACCELERATION_DURATION = 2.0f;
    if (m_fAccelerationTime < ACCELERATION_DURATION)
    {
        m_fAccelerationTime += fDT;
        // Lerp를 이용한 부드러운 가속
        m_fCurrentSpeed = m_fSpeed * (m_fAccelerationTime / ACCELERATION_DURATION);
        if (m_fCurrentSpeed > m_fSpeed) m_fCurrentSpeed = m_fSpeed;
    }
    else
    {
        m_fCurrentSpeed = m_fSpeed;
    }

    // 목표 지점
    Vec2 vTargetPos = m_vecPath[m_iCurrentPathIndex];
    Vec2 vPos = GetWorldPos();

    Vec2 vDir = vTargetPos - vPos;
    float dist = vDir.Length();
    vDir.Normalize();

    Vec2 vMoveDelta = vDir * m_fCurrentSpeed * fDT;
    vPos += vMoveDelta;
    SetWorldPos(vPos);

    m_vVelocity = (fDT > 0.f) ? vMoveDelta / fDT : Vec2(0.f, 0.f);

    if (dist < m_fCurrentSpeed * fDT)
    {
        SetWorldPos(vTargetPos); // 정확한 위치로 보정
        m_iCurrentPathIndex++;
        if (m_iCurrentPathIndex >= m_vecPath.size())
        {
            m_eState = SKYLINE_CAR_STATE::EXPLODING;
            m_fExplosionTimer = 0.f; // 폭발 타이머 초기화
            m_vVelocity = Vec2(0.f, 0.f);
        }
    }
}

void CSkylineCar::Update_Exploding()
{
    // 폭발 시작 시 한 번만 실행
    if (m_fExplosionTimer == 0.f)
    {
        // 자식으로 등록된 플레이어가 있다면 부모-자식 관계를 해제합니다.
        SPlayer* pPlayer = dynamic_cast<SPlayer*>(CSceneMgr::GetInst()->GetCurScene()->GetPlayer());
        if (pPlayer && pPlayer->GetParent() == this)
        {
            Vec2 playerWorldPos = pPlayer->GetWorldPos();
            pPlayer->SetParent(nullptr);
            pPlayer->SetWorldPos(playerWorldPos);

            // [BUG FIX] 플레이어가 더 이상 땅 위에 있지 않다고 명시적으로 알려줍니다.
            pPlayer->SetOnGround(false);
            if (pPlayer->GetGravity())
            {
                pPlayer->GetGravity()->SetApplyGravity(true);
            }
        }

        // 물리 효과 적용
        if (GetRigidBody() && GetGravity())
        {
            GetCollider()->SetActive(false); // 더 이상 충돌하지 않도록 비활성화
            SetLocalRotation(30.f); // 옆으로 30도 회전
            GetRigidBody()->SetVelocity(Vec2(0.f, 0.f));
            GetRigidBody()->AddForce(Vec2(0.f, -50000.f)); // 위로 힘을 가함
            GetGravity()->SetApplyGravity(true); // 중력 적용하여 떨어지게 함
        }
    }

    m_fExplosionTimer += fDT;

    // 2초 후 리스폰 시작
    if (m_fExplosionTimer >= 3.f)
    {
        m_eState = SKYLINE_CAR_STATE::SPAWNING;

        // 물리 상태 초기화 후 스폰 위치로 이동
        if (GetRigidBody() && GetGravity())
        {
            GetRigidBody()->SetVelocity(Vec2(0.f, 0.f));
            GetGravity()->SetApplyGravity(false);
        }
        SetLocalRotation(0.f);
        SetWorldPos(m_vStartPos);
        m_fExplosionTimer = 0.f; // 타이머 리셋
    }
}

void CSkylineCar::Update_Spawning()
{
    m_vVelocity = Vec2(0.f, 0.f);
    if (GetAnimator()->GetCurAnimation() && GetAnimator()->GetCurAnimation()->IsFinish())
    {
        Reset(); // 리스폰 애니메이션 완료 후 전체 상태 초기화
    }
}

void CSkylineCar::OnCollision(CCollider* _pOther)
{
    // 1. CNormalGround의 기본 충돌 로직을 먼저 수행합니다.
    //    - 물리적 밀어내기(MTV) 및 벽타기(SetWallClimbing) 로직이 여기서 처리됩니다.
    CNormalGround::OnCollision(_pOther);

    // 2. 충돌한 객체가 플레이어일 경우, CSkylineCar만의 특별한 로직을 추가합니다.
    GameObject* pOtherObj = _pOther->GetObj();
    if (pOtherObj->GetGroup() == GROUP_TYPE::PLAYER)
    {
        // Car가 움직이는 상태가 아닐 경우(예: 폭발 중, 스폰 중), 자식으로 편입시키지 않습니다.
        // 이렇게 하면 종착지에서 자식 관계를 해제한 후, 충돌 판정으로 인해 다시 자식이 되는 것을 방지합니다.
        if (m_eState != SKYLINE_CAR_STATE::MOVING)
        {
            return;
        }

        SPlayer* pPlayer = static_cast<SPlayer*>(pOtherObj);

        CCollider* pPlayerCollider = pPlayer->GetCollider();
        CCollider* pCarCollider = this->GetCollider();
        Vec2 mtvDirection;
        float mtvDepth;

        // 플레이어가 발판 '위'에 있는지 정확히 판단하기 위해 충돌 정보를 다시 계산합니다.
        if (CCollisionMgr::GetInst()->CalculateCollisionInfo(pPlayerCollider, pCarCollider, mtvDirection, mtvDepth))
        {
            Vec2 groundUpNormal = Vec2(0.f, -1.f);
            float verticalDot = mtvDirection.Dot(groundUpNormal);

            // 충돌 방향이 '위쪽'(즉, 플레이어가 발판에 의해 위로 밀림)에 가까운지 확인합니다.
            bool isOnTop = (abs(verticalDot) > 0.707f);

            if (isOnTop)
            {
                // 플레이어의 Y축 속도를 발판의 속도와 동기화하여 분리를 방지합니다.
                // 이것이 FALL/IDLE 반복 현상을 막는 핵심입니다.
                pPlayer->GetRigidBody()->SetVelocityY(m_vVelocity.y);

                // 플레이어가 FALL 상태로 착지했다면, 즉시 IDLE 상태로 전환하여 초기 튕김을 막습니다.
                if (pPlayer->GetState() == PLAYER_STATE::FALL)
                {
                    pPlayer->ChangeState(PLAYER_STATE::IDLE);
                }

                // 플레이어를 발판의 자식으로 설정하여 함께 움직이도록 합니다.
                if (pPlayer->GetParent() != this)
                {
                    Vec2 worldPos = pPlayer->GetWorldPos();
                    pPlayer->SetParent(this);
                    pPlayer->SetWorldPos(worldPos);
                }
            }
            // 플레이어가 위에 있지 않고 이 차의 벽에 매달려 있다면 자식으로 설정합니다.
            else if (pPlayer->IsWallClimbing())
            {
                if (pPlayer->GetParent() != this)
                {
                    Vec2 worldPos = pPlayer->GetWorldPos();
                    pPlayer->SetParent(this);
                    pPlayer->SetWorldPos(worldPos);
                }
            }
        }
    }
}

void CSkylineCar::OnCollisionExit(CCollider* _pOther)
{
    CNormalGround::OnCollisionExit(_pOther);
    GameObject* pOtherObj = _pOther->GetObj();
    if (pOtherObj->GetParent() == this)
    {
        Vec2 worldPos = pOtherObj->GetWorldPos();
        pOtherObj->SetParent(nullptr);
        pOtherObj->SetWorldPos(worldPos);
    }
}

void CSkylineCar::OnCollisionEnter(CCollider* _pOther)
{
    CNormalGround::OnCollisionEnter(_pOther);

    GameObject* pOtherObj = _pOther->GetObj();
    GROUP_TYPE eOtherGroup = pOtherObj->GetGroup();

    if (m_eState == SKYLINE_CAR_STATE::IDLE && (eOtherGroup == GROUP_TYPE::PLAYER || eOtherGroup == GROUP_TYPE::HOOK))
    {
        m_eState = SKYLINE_CAR_STATE::MOVING;
        m_fCurrentSpeed = 0.f;
        m_fAccelerationTime = 0.f;
    }
}

void CSkylineCar::Reset()
{
    CNormalGround::Reset();

    m_eState = SKYLINE_CAR_STATE::IDLE;
    m_iCurrentPathIndex = 0;
    m_fCurrentSpeed = 0.f;
    m_fAccelerationTime = 0.f;
    m_fExplosionTimer = 0.f;

    SetWorldPos(m_vStartPos);
    SetLocalRotation(0.f);
    GetCollider()->SetActive(true);

    if (GetRigidBody() && GetGravity())
    {
        GetRigidBody()->SetVelocity(Vec2(0.f, 0.f));
        GetGravity()->SetApplyGravity(false);
    }

    if (GetAnimator())
    {
        wstring initialAnim = m_bIsVertical ? L"SKYLINE_VERT_IDLE" : L"SKYLINE_WIDE_IDLE";
        GetAnimator()->Play(initialAnim, true);
    }
}

void CSkylineCar::SetPath(const std::vector<Vec2>& _path)
{
    m_vecPath = _path;
    m_iCurrentPathIndex = 0;

    if (!_path.empty())
    {
        SetStartPos(_path.front());
    }
}

void CSkylineCar::AddPathPoint(const Vec2& point)
{
	m_vecPath.push_back(point);
}

void CSkylineCar::ClearPath()
{
    m_vecPath.clear();
    m_iCurrentPathIndex = 0;
    if (GetAnimator())
    {
        // 클리어 시에도 방향에 맞는 IDLE 애니메이션으로 설정합니다.
        wstring initialAnim = m_bIsVertical ? L"SKYLINE_VERT_IDLE" : L"SKYLINE_WIDE_IDLE";
        GetAnimator()->Play(initialAnim, true);
    }
    SetStartPos(Vec2(-10000.f, -10000.f)); // 눈에 안보이는 곳으로 이동
    SetActive(false);
}

void CSkylineCar::Save(FILE* _pFile)
{
    // 경로 포인트 개수 저장
    size_t pathCount = m_vecPath.size();
    fprintf(_pFile, "%zu\n", pathCount);

    // 각 경로 포인트 저장
    for (const auto& point : m_vecPath)
    {
        fprintf(_pFile, "%f %f\n", point.x, point.y);
    }
}

void CSkylineCar::Load(FILE* _pFile)
{
    char buf[256] = {};

    // 경로 포인트 개수 로드
    size_t pathCount = 0;
    FScanf(buf, _pFile);
    sscanf_s(buf, "%zu", &pathCount);

    m_vecPath.clear();
    m_vecPath.reserve(pathCount);

    // 각 경로 포인트 로드
    for (size_t i = 0; i < pathCount; ++i)
    {
        Vec2 point;
        FScanf(buf, _pFile);
        sscanf_s(buf, "%f %f", &point.x, &point.y);
        m_vecPath.push_back(point);
    }

    // 경로가 있으면 시작 위치 설정
    if (!m_vecPath.empty())
    {
        SetStartPos(m_vecPath[0]);
        SetActive(true);
    }
    else
    {
        SetActive(false);
    }
    m_iCurrentPathIndex = 0;
    m_eState = SKYLINE_CAR_STATE::IDLE;
}