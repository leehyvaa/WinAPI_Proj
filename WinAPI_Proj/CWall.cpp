#include "pch.h"
#include "CWall.h"
#include "CCore.h"
CWall::CWall()
{
	m_fSpeed = 10;
	type = WALL;
	m_iHp = 3;

}

CWall::CWall(Vec2 _vPos, Vec2 _vScale)
{
	m_vPos.x = _vPos.x;
	m_vPos.y = _vPos.y;

	this->m_vScale = _vScale;
	m_fSpeed = 10;
	type = WALL;
	m_iHp = 3;


}

CWall::~CWall()
{

}

void CWall::Update()
{
	

}

void CWall::Draw()
{
	HBRUSH hBrush, oldBrush;

	switch (m_iHp)
	{
	case 1:
	{
		hBrush = CreateSolidBrush(RGB(255,0,0));
		//hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		oldBrush = (HBRUSH)SelectObject(CCore::GetInst()->GetmemDC(), hBrush);
		Rectangle(CCore::GetInst()->GetmemDC(), m_vPos.x - m_vScale.x, m_vPos.y - m_vScale.y, m_vPos.x + m_vScale.x, m_vPos.y + m_vScale.y);
		SelectObject(CCore::GetInst()->GetmemDC(), oldBrush);
		DeleteObject(hBrush);

	
	}break;
	case 2:
	{
		hBrush = CreateSolidBrush(RGB(255, 255, 0));
		//hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		oldBrush = (HBRUSH)SelectObject(CCore::GetInst()->GetmemDC(), hBrush);
		Rectangle(CCore::GetInst()->GetmemDC(), m_vPos.x - m_vScale.x, m_vPos.y - m_vScale.y, m_vPos.x + m_vScale.x, m_vPos.y + m_vScale.y);
		SelectObject(CCore::GetInst()->GetmemDC(), oldBrush);
		DeleteObject(hBrush);

	}break;
	case 3:	
	{
		
		hBrush = CreateSolidBrush(RGB(0, 128, 0));
		//hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		oldBrush = (HBRUSH)SelectObject(CCore::GetInst()->GetmemDC(), hBrush);
		Rectangle(CCore::GetInst()->GetmemDC(), m_vPos.x - m_vScale.x, m_vPos.y - m_vScale.y, m_vPos.x + m_vScale.x, m_vPos.y + m_vScale.y);
		SelectObject(CCore::GetInst()->GetmemDC(), oldBrush);
		DeleteObject(hBrush);
	}break;

	}
	




	
	
}

bool CWall::Collision(GObject& vObj)
{
	

	if (m_iHp > 1)
	{
		
		m_iHp -= 1;
	}
	else
	{
		m_vPos.x = -1;
		m_vPos.y = -1;
		m_vScale.x = 0;
		m_vScale.y = 0;
	}

	return false;
}

