#pragma once
#include "CNormalGround.h"

struct ID2D1RenderTarget;

class CWall : public CNormalGround
{
private:
    WALL_STATE m_eCurState;      // 현재 벽 상태
    WALL_STATE m_ePrevState;     // 이전 벽 상태
    wstring m_strWallType;       // 벽 타입 (Gate1, Gate2 등)
    bool m_bIsHorizontal;        // 수평/수직 방향 여부

public:
    CWall();
    virtual ~CWall();

    // 상태 관련 메서드
    void ChangeState(WALL_STATE _eNextState);
    WALL_STATE GetState() const { return m_eCurState; }
    WALL_STATE GetPrevState() const { return m_ePrevState; }

    // 벽 설정 메서드
    void SetWallType(const wstring& _strType) { m_strWallType = _strType; }
    void SetHorizontal(bool _bHorizontal) { m_bIsHorizontal = _bHorizontal; }
    const wstring& GetWallType() const { return m_strWallType; }
    bool IsHorizontal() const { return m_bIsHorizontal; }

    // 상태 제어 메서드
    void OpenWall();
    void CloseWall();

    virtual void Start() override;
    virtual void Update() override;

private:
    // 상태패턴 메서드들
    void Enter_State(WALL_STATE _eState);
    void Update_State();
    void Exit_State(WALL_STATE _eState);
    void Update_Animation();

    // 애니메이션 설정 메서드
    void SetupAnimations();

    // 오프셋 계산 메서드
    Vec2 CalculateAnimationOffset();

public:
    CLONE(CWall)
};
