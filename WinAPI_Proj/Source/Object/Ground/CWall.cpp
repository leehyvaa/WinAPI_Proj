#include "pch.h"
#include "CWall.h"
#include "CCollider.h"
#include "CAnimation.h"
#include "CAnimator.h"
#include "CResMgr.h"

CWall::CWall()
    : m_eCurState(WALL_STATE::CLOSE)
    , m_ePrevState(WALL_STATE::CLOSE)
    , m_strWallType(L"Gate1")
    , m_bIsHorizontal(true)
{
    CreateCollider();
    SetGroup(GROUP_TYPE::GROUND);
    CreateAnimator();

    // 기본적으로 UNWALKABLE 타입으로 설정 (갈고리 및 벽타기 불가)
    SetGroundType(GROUND_TYPE::UNWALKABLE);
    SetCollideType(TILE_COLLIDE_TYPE::SOLID);

    // SetupAnimations()는 Start()에서 호출 (스케일 설정 후)
}

CWall::~CWall()
{
}

void CWall::SetupAnimations()
{
    // Gate 텍스처 로드
    CTexture* pTex_Gate1_Horizontal = CResMgr::GetInst()->LoadTexture(L"Gate1Tex_Horizontal", L"texture\\gate\\Gate1_Horizontal.png");
    CTexture* pTex_Gate1_Vertical = CResMgr::GetInst()->LoadTexture(L"Gate1Tex_Vertical", L"texture\\gate\\Gate1_Vertical.png");
    CTexture* pTex_Gate2_Horizontal = CResMgr::GetInst()->LoadTexture(L"Gate2Tex_Horizontal", L"texture\\gate\\Gate2_Horizontal.png");
    CTexture* pTex_Gate2_Vertical = CResMgr::GetInst()->LoadTexture(L"Gate2Tex_Vertical", L"texture\\gate\\Gate2_Vertical.png");

    // 오프셋 계산
    Vec2 vOffset = CalculateAnimationOffset();

    // Gate1 Horizontal 애니메이션
    GetAnimator()->CreateAnimation(L"Gate1_Horizontal_Closed", pTex_Gate1_Horizontal,
        Vec2(0.f, 0.f), Vec2(600.f, 100.f), Vec2(0.f, 100.f), 0.25f, 2, 0.7f, vOffset);
    GetAnimator()->CreateAnimation(L"Gate1_Horizontal_Closing", pTex_Gate1_Horizontal,
        Vec2(600.f, 0.f), Vec2(600.f, 100.f), Vec2(0.f, 100.f), 0.25f, 7, 0.7f, vOffset);
    GetAnimator()->CreateAnimation(L"Gate1_Horizontal_Opening", pTex_Gate1_Horizontal,
        Vec2(1200.f, 0.f), Vec2(600.f, 100.f), Vec2(0.f, 100.f), 0.25f, 7, 0.7f, vOffset);

    // Gate2 Horizontal 애니메이션
    GetAnimator()->CreateAnimation(L"Gate2_Horizontal_Closed", pTex_Gate2_Horizontal,
        Vec2(0.f, 0.f), Vec2(600.f, 100.f), Vec2(600.f, 0.f), 0.25f, 5, 0.7f, vOffset);
    GetAnimator()->CreateAnimation(L"Gate2_Horizontal_Closing", pTex_Gate2_Horizontal,
        Vec2(0.f, 100.f), Vec2(600.f, 100.f), Vec2(600.f, 0.f), 0.25f, 9, 0.7f, vOffset);
    GetAnimator()->CreateAnimation(L"Gate2_Horizontal_Opening", pTex_Gate2_Horizontal,
        Vec2(0.f, 200.f), Vec2(600.f, 100.f), Vec2(600.f, 0.f), 0.25f, 7, 0.7f, vOffset);

    // Gate1 Vertical 애니메이션
    GetAnimator()->CreateAnimation(L"Gate1_Vertical_Closed", pTex_Gate1_Vertical,
        Vec2(0.f, 0.f), Vec2(100.f, 600.f), Vec2(100.f, 0.f), 0.25f, 2, 0.7f, vOffset);
    GetAnimator()->CreateAnimation(L"Gate1_Vertical_Closing", pTex_Gate1_Vertical,
        Vec2(0.f, 600.f), Vec2(100.f, 600.f), Vec2(100.f, 0.f), 0.25f, 7, 0.7f, vOffset);
    GetAnimator()->CreateAnimation(L"Gate1_Vertical_Opened", pTex_Gate1_Vertical,
        Vec2(0.f, 1200.f), Vec2(100.f, 600.f), Vec2(100.f, 0.f), 0.25f, 1, 0.7f, vOffset);
    GetAnimator()->CreateAnimation(L"Gate1_Vertical_Opening", pTex_Gate1_Vertical,
        Vec2(0.f, 1800.f), Vec2(100.f, 600.f), Vec2(100.f, 0.f), 0.25f, 7, 0.7f, vOffset);

    // Gate2 Vertical 애니메이션
    GetAnimator()->CreateAnimation(L"Gate2_Vertical_Closed", pTex_Gate2_Vertical,
        Vec2(0.f, 0.f), Vec2(100.f, 600.f), Vec2(100.f, 0.f), 0.25f, 5, 0.7f, vOffset);
    GetAnimator()->CreateAnimation(L"Gate2_Vertical_Closing", pTex_Gate2_Vertical,
        Vec2(0.f, 600.f), Vec2(100.f, 600.f), Vec2(100.f, 0.f), 0.25f, 9, 0.7f, vOffset);
    GetAnimator()->CreateAnimation(L"Gate2_Vertical_Opening", pTex_Gate2_Vertical,
        Vec2(0.f, 1200.f), Vec2(100.f, 600.f), Vec2(100.f, 0.f), 0.25f, 7, 0.7f, vOffset);
}

void CWall::Start()
{
    CGround::Start();

    // 스케일이 설정된 후 애니메이션 설정
    SetupAnimations();

    // 초기 상태 설정
    Enter_State(m_eCurState);
}

void CWall::Update()
{
    Update_State();
    Update_Animation();
}

void CWall::ChangeState(WALL_STATE _eNextState)
{
    if (m_eCurState == _eNextState) return;

    Exit_State(m_eCurState);
    Enter_State(_eNextState);
    m_ePrevState = m_eCurState;
    m_eCurState = _eNextState;
}

void CWall::Enter_State(WALL_STATE _eState)
{
    switch (_eState)
    {
    case WALL_STATE::CLOSE:
        // 벽이 닫힌 상태 - 충돌 활성화
        GetCollider()->SetActive(true);
        SetGroundType(GROUND_TYPE::UNWALKABLE);
        break;
    case WALL_STATE::OPENING:
        // 벽이 열리는 중 - 충돌 유지
        GetCollider()->SetActive(true);
        SetGroundType(GROUND_TYPE::UNWALKABLE);
        break;
    case WALL_STATE::OPEN:
        // 벽이 열린 상태 - 오브젝트 자체를 비활성화
        SetActive(false);
        break;
    case WALL_STATE::CLOSING:
        // 벽이 닫히는 중 - 충돌 활성화
        GetCollider()->SetActive(true);
        SetGroundType(GROUND_TYPE::UNWALKABLE);
        break;
    }
}

void CWall::Update_State()
{
    switch (m_eCurState)
    {
    case WALL_STATE::CLOSE:
        // 닫힌 상태에서는 특별한 로직 없음
        break;
    case WALL_STATE::OPENING:
        // 열리는 애니메이션이 끝나면 OPEN 상태로 전환
        if (GetAnimator()->GetCurAnimation() && GetAnimator()->GetCurAnimation()->IsFinish())
        {
            ChangeState(WALL_STATE::OPEN);
        }
        break;
    case WALL_STATE::OPEN:
        // 열린 상태에서는 특별한 로직 없음
        break;
    case WALL_STATE::CLOSING:
        // 닫히는 애니메이션이 끝나면 CLOSE 상태로 전환
        if (GetAnimator()->GetCurAnimation() && GetAnimator()->GetCurAnimation()->IsFinish())
        {
            ChangeState(WALL_STATE::CLOSE);
        }
        break;
    }
}

void CWall::Exit_State(WALL_STATE _eState)
{
    switch (_eState)
    {
    case WALL_STATE::CLOSE:
        break;
    case WALL_STATE::OPENING:
        break;
    case WALL_STATE::OPEN:
        break;
    case WALL_STATE::CLOSING:
        break;
    }
}

void CWall::Update_Animation()
{
    wstring animName = L"";

    // 벽 타입과 방향에 따른 애니메이션 이름 생성
    switch (m_eCurState)
    {
    case WALL_STATE::CLOSE:
        animName = m_strWallType + (m_bIsHorizontal ? L"_Horizontal_Closed" : L"_Vertical_Closed");
        break;
    case WALL_STATE::OPENING:
        animName = m_strWallType + (m_bIsHorizontal ? L"_Horizontal_Opening" : L"_Vertical_Opening");
        break;
    case WALL_STATE::OPEN:
        animName = m_strWallType + (m_bIsHorizontal ? L"_Horizontal_Opened" : L"_Vertical_Opened");
        break;
    case WALL_STATE::CLOSING:
        animName = m_strWallType + (m_bIsHorizontal ? L"_Horizontal_Closing" : L"_Vertical_Closing");
        break;
    }

    // 애니메이션 재생
    if (!animName.empty() && GetAnimator())
    {
        CAnimation* pCurAnim = GetAnimator()->GetCurAnimation();
        if (!pCurAnim || pCurAnim->GetName() != animName)
        {
            bool bRepeat = (m_eCurState == WALL_STATE::CLOSE || m_eCurState == WALL_STATE::OPEN);
            GetAnimator()->Play(animName, bRepeat);
        }
    }
}

void CWall::OpenWall()
{
    if (m_eCurState == WALL_STATE::CLOSE)
    {
        ChangeState(WALL_STATE::OPENING);
    }
}

void CWall::CloseWall()
{
    if (m_eCurState == WALL_STATE::OPEN)
    {
        ChangeState(WALL_STATE::CLOSING);
    }
}

Vec2 CWall::CalculateAnimationOffset()
{
    // 벽의 현재 스케일 가져오기
    Vec2 vWallScale = GetScale();
    Vec2 vOffset;
    vOffset.x = vWallScale.x/2  ;
    vOffset.y = vWallScale.y/2 ;
    return vOffset;
}
