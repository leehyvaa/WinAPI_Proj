
#include "pch.h"
#include "CTimeMgr.h"
#include "CCore.h"
#include <iostream>
CTimeMgr::CTimeMgr()
	: m_CurCount{}
	, m_PrevCount{}
	, m_FreQuency{}
	, m_dDT(0.)
	, m_dAcc(0.)
	, m_iCallCount{0}
	
{

}

CTimeMgr::~CTimeMgr()
{

}

void CTimeMgr::init()
{
	//���� ī��Ʈ
	QueryPerformanceCounter(&m_PrevCount);
	//�ʴ� ī��Ʈ Ƚ��
	QueryPerformanceFrequency(&m_FreQuency);
}

void CTimeMgr::Update()
{
	QueryPerformanceCounter(&m_CurCount);

	//���� �������� ī���ð�, ���� ������ ī���� ���� ���̸� ���Ѵ�.
	m_dDT = (double)(m_CurCount.QuadPart - m_PrevCount.QuadPart) /(double)m_FreQuency.QuadPart;
	
	//���� ī��Ʈ ���� ���簪���� ����(���� ����� ���ؼ�)
	m_PrevCount = m_CurCount;

#ifdef _DEBUG
	if (m_dDT > (1. / 60.))
		m_dDT = (1. / 60.);
#endif
	
}

void CTimeMgr::Render()
{
	++m_iCallCount;
	m_dAcc += m_dDT; //DT ����

	if (m_dAcc >= 1.)
	{
		m_iFPS = m_iCallCount;
		m_dAcc = 0.;
		m_iCallCount = 0;

		wchar_t szBuffer[255] = {};

		swprintf_s(szBuffer, L"FPS : %d,   DT : %lf", m_iFPS, m_dDT);
		SetWindowText(CCore::GetInst()->GetMainHwnd(), szBuffer);
	}
}
