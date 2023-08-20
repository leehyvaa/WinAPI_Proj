#pragma once
#include "GameObject.h"
class CHook :
    public GameObject
{
private:
    Vec2 m_vDir;
    float m_fSpeed;

public:
    CHook();
    virtual ~CHook();

    void SetDir(Vec2 _dir) { m_vDir = _dir; }

    virtual void Update();
    virtual void Render(HDC _dc);


    virtual void OnCollisionEnter(CCollider* _pOther);
    CLONE(CHook);
};

