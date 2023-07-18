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

void GPlayer::CalculateDir(PlayerDir enterDir, PlayerDir endDir)
{
	if (abs(enterDir - endDir) == 1 || abs(enterDir - endDir) == 3)
	{
		if (enterDir - endDir == 3 || enterDir - endDir == -1)
			clockWise = true;
		else
			clockWise = false;

	}
	else if (abs(enterDir - endDir) == 2)
	{
		if (enterDir == UP || enterDir == DOWN)
			if (m_arrMovePoint[0].x - m_arrMovePoint[m_arrMovePoint.size() - 1].x > 0)
				clockWise = true;
			else
				clockWise = false;
		else
		{
			if (m_arrMovePoint[0].y - m_arrMovePoint[m_arrMovePoint.size() - 1].y > 0)
				clockWise = false;
			else
				clockWise = true;
		}
	}
	else if (abs(enterDir - endDir) == 0)
	{

	}
}



void GPlayer::DrawRail()
{
	m_OnDrawRail = false;
	for (int i = 0; i < m_arrMovePoint.size(); i++)
	{
		GMap::GetInst()->m_arrMap[m_arrMovePoint[i].y][m_arrMovePoint[i].x] = Rail;
		
	}



	inBoss = false;

	for (int i = 0; i < m_arrMovePoint.size(); i++)
	{
		

		
		if (m_arrMovePoint[i].dir == UP || m_arrMovePoint[i].dir == RIGHT || m_arrMovePoint[i].dir == DOWN)
			DrawEnemyZone((PlayerDir)(m_arrMovePoint[i].dir + 1), i,TempA,TempRail);
		else
			DrawEnemyZone(UP, i,TempA,TempRail);
		
		//보스 위치 판별
		for (int i = 0; i < m_target.size(); i++)
		{
			if (m_target[i]->objectType == Boss)
			{
				Vec2 _Pos = m_target[i]->GetPos();
				if (GMap::GetInst()->m_arrMap[(int)_Pos.y][(int)_Pos.x] == TempA)
				{
					inBoss = true;
				}
			
			}

		}
		/*
			if (m_arrMovePoint[i].dir == LEFT || m_arrMovePoint[i].dir == RIGHT || m_arrMovePoint[i].dir == DOWN)
				DrawEnemyZone((PlayerDir)(m_arrMovePoint[i].dir - 1), i);
			else
				DrawEnemyZone(LEFT, i);*/

	}


	if (inBoss)
	{
		for (int i = 0; i < m_arrMovePoint.size(); i++)
		{
			if (m_arrMovePoint[i].dir == UP || m_arrMovePoint[i].dir == RIGHT || m_arrMovePoint[i].dir == DOWN)
				DrawEnemyZone((PlayerDir)(m_arrMovePoint[i].dir + 1), i,MyGround,Rail);
			else
				DrawEnemyZone(UP, i,MyGround,Rail);
		}
	}
	else
	{
		//보스가 색칠한 곳 밖에 있을경우 원래 색칠했던 공간을 다시 에너미땅
		//으로 돌리고 반대편을 마이그라운드로 바로 색칠함 이때는 tempRail이
		//아닌 Rail로 
	}
}
void GPlayer::DrawEnemyZone(PlayerDir dir,int i,MapType _type, MapType _typeTwo)
{
	int j = 1;
	while (1)
	{
		if (dir == UP)
		{
			if (GMap::GetInst()->m_arrMap[m_arrMovePoint[i].y - j][m_arrMovePoint[i].x] == Rail)
				break;
			GMap::GetInst()->m_arrMap[m_arrMovePoint[i].y-j][m_arrMovePoint[i].x] = _type;
		}
		else if (dir == RIGHT)
		{
			if (GMap::GetInst()->m_arrMap[m_arrMovePoint[i].y][m_arrMovePoint[i].x+j] == Rail)
				break;
			GMap::GetInst()->m_arrMap[m_arrMovePoint[i].y][m_arrMovePoint[i].x + j] = _type;

		}
		else if (dir == DOWN)
		{
			if (GMap::GetInst()->m_arrMap[m_arrMovePoint[i].y +j][m_arrMovePoint[i].x] == Rail)
				break;
			GMap::GetInst()->m_arrMap[m_arrMovePoint[i].y+j][m_arrMovePoint[i].x] = _type;
		}
		else if (dir == LEFT)
		{
			if (GMap::GetInst()->m_arrMap[m_arrMovePoint[i].y][m_arrMovePoint[i].x -j] == Rail)
				break;
			GMap::GetInst()->m_arrMap[m_arrMovePoint[i].y][m_arrMovePoint[i].x-j] = _type;
		}

		j++;
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

	MovePoint mPoint;


	if(KEY_TAP(KEY::Q))
		cout << myPosX << " " << myPosY << " "<<pos << endl;
	if (KEY_TAP(KEY::W))
		GMap::GetInst()->DrawMap();
	if (KEY_TAP(KEY::E))
		system("cls");
	if (KEY_TAP(KEY::V))
	{
		for (int i = 0; i < m_arrTempRail_X.size(); i++)
			cout << m_arrTempRail_X[i] << endl;

		for (int i = 0; i < m_arrTempRail_Y.size(); i++)
			cout << m_arrTempRail_Y[i] << endl;
	}


	static int yCount = 0;
	static int xCount = 0;
	
	static PlayerDir endDir = DEFAULT;
	

	if (CKeyMgr::GetInst()->GetKeyState(KEY::SPACE) == KEY_STATE::AWAY)
	{
		ResetRail();
	}
	
	

	if (KEY_HOLD(KEY::SPACE))
	{
		

		if (KEY_HOLD(KEY::LEFT))
		{
			if (left == MyGround || left == Wall)
				return;
			if (!m_OnDrawRail)
			{
				m_OnDrawRail = true;
				enterDir = LEFT;
			}

	
			m_vPos.x += -1;
			if (left == EnemyGround)
			{
				mPoint = { (int)m_vPos.x,(int)m_vPos.y,LEFT };
				m_arrMovePoint.push_back(mPoint);

			}
			if (left == Rail)
			{
				endDir = LEFT;
				CalculateDir(enterDir, endDir);

				DrawRail();
				ResetRail();
			}
		}
		if (KEY_HOLD(KEY::RIGHT))
		{
			if (right == MyGround || right == Wall)
				return;
			if (!m_OnDrawRail)
			{
				m_OnDrawRail = true;
				enterDir = RIGHT;
			}

			m_vPos.x += 1;
			if (right == EnemyGround)
			{
				mPoint = { (int)m_vPos.x,(int)m_vPos.y,RIGHT };
				m_arrMovePoint.push_back(mPoint);

			}
			if (right == Rail)
			{
				CalculateDir(enterDir, endDir);

				DrawRail();
				ResetRail();
			}
		}
		if (KEY_HOLD(KEY::UP))
		{
			if ( up == MyGround || up == Wall)
				return;
			if (!m_OnDrawRail)
			{
				m_OnDrawRail = true;
				enterDir = UP;
			}

			m_vPos.y += -1;
			if (up == EnemyGround)
			{
				mPoint = { (int)m_vPos.x,(int)m_vPos.y,UP };
				m_arrMovePoint.push_back(mPoint);

			}
			if (up == Rail)
			{
				CalculateDir(enterDir, endDir);

				DrawRail();
				ResetRail();
			}
		}
		if (KEY_HOLD(KEY::DOWN))
		{
			if ( down == MyGround || down == Wall)
				return;
			if (!m_OnDrawRail)
			{
				m_OnDrawRail = true;
				enterDir = DOWN;
			}

			m_vPos.y += 1;
			if (down == EnemyGround)
			{
				mPoint = { (int)m_vPos.x,(int)m_vPos.y,DOWN };
				m_arrMovePoint.push_back(mPoint);

			}
			if (down == Rail)
			{
				CalculateDir(enterDir, endDir);

				DrawRail();
				ResetRail();
			}
		}
	}
	else
	{
		if (KEY_HOLD(KEY::LEFT))
		{
			if (left == EnemyGround || left == MyGround || left == Wall)
				return;
			m_vPos.x += -1;
		}
		if (KEY_HOLD(KEY::RIGHT))
		{
			if (right == EnemyGround || right == MyGround || right == Wall)
				return;
			m_vPos.x += 1;
		}
		if (KEY_HOLD(KEY::UP))
		{
			if (up == EnemyGround || up == MyGround || up == Wall)
				return;
			m_vPos.y += -1;
		}
		if (KEY_HOLD(KEY::DOWN))
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



