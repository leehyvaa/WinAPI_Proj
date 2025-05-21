#pragma once
#include "GameObject.h"

class CShooterHead : public GameObject
{
private:
    
public:
    CShooterHead();
    ~CShooterHead();

    virtual void Update() override;
    virtual void Render(HDC _dc) override;

    CLONE(CShooterHead)
};
