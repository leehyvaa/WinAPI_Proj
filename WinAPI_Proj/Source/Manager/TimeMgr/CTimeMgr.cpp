
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
unordered_map<wstring, LARGE_INTEGER> CTimeMgr::m_mapStartTime;

void CTimeMgr::StartTimer(const wstring& _strName)
{
    LARGE_INTEGER start;
    QueryPerformanceCounter(&start);
    m_mapStartTime[_strName] = start;  // 시작 시간 저장
}

void CTimeMgr::EndTimer(const wstring& _strName)
{
    // 시작 시간이 없으면 리턴
    auto startIter = m_mapStartTime.find(_strName);
    if (startIter == m_mapStartTime.end())
        return;

    LARGE_INTEGER end;
    QueryPerformanceCounter(&end);
    
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    
    // 경과 시간을 밀리초 단위로 계산
    double elapsedMs = ((end.QuadPart - startIter->second.QuadPart) * 1000.0) / frequency.QuadPart;
    
    // ProfileResult 업데이트
    auto& data = m_mapProfileData[_strName];
    data.dDuration += elapsedMs;
    data.iCount++;
    data.dAvgMs = data.dDuration / data.iCount;
    
    // 시작 시간 제거
    m_mapStartTime.erase(startIter);
}

void CTimeMgr::ResetProfileData()
{
    m_mapProfileData.clear();
    m_mapStartTime.clear();  // 시작 시간도 클리어
}

void CTimeMgr::RenderProfileData(HDC _dc, int _iOffsetY)
{
    if (m_mapProfileData.empty())
        return;

    int y = _iOffsetY;
    for (auto iter = m_mapProfileData.begin(); iter != m_mapProfileData.end(); ++iter) {
        const auto& name = iter->first;
        const auto& data = iter->second;  // const로 변경 (수정 불필요)
        
        wchar_t szBuf[128];
        swprintf_s(szBuf, L"%s: %.2fms (호출:%d, 평균:%.3fms)",
                  name.c_str(), data.dDuration, data.iCount, data.dAvgMs);
        TextOut(_dc, 10, y, szBuf, wcslen(szBuf));
        y += 20;
    }
}