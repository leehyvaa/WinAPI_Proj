#pragma once
#include "CMonster.h"

class CShooterHead;
struct ID2D1RenderTarget;

class CShooterMonster : public CMonster
{
public:
    CShooterMonster();
    ~CShooterMonster();
    CShooterMonster(const CShooterMonster& _origin)
        : CMonster(_origin)
    {
    }

private:
private:
    void Update_Gravity();

public:
    virtual void OnCollisionEnter(CCollider* _pOther);
    virtual void OnCollision(CCollider* _pOther);
    virtual void OnCollisionExit(CCollider* _pOther);
    

    
    virtual void Start();
    virtual void Update();
    virtual void Render(ID2D1RenderTarget* _pRenderTarget) override;
    virtual void FinalUpdata();
    virtual void Reset() override;

    CLONE(CShooterMonster)
};
