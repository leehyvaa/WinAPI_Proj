﻿#pragma once
#include "CState.h"

// Direct2D 전방 선언
struct ID2D1RenderTarget;

class CAimingState : public CState
{
public:
    CAimingState();
    ~CAimingState();
    
private:
    float m_fShotDelay; // 총알 발사 딜레이
    float m_fShotTimer; // 총알 발사 타이머

    void FireBullet();
    
public:
    virtual void Enter() override;
    virtual void Update() override;
    virtual void Exit() override;
    virtual void Render(ID2D1RenderTarget* _pRenderTarget);
};
