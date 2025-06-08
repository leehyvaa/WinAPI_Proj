#pragma once
#include <unordered_map>

// DirectWrite 전방 선언
struct ID2D1RenderTarget;

class CTimeMgr
{
	SINGLE(CTimeMgr)

public:
    struct ProfileResult {
        double dDuration;  // 누적 시간 (밀리초)
        UINT iCount;       // 호출 횟수
        double dAvgMs;     // 평균 소요시간
        
        ProfileResult() : dDuration(0.0), iCount(0), dAvgMs(0.0) {}
    };
    
    static unordered_map<wstring, ProfileResult> m_mapProfileData;
    static unordered_map<wstring, LARGE_INTEGER> m_mapStartTime;  // 시작 시간 

private:
	//FPS
	//해당 컴퓨터에서 1프레임당 시간 Delta Time
	LARGE_INTEGER m_CurCount;
	LARGE_INTEGER m_PrevCount;
	LARGE_INTEGER m_FreQuency;

	double m_dDT; //프레임 간의 시간값
	double m_dAcc; //1초 체크를 위한 누적 시간
	UINT m_iCallCount;//함수 호출 횟수 체크
	UINT m_iFPS;//초당 호출 체크

public:
	void init();
	void Update();
	void Render();

	double GetDT() { return m_dDT; }
	float GetfDT() { return static_cast<float>(m_dDT); }

    static void StartTimer(const wstring& _strName);
    static void EndTimer(const wstring& _strName);
    static void RenderProfileData(HDC _dc, int _iOffsetY = 20);
    static void RenderProfileDataD2D(ID2D1RenderTarget* _pRenderTarget, int _iOffsetY = 20);
    static void ResetProfileData();
};

