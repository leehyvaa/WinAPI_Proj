#include "pch.h"
#include "CBullet.h"
#include "CCore.h"
#include "CTimeMgr.h"
CBullet::CBullet()
{
}

CBullet::CBullet(Vec2 _vDir, Vec2 _vPos,  float _speed,GObject& _owner, int _type)
{
	
	m_vPos.x = _vPos.x;
	m_vPos.y = _vPos.y;

	this->m_vScale = Vec2{ 10,10 };
	m_fSpeed = _speed;

	m_vDir.x = _vDir.x;
	m_vDir.y = _vDir.y;
	
	type = _type;
	m_owner = &_owner;


}

CBullet::~CBullet()
{
}

void CBullet::Update()
{

	for (int i = 0; i < m_owner->m_CollisionObj.size(); i++)
	{
		if (m_owner->type == PLAYER)
		{
			if (m_vScale.x + m_owner->m_CollisionObj[i]->m_vScale.x > LengthPts(m_vPos, m_owner->m_CollisionObj[i]->m_vPos))
			{
				Collision(*m_owner->m_CollisionObj[i]);
				
			}
		}
		else if (m_owner->type == SPAWNER)
		{
			if (m_owner->m_CollisionObj[i]->type == PLAYER)
			{
				if (m_vScale.x + m_owner->m_CollisionObj[i]->m_vScale.x > LengthPts(m_vPos, m_owner->m_CollisionObj[i]->m_vPos))
				{

					Collision(*m_owner->m_CollisionObj[i]);

				}
			}
			
		}

		


	}

	

	

	m_vPos.x = m_vPos.x + m_fSpeed * m_vDir.x *fDT;
	m_vPos.y = m_vPos.y + m_fSpeed * m_vDir.y *fDT;

	

}

void CBullet::Draw()
{
	Ellipse(CCore::GetInst()->GetmemDC(), m_vPos.x - m_vScale.x, m_vPos.y - m_vScale.y, m_vPos.x + m_vScale.x, m_vPos.y + m_vScale.y);
}

bool CBullet::Collision(GObject& vObj)
{
	if (m_owner != &vObj)
	{
		if (vObj.type == PLAYER)
		{
			vObj.Collision(*this);
		}

		if (vObj.type == SPAWNER)
		{
			vObj.Collision(*this);
		}

		if (vObj.type == ENEMY)
		{
			vObj.Collision(*this);
		}
	}


	m_vPos.x = 0;
	m_vPos.y = 0;

	m_vScale.x = 0;
	m_vScale.y = 0;

	return false;
}


float CBullet::LengthPts(Vec2 pt1, Vec2 pt2)
{
	return (sqrt(
		((float)(pt2.x - pt1.x) * (pt2.x - pt1.x)) +
		((float)(pt2.y - pt1.y) * (pt2.y - pt1.y))
	));
}