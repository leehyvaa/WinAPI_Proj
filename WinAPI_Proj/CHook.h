#pragma once
#include "GameObject.h"
class CHook :
    public GameObject
{
private:
    Vec2 m_vDir;
    float m_fSpeed;
    float m_rotation;


public:
    CHook();
    virtual ~CHook();

    void SetDir(Vec2 _dir) { m_vDir = _dir; }
    void LookAt(Vec2 _target);


    virtual void Update();
    virtual void Render(HDC _dc);


    virtual void OnCollisionEnter(CCollider* _pOther);
    CLONE(CHook);
};

