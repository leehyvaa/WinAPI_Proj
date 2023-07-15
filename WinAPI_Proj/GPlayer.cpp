#include "pch.h"
#include "GPlayer.h"
#include "CCore.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "GMap.h"

GPlayer::GPlayer()
{
	//m_fSpeed = 300;

	m_OnDrawRail = false;
}

GPlayer::GPlayer(Vec2 _vPos, Vec2 _vScale)
{
	SetPos(_vPos);
	SetScale(_vScale);
	//m_fSpeed = 300;
	m_OnDrawRail = false;



}

GPlayer::~GPlayer()
{

}




void GPlayer::ResetRail()
{
		m_OnDrawRail = false;

		m_arrTempRail_X.clear();
		m_arrTempRail_Y.clear();

		/*for (int i = 0; i < m_arrTempRail_X.size(); i++)
			m_arrTempRail_X.pop_back();
		for (int i = 0; i < m_arrTempRail_Y.size(); i++)
			m_arrTempRail_Y.pop_back();*/

}

void GPlayer::DrawRail()
{
	for (int i = 0; i < m_arrTempRail_X.size(); i++)
	{
		GMap::GetInst()->m_arrMap[m_arrTempRail_Y[i]][m_arrTempRail_X[i]] = Rail;
	}
}

void GPlayer::Update()
{
	
	Vec2 m_vPos(GetPos().x, GetPos().y);

	int left = GMap::GetInst()->m_arrMap[(int)m_vPos.y][(int)m_vPos.x - 1];
	int right = GMap::GetInst()->m_arrMap[(int)m_vPos.y][(int)m_vPos.x + 1];
	int up = GMap::GetInst()->m_arrMap[(int)m_vPos.y-1][(int)m_vPos.x];
	int down = GMap::GetInst()->m_arrMap[(int)m_vPos.y+1][(int)m_vPos.x];
	int pos = GMap::GetInst()->m_arrMap[(int)m_vPos.y][(int)m_vPos.x];
	int myPosX = m_vPos.x;
	int myPosY = m_vPos.y;

	if(CKeyMgr::GetInst()->GetKeyState(KEY::Q) == KEY_STATE::TAP)
		cout << myPosX << " " << myPosY << " "<<pos << endl;
	if (CKeyMgr::GetInst()->GetKeyState(KEY::W) == KEY_STATE::TAP)
		GMap::GetInst()->DrawMap();
	if (CKeyMgr::GetInst()->GetKeyState(KEY::E) == KEY_STATE::TAP)
		system("cls");
	if (CKeyMgr::GetInst()->GetKeyState(KEY::V) == KEY_STATE::TAP)
	{
		for (int i = 0; i < m_arrTempRail_X.size(); i++)
			cout << m_arrTempRail_X[i] << endl;

		for (int i = 0; i < m_arrTempRail_Y.size(); i++)
			cout << m_arrTempRail_Y[i] << endl;
	}


	static int yCount = 0;
	static int xCount = 0;
	
	
	if (CKeyMgr::GetInst()->GetKeyState(KEY::SPACE) == KEY_STATE::AWAY)
	{
		ResetRail();
	}
	


	if (KEY_HOLD(KEY::SPACE))
	{
		m_OnDrawRail = true;


		if (CKeyMgr::GetInst()->GetKeyState(KEY::LEFT) == KEY_STATE::HOLD)
		{
			if (left == MyGround || left == Wall)
				return;

	
			m_vPos.x += -1;
			if (left == EnemyGround)
			{
				m_arrTempRail_X.push_back(m_vPos.x);
				m_arrTempRail_Y.push_back(m_vPos.y);

			}
			if (left == Rail)
			{

				DrawRail();
				ResetRail();
			}
		}
		if (CKeyMgr::GetInst()->GetKeyState(KEY::RIGHT) == KEY_STATE::HOLD)
		{
			if (right == MyGround || right == Wall)
				return;
			m_vPos.x += 1;
			if (right == EnemyGround)
			{
				m_arrTempRail_X.push_back(m_vPos.x);
				m_arrTempRail_Y.push_back(m_vPos.y);

			}
			if (right == Rail)
			{

				DrawRail();
				ResetRail();
			}
		}
		if (CKeyMgr::GetInst()->GetKeyState(KEY::UP) == KEY_STATE::HOLD)
		{
			if ( up == MyGround || up == Wall)
				return;
			m_vPos.y += -1;
			if (up == EnemyGround)
			{
				m_arrTempRail_X.push_back(m_vPos.x);
				m_arrTempRail_Y.push_back(m_vPos.y);

			}
			if (up == Rail)
			{

				DrawRail();
				ResetRail();
			}
		}
		if (CKeyMgr::GetInst()->GetKeyState(KEY::DOWN) == KEY_STATE::HOLD)
		{
			if ( down == MyGround || down == Wall)
				return;
			m_vPos.y += 1;
			if (down == EnemyGround)
			{
				m_arrTempRail_X.push_back(m_vPos.x);
				m_arrTempRail_Y.push_back(m_vPos.y);

			}
			if (down == Rail)
			{

				DrawRail();
				ResetRail();
			}
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




	SetPos(m_vPos);
}

void GPlayer::Render(HDC _dc)
{
	Vec2 m_vPos(GetPos().x, GetPos().y);
	Vec2 m_vScale(GetScale().x, GetScale().y);


	Rectangle(CCore::GetInst()->GetmemDC(), m_vPos.x - m_vScale.x, m_vPos.y - m_vScale.y, m_vPos.x + m_vScale.x, m_vPos.y + m_vScale.y);


	


}

//bool GPlayer::Collision(GameObject& vObj)
//{
//	Vec2 m_vPos(GetPos().x, GetPos().y);
//
//
//
//	if (GMap::GetInst()->m_arrMap[(int)m_vPos.y][(int)m_vPos.x + 1] == -1)
//	{
//		
//		return true;
//	}
//	if (GMap::GetInst()->m_arrMap[(int)m_vPos.y][(int)m_vPos.x - 1] == -1)
//	{
//
//		return true;
//	}
//
//	/*if (m_vPos.x - radius + moveSpeed * dirVector.Getx() < rectView.left)
//	{
//		dirVector.Setx(-dirVector.Getx());
//		return true;
//	}
//
//	if (position.y - radius + moveSpeed * dirVector.Gety() < rectView.top)
//	{
//		dirVector.Sety(-dirVector.Gety());
//
//		return true;
//	}
//
//	if (position.y + radius + moveSpeed * dirVector.Gety() > rectView.bottom)
//	{
//		dirVector.Sety(-dirVector.Gety());
//		return true;
//	}*/
//
//
//
//
//
//	Damaged();
//	return false;
//}



void GPlayer::Damaged()
{
	CCore::GetInst()->GameOver();
}



