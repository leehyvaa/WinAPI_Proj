#pragma once
#include "GameObject.h"
class CMonster :
    public GameObject
{
private:
    Vec2 m_vCenterPos;
    float m_fSpeed;
    float m_fMaxDistance;
    int m_iHP;
    int m_iDir;
public:
    float GetSpeed() { return m_fSpeed; }
    void SetSpeed(float _f) { m_fSpeed = _f; }

    void SetCenterPos(Vec2 _vPos) { m_vCenterPos = _vPos; }

public:
    virtual void OnCollisionEnter(CCollider* _pOther);

    virtual void Update();
    //virtual void Render(HDC _dc);

    CMonster();
    ~CMonster();
};

