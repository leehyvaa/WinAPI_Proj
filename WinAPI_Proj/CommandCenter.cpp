#include "pch.h"
#include "CCore.h"
#include "CommandCenter.h"
CommandCenter::CommandCenter()
{

	type = COMMAND;
	m_iHp = 2;

}

CommandCenter::CommandCenter(Vec2 _vPos, Vec2 _vScale)
{
	m_vPos.x = _vPos.x;
	m_vPos.y = _vPos.y;

	this->m_vScale = _vScale;
	
	type = COMMAND;
	m_iHp = 2;

}

CommandCenter::~CommandCenter()
{

}

void CommandCenter::Update()
{


}

void CommandCenter::Render(HDC _dc)
{
	HBRUSH hBrush, oldBrush;

	switch (m_iHp)
	{
	case 1:
	{
		hBrush = CreateSolidBrush(RGB(255, 0, 0));
		//hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		oldBrush = (HBRUSH)SelectObject(CCore::GetInst()->GetmemDC(), hBrush);
		Rectangle(CCore::GetInst()->GetmemDC(), m_vPos.x - m_vScale.x, m_vPos.y - m_vScale.y, m_vPos.x + m_vScale.x, m_vPos.y + m_vScale.y);
		SelectObject(CCore::GetInst()->GetmemDC(), oldBrush);
		DeleteObject(hBrush);


	}break;
	case 2:
	{
		hBrush = CreateSolidBrush(RGB(0, 0, 255));
		//hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		oldBrush = (HBRUSH)SelectObject(CCore::GetInst()->GetmemDC(), hBrush);
		Rectangle(CCore::GetInst()->GetmemDC(), m_vPos.x - m_vScale.x, m_vPos.y - m_vScale.y, m_vPos.x + m_vScale.x, m_vPos.y + m_vScale.y);
		SelectObject(CCore::GetInst()->GetmemDC(), oldBrush);
		DeleteObject(hBrush);

	}break;
	

	}

}

bool CommandCenter::Collision(GObject& vObj)
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
		CCore::GetInst()->GameOver();

	}

	return false;
}
