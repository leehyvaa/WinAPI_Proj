﻿#pragma once
#include "CState.h"

class CSubduedState : public CState
{
private:
    Vec2 m_vFixedPosition;       // 제압 시작 시 고정 위치
    bool m_bIsBeingSubdued;      // 현재 제압 중인지 여부
    float m_fExecutionTimer;     // 처형 애니메이션 타이머
    bool m_bIsExecuted;          // 처형 상태 여부

public:
    virtual void Update();
    virtual void Enter();
    virtual void Exit();


    void SetFixedPosition(Vec2 _vPos) { m_vFixedPosition = _vPos; }
    Vec2 GetFixedPosition() const { return m_vFixedPosition; }
    bool IsBeingSubdued() const { return m_bIsBeingSubdued; }
    void SetSubduedStatus(bool _bStatus) { m_bIsBeingSubdued = _bStatus; }
    

    void StartExecution();  
    bool IsExecuted() const { return m_bIsExecuted; }
    float GetExecutionTimer() const { return m_fExecutionTimer; }

public:
    CSubduedState();
    ~CSubduedState();
};
