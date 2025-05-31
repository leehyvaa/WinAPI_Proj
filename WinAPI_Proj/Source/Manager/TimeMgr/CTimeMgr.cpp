
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
	m_dDT = static_cast<double>(m_CurCount.QuadPart - m_PrevCount.QuadPart) /static_cast<double>(m_FreQuency.QuadPart);
	
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
unordered_map<wstring, CTimeMgr::ProfileResult> CTimeMgr::m_mapProfileData;

void CTimeMgr::StartTimer(const wstring& _strName)
{
    LARGE_INTEGER start;
    QueryPerformanceCounter(&start);
    m_mapProfileData[_strName];
    m_mapProfileData[_strName].dDuration -= start.QuadPart;
}

void CTimeMgr::EndTimer(const wstring& _strName)
{
    if (m_mapProfileData.find(_strName) == m_mapProfileData.end())
        return;

    LARGE_INTEGER end;
    QueryPerformanceCounter(&end);
    auto& data = m_mapProfileData[_strName];
    data.dDuration += end.QuadPart;
    data.iCount++;
}

void CTimeMgr::ResetProfileData()
{
    m_mapProfileData.clear();
}

void CTimeMgr::RenderProfileData(HDC _dc, int _iOffsetY)
{
    if (m_mapProfileData.empty())
        return;

    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    double dFreq = static_cast<double>(frequency.QuadPart);

    int y = _iOffsetY;
    for (auto& [name, data] : m_mapProfileData) {
        double dMilliSec = (data.dDuration * 1000.0) / dFreq;
        data.dAvgMs = dMilliSec / (data.iCount > 0 ? data.iCount : 1);

        wchar_t szBuf[128];
        swprintf_s(szBuf, L"%s: %.2fms (호출:%d, 평균:%.3fms)",
                  name.c_str(), dMilliSec, data.iCount, data.dAvgMs);
        TextOut(_dc, 10, y, szBuf, wcslen(szBuf));
        y += 20;
    }
}