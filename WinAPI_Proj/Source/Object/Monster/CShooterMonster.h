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
    CShooterHead* m_pHead;
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
    virtual void Reset();

    CShooterHead* GetHead(){return m_pHead;}
    CLONE(CShooterMonster)
};
