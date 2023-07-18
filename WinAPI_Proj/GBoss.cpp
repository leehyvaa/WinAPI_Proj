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

	
	int left = GMap::GetInst()->m_arrMap[(int)m_vPos.y][(int)m_vPos.x - 1];
	int right = GMap::GetInst()->m_arrMap[(int)m_vPos.y][(int)m_vPos.x + 1];
	int up = GMap::GetInst()->m_arrMap[(int)m_vPos.y - 1][(int)m_vPos.x];
	int down = GMap::GetInst()->m_arrMap[(int)m_vPos.y + 1][(int)m_vPos.x];


	if (m_fCountTimer- m_fStartTimer   > 2.f)
	{
		m_fStartTimer = clock();
		srand((unsigned int)time(NULL));
		int a = rand() % 4;
			
		if (a == 0)
		{
			if(right == EnemyGround)
				m_vPos.x++;
		}
		else if (a == 1)
		{
			if (left == EnemyGround)
				m_vPos.x--;
		}
		else if (a == 2)
		{
			if (down == EnemyGround)
				m_vPos.y++;
		}
		else if (a == 3)
		{
			if (up == EnemyGround)
				m_vPos.y--;
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
