#include "pch.h"
#include "GPlayer.h"
#include "CCore.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "GMap.h"

GPlayer::GPlayer()
{
	m_fSpeed = 300;
	type = PLAYER;

}

GPlayer::GPlayer(Vec2 _vPos, Vec2 _vScale)
{
	m_vPos.x = _vPos.x;
	m_vPos.y = _vPos.y;

	this->m_vScale = _vScale;
	m_fSpeed = 300;
	type = PLAYER;



}

GPlayer::~GPlayer()
{

}

void GPlayer::Update()
{
	int left = GMap::GetInst()->m_arrMap[(int)m_vPos.y][(int)m_vPos.x - 1];
	int right = GMap::GetInst()->m_arrMap[(int)m_vPos.y][(int)m_vPos.x + 1];
	int up = GMap::GetInst()->m_arrMap[(int)m_vPos.y][(int)m_vPos.y - 1];
	int down = GMap::GetInst()->m_arrMap[(int)m_vPos.y][(int)m_vPos.y + 1];
	int myPosX = m_vPos.x;
	int myPosY = m_vPos.y;
	cout << myPosX << " " << myPosY << " "<<left << endl;
	if (CKeyMgr::GetInst()->GetKeyState(KEY::SPACE) == KEY_STATE::HOLD)
	{
		if (CKeyMgr::GetInst()->GetKeyState(KEY::LEFT) == KEY_STATE::HOLD)
		{
			if (left == MyGround || left == Wall)
				return;
			m_vPos.x += -1;
		}
		if (CKeyMgr::GetInst()->GetKeyState(KEY::RIGHT) == KEY_STATE::HOLD)
		{
			if (right == MyGround || right == Wall)
				return;
			m_vPos.x += 1;
		}
		if (CKeyMgr::GetInst()->GetKeyState(KEY::UP) == KEY_STATE::HOLD)
		{
			if ( up == MyGround || up == Wall)
				return;
			m_vPos.y += -1;
		}
		if (CKeyMgr::GetInst()->GetKeyState(KEY::DOWN) == KEY_STATE::HOLD)
		{
			if ( down == MyGround || down == Wall)
				return;
			m_vPos.y += 1;
		}
	}
	else
	{
		if (CKeyMgr::GetInst()->GetKeyState(KEY::LEFT) == KEY_STATE::HOLD)
		{
			if (left == EnemyGround || left == MyGround || left == Wall)
				return;
			m_vPos.x += -1;
		}
		if (CKeyMgr::GetInst()->GetKeyState(KEY::RIGHT) == KEY_STATE::HOLD)
		{
			if (right == EnemyGround || right == MyGround || right == Wall)
				return;
			m_vPos.x += 1;
		}
		if (CKeyMgr::GetInst()->GetKeyState(KEY::UP) == KEY_STATE::HOLD)
		{
			if (up == EnemyGround || up == MyGround || up == Wall)
				return;
			m_vPos.y += -1;
		}
		if (CKeyMgr::GetInst()->GetKeyState(KEY::DOWN) == KEY_STATE::HOLD)
		{
			if (down == EnemyGround || down == MyGround || down == Wall)
				return;
			m_vPos.y += 1;
		}
	}





}

void GPlayer::Draw()
{
	Rectangle(CCore::GetInst()->GetmemDC(), m_vPos.x - m_vScale.x, m_vPos.y - m_vScale.y, m_vPos.x + m_vScale.x, m_vPos.y + m_vScale.y);

}

bool GPlayer::Collision(GObject& vObj)
{
	


	if (GMap::GetInst()->m_arrMap[(int)m_vPos.y][(int)m_vPos.x + 1] == -1)
	{
		
		return true;
	}
	if (GMap::GetInst()->m_arrMap[(int)m_vPos.y][(int)m_vPos.x - 1] == -1)
	{

		return true;
	}

	/*if (m_vPos.x - radius + moveSpeed * dirVector.Getx() < rectView.left)
	{
		dirVector.Setx(-dirVector.Getx());
		return true;
	}

	if (position.y - radius + moveSpeed * dirVector.Gety() < rectView.top)
	{
		dirVector.Sety(-dirVector.Gety());

		return true;
	}

	if (position.y + radius + moveSpeed * dirVector.Gety() > rectView.bottom)
	{
		dirVector.Sety(-dirVector.Gety());
		return true;
	}*/





	Damaged();
	return false;
}



void GPlayer::Damaged()
{
	CCore::GetInst()->GameOver();
}

