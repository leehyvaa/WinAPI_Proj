#include"pch.h"
#include "CBullet.h"
#include "CCore.h"
#include "CTimeMgr.h"
CBullet::CBullet()
{
	m_fSpeed = 100;
}

CBullet::CBullet(Vec2 _vDir, Vec2 _vPos,  float _speed)
{
	
	m_vPos.x = _vPos.x;
	m_vPos.y = _vPos.y;

	this->m_vScale = Vec2{ 10,10 };
	m_fSpeed = 400;

	m_vDir.x = _vDir.x;
	m_vDir.y = _vDir.y;
	
	//m_owner = _owner;


}

CBullet::~CBullet()
{
}

void CBullet::Update()
{
	
	m_vPos.x = m_vPos.x + m_fSpeed * m_vDir.x *fDT;
	m_vPos.y = m_vPos.y + m_fSpeed * m_vDir.y *fDT;

	

}

void CBullet::Draw()
{
	Ellipse(CCore::GetInst()->GetmemDC(), m_vPos.x - m_vScale.x, m_vPos.y - m_vScale.y, m_vPos.x + m_vScale.x, m_vPos.y + m_vScale.y);
}

bool CBullet::Collision(GObject& vObj)
{
	return false;
}
