#pragma once
#include "CMonster.h"

class CShooterMonster : CMonster
{

private:
    void Update_Gravity();
    void Update_Animation();
    
public:
    virtual void OnCollisionEnter(CCollider* _pOther);
    virtual void OnCollision(CCollider* _pOther);
    virtual void OnCollisionExit(CCollider* _pOther);
    

    virtual void Start();
    virtual void Update();
    virtual void Render(HDC _dc);
    virtual void FinalUpdata();
    virtual void Reset();
};
