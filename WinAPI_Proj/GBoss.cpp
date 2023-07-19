#include "pch.h"
#include "GBoss.h"
#include "CCore.h"
#include "GMap.h"
GBoss::GBoss()
{

}

GBoss::GBoss(Vec2 _vPos, Vec2 _vScale)
{

	m_fStartTimer = clock();
	SetPos(_vPos);
	SetScale(_vScale);
	objectType = Boss;
}

GBoss::~GBoss()
{

}

void GBoss::Update()
{
	float m_fCountTimer = clock();
	Vec2 m_vPos(GetPos().x, GetPos().y);

	
	

	if (m_fCountTimer- m_fStartTimer   > 2.f)
	{
		m_fStartTimer = clock();
		srand((unsigned int)time(NULL));
		int a = rand() % 4;
			
		if (a == 0)
		{
			
		}
		else if (a == 1)
		{
			
		}
		else if (a == 2)
		{
		}
		else if (a == 3)
		{
			
		}

			

	}

	SetPos(m_vPos);
}

void GBoss::Render(HDC _dc)
{
	Vec2 m_vPos(GetPos().x, GetPos().y);
	Vec2 m_vScale(GetScale().x, GetScale().y);


	Rectangle(CCore::GetInst()->GetmemDC(), m_vPos.x - m_vScale.x, m_vPos.y - m_vScale.y, m_vPos.x + m_vScale.x, m_vPos.y + m_vScale.y);

}

void GBoss::Move()
{
}
