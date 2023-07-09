#pragma once
#include"pch.h"
//객체를 데이터영역에 넣어두는 방식
class CCore
{
	SINGLE(CCore);

private:
	HWND	m_hWnd;
	POINT	m_ptResolution;
	HDC		m_hDC;
	HBITMAP m_hBit;
	HDC		m_memDC;

public:
	int init(HWND _hWnd, POINT _ptResolution);
	void Progress();
	void Update();
	void Render();

	HWND GetMainHwnd() { return m_hWnd; }
private:

};







//포인터만 데이터영역에 넣는 방식
//class CCore
//{
//private:
//	static CCore* g_pInst;
//private:
//	CCore();
//	~CCore();
//
//public:
//	static CCore* GetIstance()
//	{
//		//최초호출
//		if (nullptr == g_pInst)
//		{
//			g_pInst = new CCore;
//		}
//
//		return g_pInst;
//	}
//
//
//	static void Release()
//	{
//		if (nullptr != g_pInst)
//		{
//			delete g_pInst;
//			g_pInst = nullptr;
//		}
//	
//	}
//};

