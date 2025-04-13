#include "pch.h"
#include "CCollider.h"
#include "GameObject.h"
#include "CCore.h"
#include "SelectGDI.h"
#include "CCamera.h"

UINT CCollider::g_iNextID = 0;


CCollider::CCollider()
    : m_pOwner(nullptr)
    , m_iID(g_iNextID++)
    , m_iCol(0)
    , m_bActive(true)
    , m_vOffsetPos(Vec2(0.f, 0.f))
    , m_vScale(Vec2(0.f, 0.f))
    , m_vHalfExtents(Vec2(0.f, 0.f))
    , m_vFinalPos(Vec2(0.f, 0.f))
    , m_fWorldRotation(0.f)
{
    m_vAxes[0] = Vec2(1.f, 0.f); // 초기 X축
    m_vAxes[1] = Vec2(0.f, 1.f); // 초기 Y축
}

CCollider::CCollider(const CCollider& _origin)
    : m_pOwner(nullptr) // 소유자는 복사하지 않음 (새 오브젝트에 할당해야 함)
    , m_vOffsetPos(_origin.m_vOffsetPos)
    , m_vScale(_origin.m_vScale)
    , m_vHalfExtents(_origin.m_vHalfExtents)
    , m_iID(g_iNextID++) // ID 새로 발급
    , m_iCol(0)          // 충돌 상태는 초기화
    , m_bActive(true)    // 활성 상태로 시작
    , m_vFinalPos(Vec2(0.f, 0.f))
    , m_fWorldRotation(0.f)     
{
    m_vAxes[0] = Vec2(1.f, 0.f);
    m_vAxes[1] = Vec2(0.f, 1.f);
}

CCollider::~CCollider()
{
}

void CCollider::FinalUpdate()
{
    if (!m_bActive || !m_pOwner) // 활성화 상태 및 소유자 유효성 검사
        return;

    // 소유자 로부터 월드 위치 및 회전(라디안) 가져오기
    Vec2 vObjectPos = m_pOwner->GetWorldPos();
    // GetWorldRotation()은 도(degree)를 반환한다고 가정, 라디안으로 변환
    m_fWorldRotation = m_pOwner->GetWorldRotation() * (3.14159265f / 180.f);

    // 오프셋을 고려한 최종 월드 위치 계산
    // 오프셋도 회전의 영향을 받음
    float cosR = cos(m_fWorldRotation);
    float sinR = sin(m_fWorldRotation);
    Vec2 rotatedOffset;
    rotatedOffset.x = m_vOffsetPos.x * cosR - m_vOffsetPos.y * sinR;
    rotatedOffset.y = m_vOffsetPos.x * sinR + m_vOffsetPos.y * cosR;
    m_vFinalPos = vObjectPos + rotatedOffset; // 최종 중심 위치

    // 3. 월드 회전값을 기반으로 로컬 축 벡터 계산 (정규화)
    m_vAxes[0] = Vec2(cosR, sinR); // 회전된 X축
    m_vAxes[1] = Vec2(-sinR, cosR); // 회전된 Y축 (X축에서 +90도 회전)
    
	assert(0 <= m_iCol);
}

void CCollider::Render(HDC _dc)
{
    PEN_TYPE ePen = PEN_TYPE::GREEN;
    if (m_iCol > 0) // 충돌 중이면 빨간색
        ePen = PEN_TYPE::RED;
    else if (!m_bActive) // 비활성이면 파란색
        ePen = PEN_TYPE::BLUE;


    SelectGDI p(_dc, ePen);
    SelectGDI b(_dc, BRUSH_TYPE::HOLLOW);

    // OBB의 네 꼭짓점 계산 (월드 좌표)
    Vec2 halfExtents = GetHalfExtents();
    const Vec2* axes = GetAxes(); // 미리 계산된 축 벡터 사용

    // 로컬 좌표계 기준 꼭짓점 오프셋
    Vec2 offset1 = axes[0] * halfExtents.x + axes[1] * halfExtents.y; // 우상단
    Vec2 offset2 = axes[0] * -halfExtents.x + axes[1] * halfExtents.y; // 좌상단
    Vec2 offset3 = axes[0] * -halfExtents.x + axes[1] * -halfExtents.y; // 좌하단
    Vec2 offset4 = axes[0] * halfExtents.x + axes[1] * -halfExtents.y; // 우하단

    // 월드 좌표계 꼭짓점
    Vec2 corners[4];
    corners[0] = GetFinalPos() + offset1;
    corners[1] = GetFinalPos() + offset2;
    corners[2] = GetFinalPos() + offset3;
    corners[3] = GetFinalPos() + offset4;

    // 카메라 적용된 렌더링 좌표 계산
    Vec2 renderCorners[4];
    for (int i = 0; i < 4; ++i)
    {
        renderCorners[i] = CCamera::GetInst()->GetRenderPos(corners[i]);
    }

    // OBB 그리기 (선 사용)
    MoveToEx(_dc, static_cast<int>(renderCorners[0].x), static_cast<int>(renderCorners[0].y), nullptr);
    LineTo(_dc, static_cast<int>(renderCorners[1].x), static_cast<int>(renderCorners[1].y));
    LineTo(_dc, static_cast<int>(renderCorners[2].x), static_cast<int>(renderCorners[2].y));
    LineTo(_dc, static_cast<int>(renderCorners[3].x), static_cast<int>(renderCorners[3].y));
    LineTo(_dc, static_cast<int>(renderCorners[0].x), static_cast<int>(renderCorners[0].y));
}

void CCollider::OnCollision(CCollider* _pOther)
{
	m_pOwner->OnCollision(_pOther);
}

void CCollider::OnCollisionEnter(CCollider* _pOther)
{
	++m_iCol;
	m_pOwner->OnCollisionEnter(_pOther);
}

void CCollider::OnCollisionExit(CCollider* _pOther)
{
    if (m_iCol > 0)
        --m_iCol;
	m_pOwner->OnCollisionExit(_pOther);
}
