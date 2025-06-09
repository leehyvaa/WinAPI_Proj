#pragma once
#include "GameObject.h"

struct ID2D1RenderTarget;

class CShooterHead : public GameObject
{
private:
    
public:
    CShooterHead();
    ~CShooterHead();

    virtual void Reset() override;

    virtual void Update() override;
    virtual void RenderD2D(ID2D1RenderTarget* _pRenderTarget) override;

    CLONE(CShooterHead)
};
