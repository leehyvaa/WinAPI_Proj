#include "pch.h"
#include "CSpawner.h"
#include "CCore.h"
#include "CTimeMgr.h"
#include <iostream>
#include "Player.h"

CSpawner::CSpawner()
//	:m_player(0)
{
	m_fSpeed = 10;

	m_fSpeed = 100;
	m_iScore = 0;
	m_vDir.x = 1;
	type = SPAWNER;
	m_fStartTimer = 0;

}

CSpawner::CSpawner(Vec2 _vPos, Vec2 _vScale)
//	:m_player(0)
{
	m_vPos.x = _vPos.x;
	m_vPos.y = _vPos.y;

	this->m_vScale = _vScale;
	m_fSpeed = 100;
	m_iScore = 0;
	m_vDir.x = 1;
	type = SPAWNER;
	m_fStartTimer = 0;

}

CSpawner::~CSpawner()
{
	/*for (int i = 0; i < m_vBullet.size(); i++)
	{
		m_vBullet[i] = nullptr;
		delete m_vBullet[i];
	}*/
}

void CSpawner::Update()
{

	m_vPos.x += m_vDir.x * m_fSpeed * fDT;


	float m_fCountTimer = clock();
	
	if (m_fCountTimer-m_fStartTimer >2000.f)
	{
		m_fStartTimer = clock();
	
		InstEnemy();
	}


	for (int i = 0; i < m_vEnemy.size(); i++)
	{
		m_vEnemy[i]->Update();
	}

	/*for (int i = 0; i < m_vBullet.size(); i++)
	{
		m_vBullet[i]->Update();
	}*/
}

void CSpawner::Draw()
{
	Rectangle(CCore::GetInst()->GetmemDC(), m_vPos.x - m_vScale.x, m_vPos.y - m_vScale.y, m_vPos.x + m_vScale.x, m_vPos.y + m_vScale.y);
	
	for (int i = 0; i < m_vEnemy.size(); i++)
	{
		m_vEnemy[i]->Draw();
	}
	/*for (int i = 0; i < m_vBullet.size(); i++)
	{
		m_vBullet[i]->Draw();
	}*/


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

	if (vObj.type == P_BULLET)
	{
		m_iScore += 10;
		m_vScale.y += 0.1;
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

	//m_vBullet.push_back(new CBullet(_Dir, m_vPos,30.f));
}

void CSpawner::InstEnemy()
{
	CEnemy* enemy = new CEnemy(m_vPos, Vec2(20, 20), 30);
	m_vEnemy.push_back(enemy);
	m_player->m_CollisionObj.push_back(enemy);
}

