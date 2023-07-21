#include "pch.h"
#include "GPlayer.h"
#include "CCore.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "GMap.h"
#include "CPathMgr.h"
#include "CTexture.h"


GPlayer::GPlayer()
	:m_pTex(nullptr)
{
	//m_fSpeed = 300;
	//Texture ·Îµù
	m_pTex = new CTexture;

	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += L"texture\\sigong.bmp";
	m_pTex->Load(strFilePath);




	m_OnDrawRail = false;
	clockWise = true;
	enterDir = DEFAULT;
	objectType = Player;
	inBoss = false;
}

GPlayer::GPlayer(Vec2 _vPos, Vec2 _vScale)
{
	m_pTex = new CTexture;

	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += L"texture\\sigong.bmp";
	m_pTex->Load(strFilePath);


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
	if (nullptr != m_pTex)
		delete m_pTex;
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
		

		//if (KEY_HOLD(KEY::LEFT))
		//{
		//	if (InBox(GMap::GetInst()->m_arrMap, 4, pos))
		//		m_vPos.x--;
		//}
		//if (KEY_HOLD(KEY::RIGHT))
		//{
		//	if (InBox(GMap::GetInst()->m_arrMap, 4, pos))
		//		m_vPos.x++;
		//}
		//if (KEY_HOLD(KEY::UP))
		//{
		//	if (InBox(GMap::GetInst()->m_arrMap, 4, pos))
		//		m_vPos.y--;
		//}
		//if (KEY_HOLD(KEY::DOWN))
		//{
		//	if (InBox(GMap::GetInst()->m_arrMap, 4, pos))
		//		m_vPos.y++;
		//}
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

bool GPlayer::InBox(const POINT* _box, int _count, POINT _p)
{
	/*POINT a;
	
	double A = 



	return true;*/
}