#pragma once
#include "CMonster.h"

class CShooterHead;

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
    virtual void Render(HDC _dc);
    virtual void FinalUpdata();
    virtual void Reset() override;

    CLONE(CShooterMonster)
};
