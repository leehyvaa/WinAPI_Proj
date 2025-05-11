#include "CShooterMonster.h"

void CShooterMonster::Update_Gravity()
{
}

void CShooterMonster::Update_Animation()
{
}



void CShooterMonster::Start()
{
    CMonster::Start();
}

void CShooterMonster::Update()
{
    CMonster::Update();
}

void CShooterMonster::Render(HDC _dc)
{
    CMonster::Render(_dc);
}

void CShooterMonster::FinalUpdata()
{
    CMonster::FinalUpdata();
}

void CShooterMonster::Reset()
{
    CMonster::Reset();
}

void CShooterMonster::OnCollisionEnter(CCollider* _pOther)
{
    CMonster::OnCollisionEnter(_pOther);
}

void CShooterMonster::OnCollision(CCollider* _pOther)
{
    CMonster::OnCollision(_pOther);
}

void CShooterMonster::OnCollisionExit(CCollider* _pOther)
{
    CMonster::OnCollisionExit(_pOther);
}