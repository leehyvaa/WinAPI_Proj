#include "pch.h"
#include "GPlayer.h"
#include "CCore.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "GMap.h"

#include "CResMgr.h"
#include "CTexture.h"


GPlayer::GPlayer()
	:m_pTex(nullptr)
{
	//m_fSpeed = 300;
	//Texture 로딩
	m_pTex = CResMgr::GetInst()->LoadTexture(L"PlayerTex",L"texture\\sigong.bmp");






	m_OnDrawRail = false;
	clockWise = true;
	updateDir = DEFAULT;
	objectType = Player;
	inBoss = false;
}

GPlayer::GPlayer(Vec2 _vPos, Vec2 _vScale)
{
	m_pTex = CResMgr::GetInst()->LoadTexture(L"PlayerTex", L"texture\\sigong.bmp");



	SetPos(_vPos);
	SetScale(_vScale);
	//m_fSpeed = 300;
	m_OnDrawRail = false;
	clockWise = true;
	updateDir = DEFAULT;
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

void GPlayer::DrawCheck(POINT _nextP,POINT _crntP,PlayerDir _dir)
{
	bool inLine = InLine(GMap::GetInst()->m_playerRail, _nextP);

	if (m_OnDrawRail && inLine)
	{
		//도착점 추가해야함
		DrawRail();
	}

	DrawTempRail(_nextP, _dir);

	if (!m_OnDrawRail && !inLine)
	{
		m_OnDrawRail = true;
		//시작점 추가해야함
	}
}

void GPlayer::DrawTempRail(POINT _p, PlayerDir _dir)
{
	if (updateDir != _dir)
	{
		updateDir = _dir;
		//점 추가
	}
	

}


void GPlayer::DrawRail()
{


	ResetRail();
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
	
	if (m_OnDrawRail)
	{

	}



	if (KEY_HOLD(KEY::SPACE))
	{
		if (KEY_HOLD(KEY::LEFT))
		{
			if (InBox(GMap::GetInst()->m_playerRail, left))
			{
				m_vPos.x--;

				if (updateDir == DEFAULT && m_OnDrawRail)
					updateDir = LEFT;

				DrawCheck(left,pos,LEFT);




					
			}
		}
		else if (KEY_HOLD(KEY::RIGHT))
		{
			if (InBox(GMap::GetInst()->m_playerRail, right))
			{
				m_vPos.x++;

				if (updateDir == DEFAULT && m_OnDrawRail)
					updateDir = RIGHT;

				DrawCheck(right,pos,RIGHT);


				
			}
		}
		else if (KEY_HOLD(KEY::UP))
		{
			if (InBox(GMap::GetInst()->m_playerRail, up))
			{
				m_vPos.y--;

				if (updateDir == DEFAULT && m_OnDrawRail)
					updateDir = UP;

				DrawCheck(up,pos,UP);

			}
		}
		else if (KEY_HOLD(KEY::DOWN))
		{
			if (InBox(GMap::GetInst()->m_playerRail, down))
			{
				m_vPos.y++;

				if (updateDir == DEFAULT && m_OnDrawRail)
					updateDir = DOWN;

				DrawCheck(down,pos,DOWN);


			}
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
	Vec2 vPos(GetPos().x, GetPos().y);
	Vec2 vScale(GetScale().x, GetScale().y);


	//Rectangle(CCore::GetInst()->GetmemDC(), m_vPos.x - m_vScale.x, m_vPos.y - m_vScale.y, m_vPos.x + m_vScale.x, m_vPos.y + m_vScale.y);
	
	int iWidth = (int)m_pTex->Width();
	int iHeight = (int) m_pTex->Height();



	//BitBlt(_dc
	//	, int(vPos.x - (float)(iWidth / 2))
	//	, int(vPos.y - (float)(iHeight / 2))
	//	, iWidth, iHeight
	//	, m_pTex->GetDC()
	//	, 0, 0, SRCCOPY);
	//	
	TransparentBlt(_dc
		, int(vPos.x - (float)(iWidth / 2))
		, int(vPos.y - (float)(iHeight / 2))
		, iWidth, iHeight
		, m_pTex->GetDC()
		, 0, 0, iWidth,iHeight,RGB(255,0,255));

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
	list<MovePoint>::iterator iter = _line.begin();
	
	for (iter=_line.begin(); iter!=_line.end(); iter++)
	{
		
		list<MovePoint>::iterator nextIt = next(iter);

		if (iter == prev(_line.end()))
			nextIt = _line.begin();
			
			
		
		Vec2 v = { iter->x,iter->y };
		Vec2 v2 = { nextIt->x,nextIt->y };
		float ans = v.Cross(v2 , _point);
		
		if (ans == 0)
		{
			if (v.x == v2.x)
			{
				if (v.y > v2.y)
				{
					if (v.y >= _point.y && v2.y <= _point.y)
						return true;

				}
				else
				{
					if (v.y <= _point.y && v2.y >= _point.y)
						return true;

				}
			}
			else
			{
				if (v.x > v2.x)
				{
					if (v.x >= _point.x && v2.x <= _point.x)
						return true;
				}
				else
				{
					if (v.x <= _point.x && v2.x >= _point.x)
						return true;

				}
			
			}

				
		}


	}
	return false;
}

bool GPlayer::InBox(list<MovePoint>& _line, POINT _p)
{
	int topCount = 0;
	int leftCount = 0;
	int rightCount = 0;
	int botCount = 0;

	POINT top = { _p.x, _p.y - 1200 };
	POINT left = { _p.x-1200, _p.y};
	POINT right = { _p.x+1200, _p.y};
	POINT bot = { _p.x, _p.y + 1200 };

	
	list<MovePoint>::iterator iter = _line.begin();

	for (iter = _line.begin(); iter != _line.end(); iter++)
	{

		list<MovePoint>::iterator nextIt = next(iter);

		if (iter == prev(_line.end()))
			nextIt = _line.begin();



		Vec2 v = { iter->x,iter->y };
		Vec2 v2 = { nextIt->x,nextIt->y };
		
		topCount += CollisionCount(v, v2, _p, top);
		leftCount += CollisionCount(v, v2, _p, left);
		rightCount += CollisionCount(v, v2, _p, right);
		botCount += CollisionCount(v, v2, _p, bot);



	}


	if (topCount % 2 == 1 || botCount %2 == 1)
		return true;
	else
		return false;
}

int GPlayer::CollisionCount(Vec2 v,Vec2 v2,POINT _p, POINT _rayP)
{
	float a, b, c;
	float a1, b1, c1;


	a = v2.y - v.y;
	b = v.x - v2.x;
	c = v.x * (v2.y - v.y) - v.y * (v2.x - v.x);


	a1 = _rayP.y - _p.y;
	b1 = _p.x - _rayP.x;
	c1 = _p.x * (_rayP.y - _p.y) - _p.y * (_rayP.x - _p.x);

	double del = a * b1 - a1 * b;


	double delX = c * b1 - c1 * b;
	double delY = a * c1 - c * a1;

	double x = delX / del;
	double y = delY / del;
	

	if (abs(del) != 0)
	{
		if (_p.y < _rayP.y)
		{
			if (y <= _rayP.y && y >= _p.y)
			{
				if (v.x <= v2.x)
				{
					if (x >= v.x && x <= v2.x)
						return 1;
				}
				else
				{
					if (x <= v.x && x >= v2.x)
						return 1;
				}
			}
				
		}
		else
		{
			if (y >= _rayP.y && y <= _p.y)
			{
				if (v.x <= v2.x)
				{
					if (x >= v.x && x <= v2.x)
						return 1;
				}
				else
				{
					if (x <= v.x && x >= v2.x)
						return 1;
				}
			}
		}
		
	

	}


	return 0;


}