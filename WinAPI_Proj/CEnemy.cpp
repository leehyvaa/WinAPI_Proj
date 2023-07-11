#include "pch.h"
#include "CEnemy.h"
#include "CTimeMgr.h"
#include "CCore.h"
CEnemy::CEnemy()
{
}

CEnemy::CEnemy(Vec2 _vPos, Vec2 _vScale, float _speed)
{
	m_vPos = _vPos;
	m_vScale = _vScale;
	m_vDir = Vec2(0, 1);
	m_fSpeed = _speed;
	type = ENEMY;
}

CEnemy::~CEnemy()
{

}

void CEnemy::Update()
{
	m_vPos.x = m_vPos.x + m_fSpeed * m_vDir.x * fDT;
	m_vPos.y = m_vPos.y + m_fSpeed * m_vDir.y * fDT;
}

void CEnemy::Draw()
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

void CEnemy::Damaged(float _damage)
{
}
