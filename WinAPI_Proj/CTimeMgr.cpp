
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
	//현재 카운트
	QueryPerformanceCounter(&m_PrevCount);
	//초당 카운트 횟수
	QueryPerformanceFrequency(&m_FreQuency);
}

void CTimeMgr::Update()
{
	QueryPerformanceCounter(&m_CurCount);

	//이전 프레임의 카운팅과, 현재 프레임 카운팅 값의 차이를 구한다.
	m_dDT = (double)(m_CurCount.QuadPart - m_PrevCount.QuadPart) /(double)m_FreQuency.QuadPart;
	
	//이전 카운트 값을 현재값으로 갱신(다음 계산을 위해서)
	m_PrevCount = m_CurCount;

#ifdef _DEBUG
	if (m_dDT > (1. / 60.))
		m_dDT = (1. / 60.);
#endif
	
}

void CTimeMgr::Render()
{
	++m_iCallCount;
	m_dAcc += m_dDT; //DT 누적

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
