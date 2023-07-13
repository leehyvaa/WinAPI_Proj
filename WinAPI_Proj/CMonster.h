#pragma once
#include "GameObject.h"
class CMonster :
    public GameObject
{
private:
    float m_fSpeed;
public:
    CMonster();
    ~CMonster();
    virtual void Update();
    virtual void Render(HDC _dc);


};

