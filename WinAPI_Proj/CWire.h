#pragma once
#include "GameObject.h"
class CWire :
    public GameObject
{
private:
    Vec2 m_vDir;
    float m_fSpeed;

public:
    CWire();
    virtual ~CWire();

    void SetDir(Vec2 _dir) { m_vDir = _dir; }

    virtual void Update();
    virtual void Render(HDC _dc);


    virtual void OnCollisionEnter(CCollider* _pOther);
    CLONE(CWire);
};

