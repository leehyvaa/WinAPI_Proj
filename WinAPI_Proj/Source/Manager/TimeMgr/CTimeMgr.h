#pragma once
class CTimeMgr
{
	SINGLE(CTimeMgr)
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
	float GetfDT() { return (float)m_dDT; }
};

