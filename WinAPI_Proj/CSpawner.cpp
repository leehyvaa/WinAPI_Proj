#include "pch.h"
#include "CSpawner.h"
#include "CCore.h"
#include "CTimeMgr.h"
#include <iostream>
CSpawner::CSpawner()
{
	m_fSpeed = 10;
}

CSpawner::CSpawner(Vec2 _vPos, Vec2 _vScale)
{
	m_vPos.x = _vPos.x;
	m_vPos.y = _vPos.y;

	this->m_vScale = _vScale;
	m_fSpeed = 10;
	m_iScore = 0;
	
	

}

CSpawner::~CSpawner()
{
	for (int i = 0; i < m_vBullet.size(); i++)
	{
		m_vBullet[i] = nullptr;
		delete m_vBullet[i];
	}
}

void CSpawner::Update()
{

	m_vPos.x += m_vDir.x * m_fSpeed * fDT;
	m_fTimer += clock();
	
	if (m_fTimer >2.f)
	{
		m_fTimer = 0;
		InstEnemy();
	}


	for (int i = 0; i < m_vBullet.size(); i++)
	{
		m_vBullet[i]->Update();
	}
}

void CSpawner::Draw()
{
	Rectangle(CCore::GetInst()->GetmemDC(), m_vPos.x - m_vScale.x, m_vPos.y - m_vScale.y, m_vPos.x + m_vScale.x, m_vPos.y + m_vScale.y);
	for (int i = 0; i < m_vBullet.size(); i++)
	{
		m_vBullet[i]->Draw();
	}

}

bool CSpawner::Collision(GObject& vObj)
{
	if (m_vPos.x + m_vScale.x + m_fSpeed * m_vDir.x * fDT > CCore::GetInst() -> GetResolution().x)
	{
		m_vDir.x *= -1;
		return true;
	}


	if (m_vPos.x - m_vScale.x + m_fSpeed * m_vDir.x * fDT < 0)

	{
		m_vDir.x *= -1;
		return true;
	}

	return false;
}

void CSpawner::InstBullet()
{
	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(CCore::GetInst()->GetMainHwnd(), &ptMouse);

	Vec2 vDestPos = { (float)ptMouse.x - m_vPos.x,(float)ptMouse.y - m_vPos.y };
	Vec2 _Dir = vDestPos.normalize();

	m_vBullet.push_back(new CBullet(_Dir, m_vPos,30.f));
}

void CSpawner::InstEnemy()
{
	m_vEnemy.push_back(new CEnemy);

}
