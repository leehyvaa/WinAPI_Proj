#include "GameObject.h"
#include "CKeyMgr.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CRigidBody.h"
#include "CGravity.h"
#include "CScene.h"
#include "CSceneMgr.h"
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
    
    // 비활성화 시 충돌체도 비활성화
    if (m_pCollider)
        m_pCollider->SetActive(_bActive);
}

void GameObject::Reset()
{
    // 오브젝트의 기본 속성 초기화
    m_bActive = true;
    m_bAlive = true;
    m_vDir = Vec2(0.f, 0.f);
    m_fLocalRotation = 0.f;
    
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
    dir.Normalize(); // 정규화는 한 번만 수행

    float angle = atan2(dir.y, dir.x); // 라디안 값 반환
    // 라디안 -> 각도 변환 및 적절한 회전으로 보정
    m_fLocalRotation = angle * (180.f / 3.14159f) + 90.f;
    // m_vDir에는 이미 정규화 된 방향 벡터 저장
    m_vDir = dir;
}

void GameObject::SetWorldPos(Vec2 _vWorldPos)
{
    if (m_pParent) {
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
    else {
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
    if (!m_bActive)
        return;

    // 1. 컴포넌트 업데이트 (특히 위치에 영향을 줄 수 있는 RigidBody먼저)
    //    순서는 중요할 수 있음: Gravity -> RigidBody -> Animator ->Collider
    if (m_pGravity)
        m_pGravity->FinalUpdate();
    if (m_pRigidBody)
        m_pRigidBody->FinalUpdate(); // RigidBody가 m_vPos/m_vLocalPos를확정하도록 함

    // 2. 부모-자식 관계에 따른 최종 월드 위치(m_vPos) 계산
    if (m_pParent)
    {
        // 부모의 최종 월드 회전을 가져옴
        float parentRotationRad = m_pParent->GetWorldRotation() *
            (3.14159f / 180.f);
        float cosAngle = cosf(parentRotationRad);
        float sinAngle = sinf(parentRotationRad);

        // 자신의 로컬 위치를 부모의 회전에 맞춰 변환 (회전된 오프셋계산)
        Vec2 rotatedLocalPos;
        rotatedLocalPos.x = m_vLocalPos.x * cosAngle - m_vLocalPos.y *
            sinAngle;
        rotatedLocalPos.y = m_vLocalPos.x * sinAngle + m_vLocalPos.y *
            cosAngle;

        // 부모의 최종 월드 위치(RigidBody 업데이트 후)에 회전된 로컬오프셋을 더함
        m_vPos = m_pParent->GetWorldPos() + rotatedLocalPos;
    }
    else
    {
        // 부모가 없는 경우, RigidBody 등이 업데이트한 m_vPos 또는m_vLocalPos 사용
        // 만약 RigidBody가 m_vPos를 직접 업데이트했다면 그대로 두고,
        // m_vLocalPos만 업데이트했다면 여기서 m_vPos = m_vLocalPos; 수행
        // 현재 코드는 m_vLocalPos를 사용하는 것으로 보이므로 아래 유지:
        m_vPos = m_vLocalPos;
    }

    // 3. 나머지 컴포넌트 업데이트 (최종 위치 기반)
    if (m_pAnimator)
        m_pAnimator->FinalUpdate(); // Animator는 최종 m_vPos 기반으로상태 업데이트
    if (m_pCollider)
        m_pCollider->FinalUpdate(); // Collider는 최종 m_vPos 기반으로위치 설정
}

void GameObject::Render(HDC _dc)
{
    if (!m_bActive)
        return;
    // 이미지가 없어서 박스 렌더해야 할때 주석 풀고 사용
	// Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(m_vPos);
	//
	// Rectangle(_dc, static_cast<int>(vRenderPos.x - m_vScale.x / 2.f)
	// 	, static_cast<int>(vRenderPos.y - m_vScale.y / 2.f)
	// 	, static_cast<int>(vRenderPos.x + m_vScale.x / 2.f)
	// 	, static_cast<int>(vRenderPos.y + m_vScale.y / 2.f));


	Component_Render(_dc);
}

void GameObject::Component_Render(HDC _dc)
{
	if (nullptr != m_pAnimator)
	{
		m_pAnimator->Render(_dc);
	}
	if (nullptr != m_pCollider && CSceneMgr::GetInst()->GetCurScene()->GetDrawCollider())
	{
		m_pCollider->Render(_dc);
	}

}



