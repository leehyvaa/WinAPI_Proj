#include "pch.h"
#include "CSpawner.h"
#include "CCore.h"
#include "CTimeMgr.h"
#include <iostream>
#include "Player.h"

CSpawner::CSpawner()
	:m_player(0)
{
	m_fSpeed = 10;

	m_fSpeed = 100;
	m_iScore = 0;
	m_vDir.x = 1;
	type = SPAWNER;
	m_fStartTimer = 0;
    m_fSpawnTimer=1000.f;
	m_iSpawnCount=0;
	m_fDifficult=0;
	m_bDamaged = false;
	m_fStartDmgTimer = 0;
	m_fDamagedCount = 0;
}

CSpawner::CSpawner(Vec2 _vPos, Vec2 _vScale)
	:m_player(0)
{
	m_vPos.x = _vPos.x;
	m_vPos.y = _vPos.y;

	this->m_vScale = _vScale;
	m_fSpeed = 100;
	m_iScore = 0;
	m_vDir.x = 1;
	type = SPAWNER;
	m_fStartTimer = 0;
	m_fSpawnTimer = 1000.f;
	m_iSpawnCount=0;
	m_fDifficult=0;
	m_bDamaged = false;
	m_fStartDmgTimer = 0;
	m_fDamagedCount = 0;
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
	float m_fDamagedCount = clock();


	if (m_fCountTimer-m_fStartTimer >m_fSpawnTimer-m_fDifficult)
	{
		m_fStartTimer = clock();
		m_iSpawnCount++;
		if (m_iSpawnCount % 5 == 0)
		{
			InstBullet();
			m_fDifficult += 20;
		}
		InstEnemy();
	}

	if (m_bDamaged && (m_fDamagedCount - m_fStartDmgTimer > 800.f))
	{
		m_bDamaged = false;
	}


	for (int i = 0; i < m_vEnemy.size(); i++)
	{
		m_vEnemy[i]->Update();
	}

	for (int i = 0; i < m_vBullet.size(); i++)
	{
		m_vBullet[i]->Update();
	}
}

void CSpawner::Draw()
{

	static float landX = 5;

	HBRUSH hBrush, oldBrush;

	

	if (m_bDamaged)
	{
		hBrush = CreateSolidBrush(RGB(255, 0, 0));
		oldBrush = (HBRUSH)SelectObject(CCore::GetInst()->GetmemDC(), hBrush);

		Rectangle(CCore::GetInst()->GetmemDC(), m_vPos.x - m_vScale.x+landX, m_vPos.y - m_vScale.y, m_vPos.x + m_vScale.x+landX, m_vPos.y + m_vScale.y);
		SelectObject(CCore::GetInst()->GetmemDC(), oldBrush);
		DeleteObject(hBrush);
		landX *= -1;
	}
	else
	{
		hBrush = CreateSolidBrush(RGB(0, 0, 0));
		oldBrush = (HBRUSH)SelectObject(CCore::GetInst()->GetmemDC(), hBrush);

		Rectangle(CCore::GetInst()->GetmemDC(), m_vPos.x - m_vScale.x, m_vPos.y - m_vScale.y, m_vPos.x + m_vScale.x, m_vPos.y + m_vScale.y);
		SelectObject(CCore::GetInst()->GetmemDC(), oldBrush);
		DeleteObject(hBrush);
	}
	
	for (int i = 0; i < m_vEnemy.size(); i++)
	{
		m_vEnemy[i]->Draw();
	}
	
	TCHAR buf2[1024];
	wsprintf(buf2, TEXT("%dÁ¡"), m_iScore);

	TextOut(CCore::GetInst()->GetmemDC(), 0, 0, buf2, _tcslen(buf2));
	
	

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

	if (vObj.type == P_BULLET)
	{
		m_iScore += 10;
		m_bDamaged = true;
		m_fStartDmgTimer = clock();
	}

	return false;
}

void CSpawner::InstBullet()
{

	for (int i = 0; i < 8; i++)
	{
		float x = i * 120 - m_vPos.x;
		float y = 1000 - m_vPos.y;


		Vec2 vDestPos{x,y};
		Vec2 _Dir = vDestPos.normalize();

		m_vBullet.push_back(new CBullet(_Dir, m_vPos, 250.f, *this, E_BULLET));
	}
	
}

void CSpawner::InstEnemy()
{
	CEnemy* enemy = new CEnemy(m_vPos, Vec2(20, 20), 500,*this);
	m_vEnemy.push_back(enemy);
	m_player->m_CollisionObj.push_back(enemy);
}

