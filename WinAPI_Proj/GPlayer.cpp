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
	clockWise = true;
	enterDir = DEFAULT;
	objectType = Player;
	inBoss = false;
}

GPlayer::GPlayer(Vec2 _vPos, Vec2 _vScale)
{
	SetPos(_vPos);
	SetScale(_vScale);
	//m_fSpeed = 300;
	m_OnDrawRail = false;
	clockWise = true;
	enterDir = DEFAULT;
	inBoss = false;


}

GPlayer::~GPlayer()
{

}




void GPlayer::ResetRail()
{
	m_OnDrawRail = false;
	m_arrMovePoint.clear();

}



void GPlayer::Update()
{
	
	Vec2 m_vPos(GetPos().x, GetPos().y);
	POINT left = { m_vPos.x - 1,m_vPos.y };
	POINT right = { m_vPos.x + 1,m_vPos.y };
	POINT up = { m_vPos.x,m_vPos.y-1};
	POINT down = { m_vPos.x ,m_vPos.y+1 };
	POINT pos = { m_vPos.x,m_vPos.y };

	

	if(KEY_TAP(KEY::Q))
		cout << endl;
	if (KEY_TAP(KEY::W))
		//GMap::GetInst()->DrawMap();
	if (KEY_TAP(KEY::E))
		system("cls");
	if (KEY_TAP(KEY::V))



	if (CKeyMgr::GetInst()->GetKeyState(KEY::SPACE) == KEY_STATE::AWAY)
	{
		ResetRail();
	}
	
	

	if (KEY_HOLD(KEY::SPACE))
	{
		

		if (KEY_HOLD(KEY::LEFT))
		{
			if (InBox(GMap::GetInst()->m_arrMap, 4, pos))
				m_vPos.x--;
		}
		if (KEY_HOLD(KEY::RIGHT))
		{
			if (InBox(GMap::GetInst()->m_arrMap, 4, pos))
				m_vPos.x++;
		}
		if (KEY_HOLD(KEY::UP))
		{
			if (InBox(GMap::GetInst()->m_arrMap, 4, pos))
				m_vPos.y--;
		}
		if (KEY_HOLD(KEY::DOWN))
		{
			if (InBox(GMap::GetInst()->m_arrMap, 4, pos))
				m_vPos.y++;
		}
	}
	else
	{
		if (KEY_HOLD(KEY::LEFT))
		{
			if (InLine(GMap::GetInst()->m_playerRail, left))
				m_vPos.x--;
		}
		if (KEY_HOLD(KEY::RIGHT))
		{
			if (InLine(GMap::GetInst()->m_playerRail, right))
				m_vPos.x++;
		}
		if (KEY_HOLD(KEY::UP))
		{
			if (InLine(GMap::GetInst()->m_playerRail, up))
				m_vPos.y--;
		}
		if (KEY_HOLD(KEY::DOWN))
		{
			if (InLine(GMap::GetInst()->m_playerRail, down))
				m_vPos.y++;
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



bool GPlayer::InLine(list<MovePoint>& _line, POINT _point)
{
	for (int i = 0; i < _line.size(); i++)
	{

	}
	return true;
}

bool GPlayer::InBox(const POINT* _box, int _count, POINT _point)
{
	return true;

}