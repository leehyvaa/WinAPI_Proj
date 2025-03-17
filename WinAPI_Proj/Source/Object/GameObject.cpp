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
    Vec2 dir = _target - GetPos();
    dir.Normalize(); // 정규화는 한 번만 수행

    float angle = atan2(dir.y, dir.x); // 라디안 값 반환
    // 라디안 -> 각도 변환 및 적절한 회전으로 보정
    m_fLocalRotation = angle * (180.f / 3.14159f) + 90.f;
    // m_vDir에는 이미 정규화 된 방향 벡터 저장
    m_vDir = dir;
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
	if (m_pAnimator)
		m_pAnimator->FinalUpdate();

	if (m_pGravity)
		m_pGravity->FinalUpdate();

	if (m_pRigidBody)
		m_pRigidBody->FinalUpdate();

	if (m_pCollider)
		m_pCollider->FinalUpdate();
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



