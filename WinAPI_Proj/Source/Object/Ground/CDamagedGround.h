#pragma once
#include "CGround.h"

class SPlayer;

class CDamagedGround : public CGround
{
private:
    SPlayer* m_pPlayerInside;    // 현재 영역 안에 있는 플레이어
    float m_fDamageTimer;        // 데미지 타이머
    const float m_fDamageInterval; // 데미지 간격 (1초)
    const int m_iDamageAmount;   // 데미지 양
    const float m_fPushForce;    // 밀어내는 힘

public:
    CDamagedGround();
    virtual ~CDamagedGround();

    virtual void Update() override;
    virtual void OnCollisionEnter(CCollider* _pOther) override;
    virtual void OnCollision(CCollider* _pOther) override;
    virtual void OnCollisionExit(CCollider* _pOther) override;

    CLONE(CDamagedGround)

private:
    void DamageAndPushPlayer();
    Vec2 CalculatePushDirection(SPlayer* _pPlayer);
};
