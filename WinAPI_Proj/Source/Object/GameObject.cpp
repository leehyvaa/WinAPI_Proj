#include "GameObject.h"
#include "CKeyMgr.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CRigidBody.h"
#include "CGravity.h"
#include "CScene.h"
#include "CSceneMgr.h"
#include "CCore.h"
GameObject::GameObject()
    : m_vPos{}
    , m_vScale{}
    , m_pCollider(nullptr)
    , m_pAnimator(nullptr)
    , m_pRigidBody(nullptr)
    , m_pGravity(nullptr)
    , m_bAlive(true)
    , m_bActive(true)
    , m_vDir(Vec2(0.f, 0.f))
    , m_fLocalRotation(0.f)
    , m_vStandardDir(0.f,-1.f)
    , m_eGroup(GROUP_TYPE::END)
    , m_pParent(nullptr)
    , m_bManagedByPool(false)
    , m_vLocalPos(0.f, 0.f)
    , m_bIsFacingRight(true)
    , m_bIsFacingRightPrev(true)
    , m_cachedWorldRotation(0.f)
    , m_worldRotationDirty(true)
    , m_cachedParentWorldRotation(0.f)
    , m_hasCachedParentInfo(false)
{
}
GameObject::GameObject(const GameObject& _origin)
	: m_strName(_origin.m_strName)
	, m_vPos(_origin.m_vPos)
	, m_vScale(_origin.m_vScale)
	, m_pCollider(nullptr)
	, m_pAnimator(nullptr)
	, m_pRigidBody(nullptr)
	, m_pGravity(nullptr)
	, m_bAlive(true)
    , m_eGroup(GROUP_TYPE::END)
    , m_bIsFacingRight(_origin.m_bIsFacingRight)
    , m_bIsFacingRightPrev(_origin.m_bIsFacingRightPrev)
    , m_cachedWorldRotation(0.f)
    , m_worldRotationDirty(true)
    , m_cachedParentWorldRotation(0.f)
    , m_hasCachedParentInfo(false)
{
	if (_origin.m_pCollider)
	{
		m_pCollider = new CCollider(*_origin.m_pCollider);
		m_pCollider->m_pOwner = this;
	}
	if (_origin.m_pAnimator)
	{
		m_pAnimator = new CAnimator(*_origin.m_pAnimator);
		m_pAnimator->m_pOwner = this;
	}
	if (_origin.m_pRigidBody)
	{
		m_pRigidBody = new CRigidBody(*_origin.m_pRigidBody);
		m_pRigidBody->m_pOwner = this;
	}
	if (_origin.m_pGravity)
	{
		m_pGravity = new CGravity(*_origin.m_pGravity);
		m_pGravity->m_pOwner = this;
	}
}
GameObject::~GameObject()
{
    if (m_pParent)
    {
        m_pParent = nullptr;
    }
    
	if (nullptr != m_pCollider)
		delete m_pCollider;
	if (nullptr != m_pAnimator)
		delete m_pAnimator;
	if (nullptr != m_pRigidBody)
		delete m_pRigidBody;
	if (nullptr != m_pGravity)
		delete m_pGravity;
}

void GameObject::SetActive(bool _bActive)
{
    m_bActive = _bActive;
    
    if (m_pCollider)
        m_pCollider->SetActive(_bActive);
}

float GameObject::GetWorldRotation()
{
    bool needsRecompute = m_worldRotationDirty;

    if (!needsRecompute && m_pParent)
    {
        float currentParentActualWorldRotation = m_pParent->GetWorldRotation();
        if (!m_hasCachedParentInfo || m_cachedParentWorldRotation != currentParentActualWorldRotation)
            needsRecompute = true;
    }
    else if (!m_pParent && m_hasCachedParentInfo)
        needsRecompute = true;

    if (needsRecompute)
    {
        float parentActualWorldRotation = 0.f;
        if (m_pParent)
            parentActualWorldRotation = m_pParent->GetWorldRotation();
        
        m_cachedWorldRotation = parentActualWorldRotation + m_fLocalRotation; 
        m_worldRotationDirty = false;

        // 현재 계산에 사용된 부모의 월드 회전값을 캐시
        if (m_pParent)
        {
            m_cachedParentWorldRotation = parentActualWorldRotation;
            m_hasCachedParentInfo = true;
        }
        else
            m_hasCachedParentInfo = false;
    }
    return m_cachedWorldRotation;
}

void GameObject::Reset()
{
    // 오브젝트의 기본 속성 초기화
    m_bActive = true;
    m_bAlive = true;
    m_vDir = Vec2(0.f, 0.f);
    m_fLocalRotation = 0.f;
    m_pParent = nullptr;
    
    // 컴포넌트들 초기화
    if (m_pCollider)
        m_pCollider->Reset();
    if (m_pAnimator)
        m_pAnimator->Reset();
    if (m_pRigidBody)
        {
            m_pRigidBody->Reset();
            m_pRigidBody->SetVelocity(Vec2(0.f, 0.f));
        }
    if (m_pGravity)
        m_pGravity->Reset();
}


void GameObject::LookAt(Vec2 _target)
{
    Vec2 dir = _target - GetWorldPos();
    dir.Normalize();

    float angle = atan2(dir.y, dir.x); // 라디안 값 반환
    // 라디안 -> 각도 변환 및 보정
    m_fLocalRotation = angle * (180.f / 3.14159f) + 90.f;
    m_worldRotationDirty = true; 
    // m_vDir에 방향 벡터 저장
    m_vDir = dir;
}

void GameObject::SetWorldPos(Vec2 _vWorldPos)
{
    if (m_pParent)
    {
        // 부모 기준 로컬 좌표로 변환
        Vec2 parentWorldPos = m_pParent->GetWorldPos();
        
        // 부모의 회전 고려하여 역변환
        float radians = -m_pParent->GetWorldRotation() * (3.14159f / 180.f);
        float cosAngle = cosf(radians);
        float sinAngle = sinf(radians);
        
        Vec2 relativePos = _vWorldPos - parentWorldPos;
        
        // 회전 행렬의 역변환 적용
        m_vLocalPos.x = relativePos.x * cosAngle - relativePos.y * sinAngle;
        m_vLocalPos.y = relativePos.x * sinAngle + relativePos.y * cosAngle;
    }
    else
    {
        // 부모가 없으면 직접 위치 설정
        m_vPos = _vWorldPos;
        m_vLocalPos = _vWorldPos;
    }
}

Vec2 GameObject::GetWorldPos()
{
    if (m_pParent)
    {
        // 부모의 최종 월드 회전을 가져옴
        float parentRotationRad = m_pParent->GetWorldRotation() * (3.14159f / 180.f);
        float cosAngle = cosf(parentRotationRad);
        float sinAngle = sinf(parentRotationRad);

        // 자신의 로컬 위치를 부모의 회전에 맞춰 변환 (회전된 오프셋계산)
        Vec2 rotatedLocalPos;
        rotatedLocalPos.x = m_vLocalPos.x * cosAngle - m_vLocalPos.y * sinAngle;
        rotatedLocalPos.y = m_vLocalPos.x * sinAngle + m_vLocalPos.y * cosAngle;

        // 부모의 최종 월드 위치에 회전된 로컬 오프셋을 더함
        return m_pParent->GetWorldPos() + rotatedLocalPos;
    }

    // 부모가 없으면 자신의 월드 위치 (m_vPos는 FinalUpdate에서 계산된최종 값이어야 함)
    return m_vPos;
}


void GameObject::CreateCollider()
{
	m_pCollider = new CCollider;
	m_pCollider->m_pOwner = this;
}

void GameObject::CreateAnimator()
{
	m_pAnimator = new CAnimator;
	m_pAnimator->m_pOwner = this;
}

void GameObject::CreateRigidBody()
{
	m_pRigidBody = new CRigidBody;
	m_pRigidBody->m_pOwner = this;
}

void GameObject::CreateGravity()
{
	m_pGravity = new CGravity;
	m_pGravity->m_pOwner = this;
}

void GameObject::Update()
{
    if (!m_bActive)
        return;
}

void GameObject::FinalUpdate()
{
    if (!m_bActive || !m_bAlive) return;
    
    if (m_pGravity)
        m_pGravity->FinalUpdate();
    if (m_pRigidBody)
        m_pRigidBody->FinalUpdate(); 

        // 부모-자식 관계에 따른 최종 논리적 월드 위치 및 회전 계산
        if (m_pParent)
        {
            // 부모 정보 
            Vec2 parentWorldPos = m_pParent->GetWorldPos(); // 부모 논리적 월드 위치
            float parentWorldRotation = m_pParent->GetWorldRotation(); // 부모 최종 월드 회전
            float parentRotationRad = parentWorldRotation * (3.14159f / 180.f);
            float cosParent = cosf(parentRotationRad);
            float sinParent = sinf(parentRotationRad);

            // 자식 로컬 위치를 부모 회전에 맞춰 회전 (부모 기준 오프셋)
            Vec2 rotatedLocalPosOffset;
            rotatedLocalPosOffset.x = m_vLocalPos.x * cosParent - m_vLocalPos.y * sinParent;
            rotatedLocalPosOffset.y = m_vLocalPos.x * sinParent + m_vLocalPos.y * cosParent;

            // 최종 자식 논리적 위치 계산
            // 부모의 논리적 위치 + 회전된 자식의 로컬 오프셋
            m_vPos = parentWorldPos + rotatedLocalPosOffset;
        }
        else
        {
            // 부모가 없는 경우, 물리 효과를 받는 오브젝트는
            // RigidBody가 m_vPos를 직접 갱신했을 것이므로 덮어쓰지 않습니다.
            if (!m_pRigidBody)
            {
                m_vPos = m_vLocalPos;
            }
        }

    if (m_pAnimator)
        m_pAnimator->FinalUpdate(); 
    if (m_pCollider)
        m_pCollider->FinalUpdate();
}

void GameObject::Component_Render(ID2D1RenderTarget* _pRenderTarget)
{
	if (!_pRenderTarget)
		return;
    
	// 애니메이터 렌더링
	if (m_pAnimator)
		m_pAnimator->Render(_pRenderTarget);
	
	// 콜라이더 디버그 렌더링
	if (m_pCollider && CSceneMgr::GetInst()->GetCurScene()->GetDrawCollider())
		m_pCollider->Render(_pRenderTarget);
}



