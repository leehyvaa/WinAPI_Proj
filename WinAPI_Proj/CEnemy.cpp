#include "pch.h"
#include "CEnemy.h"
#include "CTimeMgr.h"
#include "CCore.h"
CEnemy::CEnemy()
{
	type = ENEMY;

}

CEnemy::CEnemy(Vec2 _vPos, Vec2 _vScale, float _speed,GObject& _owner)
{
	m_vPos = _vPos;
	m_vScale = _vScale;
	m_vDir = Vec2(0, 1);
	m_fSpeed = _speed;
	type = ENEMY;
	m_owner = &_owner;
}

CEnemy::~CEnemy()
{
}

void CEnemy::Update()
{
	for (int i = 0; i < m_owner->m_CollisionObj.size(); i++)
	{
		float left = m_owner->m_CollisionObj[i]->m_vPos.x - m_owner->m_CollisionObj[i]->m_vScale.x;
		float right = m_owner->m_CollisionObj[i]->m_vPos.x + m_owner->m_CollisionObj[i]->m_vScale.x;
		float top = m_owner->m_CollisionObj[i]->m_vPos.y - m_owner->m_CollisionObj[i]->m_vScale.y;

		
		if (m_vPos.x > left && m_vPos.x < right && m_vPos.y >top)
		{
			if (m_owner->m_CollisionObj[i]->type == WALL)
			{
				m_owner->m_CollisionObj[i]->Collision(*this);
				Collision(*m_owner->m_CollisionObj[i]);
			}
			else if (m_owner->m_CollisionObj[i]->type == COMMAND)
			{
				m_owner->m_CollisionObj[i]->Collision(*this);
				Collision(*m_owner->m_CollisionObj[i]);
			}
			else if (m_owner->m_CollisionObj[i]->type == PLAYER)
			{
				m_owner->m_CollisionObj[i]->Collision(*this);
		
			}
		}

		
	}
	m_vPos.x = m_vPos.x + m_fSpeed * m_vDir.x * fDT;
	m_vPos.y = m_vPos.y + m_fSpeed * m_vDir.y * fDT;
}

void CEnemy::Render(HDC _dc)
{
	Rectangle(CCore::GetInst()->GetmemDC(), m_vPos.x - m_vScale.x, m_vPos.y - m_vScale.y, m_vPos.x + m_vScale.x, m_vPos.y + m_vScale.y);

}

bool CEnemy::Collision(GObject& vObj)
{
	m_vPos.x = 0;
	m_vPos.y = 0;

	m_vScale.x = 0;
	m_vScale.y = 0;

	return false;
}



float CEnemy::LengthPts(Vec2 pt1, Vec2 pt2)
{

	return (sqrt(
		((float)(pt2.x - pt1.x) * (pt2.x - pt1.x)) +
		((float)(pt2.y - pt1.y) * (pt2.y - pt1.y))
	));
}