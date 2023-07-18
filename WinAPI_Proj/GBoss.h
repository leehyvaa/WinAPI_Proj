#pragma once
#include "GameObject.h"
class GBoss :
    public GameObject
{
private:
    float m_fCountTimer;
    float m_fStartTimer;
public:
    GBoss();
    GBoss(Vec2 _vPos, Vec2 _vScale);
    virtual ~GBoss();

    virtual void Update() override;
    virtual void Render(HDC _dc) override;

    void Move();


};

