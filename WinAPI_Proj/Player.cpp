#include"pch.h"
#include "Player.h"
#include "CCore.h"
#include "CTimeMgr.h"
#include "CSpawner.h"

Player::Player()
	:m_spawner(0)
{
	m_fSpeed = 100;
	type = PLAYER;
}

Player::Player(Vec2 _vPos, Vec2 _vScale)
	:m_spawner(0)
{
	m_vPos.x = _vPos.x;
	m_vPos.y = _vPos.y;

    this->m_vScale = _vScale;
    m_fSpeed = 100;
	type = PLAYER;



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
	if ((GetAsyncKeyState('A') & 0x8000))
	{

		m_vPos.x += -m_fSpeed*fDT;
	}
	if ((GetAsyncKeyState('D') & 0x8000))
	{
		m_vPos.x += m_fSpeed * fDT;
	}
	if ((GetAsyncKeyState('W') & 0x8000))
	{
		m_vPos.y += -m_fSpeed * fDT;
	}
	if ((GetAsyncKeyState('S') & 0x8000))
	{
		m_vPos.y += m_fSpeed * fDT;

	}

	if ((GetAsyncKeyState(VK_LBUTTON) * 0x8000))
	{
		InstBullet();
	}
	for (int i = 0; i < m_vBullet.size(); i++)
	{
		
		m_vBullet[i]->Update();
	}

}

void Player::Draw()
{
	Rectangle(CCore::GetInst()->GetmemDC(), m_vPos.x - m_vScale.x, m_vPos.y - m_vScale.y, m_vPos.x + m_vScale.x, m_vPos.y + m_vScale.y);
	for (int i = 0; i < m_vBullet.size(); i++)
	{
		m_vBullet[i]->Draw();
	}

}

bool Player::Collision(GObject& vObj)
{
	Damaged();
    return false;
}

void Player::InstBullet()
{
	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(CCore::GetInst()->GetMainHwnd(), &ptMouse);

	Vec2 vDestPos = { (float)ptMouse.x-m_vPos.x,(float)ptMouse.y-m_vPos.y};
	Vec2 _Dir = vDestPos.normalize();

	m_vBullet.push_back(new CBullet(_Dir,m_vPos,100.f,*this,P_BULLET));
	
}

void Player::Damaged()
{
	m_vScale.x += 50;
}

