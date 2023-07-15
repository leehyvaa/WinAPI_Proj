#include"pch.h"
#include "Player.h"
#include "CCore.h"
#include "CTimeMgr.h"
#include "CSpawner.h"
#include "CKeyMgr.h"
Player::Player()
	:m_spawner(0)
{

	m_fSpeed = 300;
	//type = PLAYER;
	m_fStartTimer = 0;
}

Player::Player(Vec2 _vPos, Vec2 _vScale)
	:m_spawner(0)
{
	Vec2 m_vPos(GetPos().x, GetPos().y);
	m_vPos.x = _vPos.x;
	m_vPos.y = _vPos.y;

    this->SetScale(_vScale);
    
	//type = PLAYER;
	m_fStartTimer = 0;
	
	SetPos(m_vPos);

}

Player::~Player()
{
	for (int i = 0; i < m_vBullet.size(); i++)
	{
		m_vBullet[i] = nullptr;
		delete m_vBullet[i];
	}
}

void Player::Update()
{
	Vec2 m_vPos(GetPos().x, GetPos().y);
	if (CKeyMgr::GetInst()->GetKeyState(KEY::A) == KEY_STATE::HOLD)
	{

		m_vPos.x += -m_fSpeed*fDT;
	}
	if (CKeyMgr::GetInst()->GetKeyState(KEY::D) == KEY_STATE::HOLD)
	{
		m_vPos.x += m_fSpeed * fDT;
	}
	/*if (CKeyMgr::GetInst()->GetKeyState(KEY::W) == KEY_STATE::HOLD)
	{
		m_vPos.y += -m_fSpeed * fDT;
	}
	if (CKeyMgr::GetInst()->GetKeyState(KEY::S) == KEY_STATE::HOLD)
	{
		m_vPos.y += m_fSpeed * fDT;

	}*/


	float m_fCountTimer = clock();

	if (m_fCountTimer - m_fStartTimer > 300.f)
	{
		

		if (CKeyMgr::GetInst()->GetKeyState(KEY::LBUTTON) == KEY_STATE::TAP)
		{
			InstBullet();
			m_fStartTimer = clock();
		}
	}

	
	for (int i = 0; i < m_vBullet.size(); i++)
	{
		
		m_vBullet[i]->Update();
	}
	SetPos(m_vPos);
}

void Player::Render(HDC _dc)
{
	Vec2 m_vPos(GetPos().x, GetPos().y);
	Vec2 m_vScale(GetScale().x, GetScale().y);

	Rectangle(CCore::GetInst()->GetmemDC(), m_vPos.x - m_vScale.x, m_vPos.y - m_vScale.y, m_vPos.x + m_vScale.x, m_vPos.y + m_vScale.y);
	for (int i = 0; i < m_vBullet.size(); i++)
	{
		m_vBullet[i]->Render(_dc);
	}

}



void Player::InstBullet()
{
	/*POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(CCore::GetInst()->GetMainHwnd(), &ptMouse);

	Vec2 vDestPos = { (float)ptMouse.x-m_vPos.x,(float)ptMouse.y-m_vPos.y};
	Vec2 _Dir = vDestPos.normalize();

	m_vBullet.push_back(new CBullet(_Dir,m_vPos,2000.f,*this,P_BULLET));
	*/
}

void Player::Damaged()
{
	CCore::GetInst()->GameOver();
}

