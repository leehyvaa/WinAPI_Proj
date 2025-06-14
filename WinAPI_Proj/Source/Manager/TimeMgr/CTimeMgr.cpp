
#include "pch.h"
#include "CTimeMgr.h"
#include "CCore.h"
#include <iostream>

// DirectWrite 헤더
#include <d2d1.h>
#include <dwrite.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
CTimeMgr::CTimeMgr()
	: m_CurCount{}
	, m_PrevCount{}
	, m_FreQuency{}
	, m_dDT(0.)
	, m_dAcc(0.)
	, m_iCallCount{0}
    , m_dTimeScale(1.0)         
    , m_dSlowMotionTimer(0.0) 
	
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

	// 이전 프레임의 카운팅과, 현재 프레임 카운팅 값의 차이를 구한다.
	m_dDT = static_cast<double>(m_CurCount.QuadPart - m_PrevCount.QuadPart) /static_cast<double>(m_FreQuency.QuadPart);
	
	// 이전 카운트 값을 현재값으로 갱신(다음 계산을 위해서)
	m_PrevCount = m_CurCount;

    // 프레임 방어 코드
#ifdef _DEBUG
	if (m_dDT > (1. / 60.))
		m_dDT = (1. / 60.);
#endif


    // 슬로우 모션 타이머
    if (m_dSlowMotionTimer > 0.0)
    {
        m_dSlowMotionTimer -= m_dDT; 

        if (m_dSlowMotionTimer <= 0.0)
        {
            m_dTimeScale = 1.0; // 시간이 다 되면 TimeScale을 원래대로 복구
            m_dSlowMotionTimer = 0.0;
        }
    }
}

void CTimeMgr::StartSlowMotion(double scale, double duration)
{
    if (scale < 0.0) scale = 0.0;
    if (scale > 1.0) scale = 1.0;

    m_dTimeScale = scale;
    m_dSlowMotionTimer = duration;
}


/*
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
*/

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


void CTimeMgr::RenderProfileData(ID2D1RenderTarget* _pRenderTarget, int _iOffsetY)
{
    if (!_pRenderTarget || m_mapProfileData.empty())
        return;

    // DirectWrite 팩토리 가져오기
    IDWriteFactory* pDWriteFactory = CCore::GetInst()->GetDWriteFactory();
    if (!pDWriteFactory)
        return;

    // 텍스트 포맷 생성 (정적으로 캐싱)
    static IDWriteTextFormat* s_pTextFormat = nullptr;
    if (!s_pTextFormat)
    {
        HRESULT hr = pDWriteFactory->CreateTextFormat(
            L"Arial",                    // 폰트 이름
            nullptr,                     // 폰트 컬렉션
            DWRITE_FONT_WEIGHT_NORMAL,   // 폰트 두께
            DWRITE_FONT_STYLE_NORMAL,    // 폰트 스타일
            DWRITE_FONT_STRETCH_NORMAL,  // 폰트 늘림
            14.0f,                       // 폰트 크기
            L"ko-kr",                    // 로케일
            &s_pTextFormat
        );

        if (SUCCEEDED(hr) && s_pTextFormat)
        {
            s_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
            s_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
        }
    }

    // 텍스트 브러시 생성 (정적으로 캐싱)
    static ID2D1SolidColorBrush* s_pTextBrush = nullptr;
    if (!s_pTextBrush)
    {
        _pRenderTarget->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::White),
            &s_pTextBrush
        );
    }

    if (!s_pTextFormat || !s_pTextBrush)
        return;

    // 배경 브러시 생성 (정적으로 캐싱)
    static ID2D1SolidColorBrush* s_pBackgroundBrush = nullptr;
    if (!s_pBackgroundBrush)
    {
        _pRenderTarget->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::Black, 0.8f),  // 반투명 검은색
            &s_pBackgroundBrush
        );
    }

    if (s_pBackgroundBrush)
    {
        // 배경 사각형 그리기
        D2D1_RECT_F backgroundRect = D2D1::RectF(0, 0, 800, 500);
        _pRenderTarget->FillRectangle(backgroundRect, s_pBackgroundBrush);
    }

    // 프로파일 데이터 렌더링
    float y = static_cast<float>(_iOffsetY);
    for (auto iter = m_mapProfileData.begin(); iter != m_mapProfileData.end(); ++iter)
    {
        const auto& name = iter->first;
        const auto& data = iter->second;
        
        wchar_t szBuf[128];
        swprintf_s(szBuf, L"%s: %.2fms (호출:%d, 평균:%.3fms)",
                  name.c_str(), data.dDuration, data.iCount, data.dAvgMs);

        D2D1_RECT_F textRect = D2D1::RectF(10, y, 780, y + 20);
        
        _pRenderTarget->DrawText(
            szBuf,
            static_cast<UINT32>(wcslen(szBuf)),
            s_pTextFormat,
            textRect,
            s_pTextBrush
        );
        
        y += 20;
    }
}