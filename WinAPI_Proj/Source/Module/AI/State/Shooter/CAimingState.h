#pragma once
#include "CState.h"

class CAimingState : public CState
{
public:
    CAimingState();
    ~CAimingState();
    
private:
    float m_fShotDelay; // 총알 발사 딜레이
    float m_fShotTimer; // 총알 발사 타이머

    void FireBullet(); // 총알 발사 메서드
    
public:
    virtual void Enter();
    virtual void Update();
    virtual void Exit();
};
