#pragma once
#include <d2d1.h>
#include <dwrite.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

// CBrushManager 관련 헤더 포함
#include "../Manager/BrushMgr/CBrushManager.h"

class CTexture;

// 객체를 데이터영역에 넣어두는 방식
class CCore
{
	SINGLE(CCore)

private:
	HWND	m_hWnd;
	POINT	m_ptResolution;
    
	ID2D1Factory*				m_pFactory;
	ID2D1HwndRenderTarget*		m_pRenderTarget;
	
	// 기존 개별 브러시 멤버는 CBrushManager로 완전 이전됨
    
	IDWriteFactory*				m_pDWriteFactory;

	// 메뉴
	HMENU m_hMenu; // Tool씬에서 사용

public:
	int init(HWND _hWnd, POINT _ptResolution);
	void Progress();

	void CreateResources();
	void ReleaseResources();

	void DockMenu();
	void DivideMenu();
	void ChangeWindowSize(Vec2 _vResolution, bool _bMenu);

	HWND GetMainHwnd() { return m_hWnd; }
	POINT GetResolution() { return m_ptResolution; }
	   
	ID2D1RenderTarget* GetRenderTarget() { return m_pRenderTarget; }
	ID2D1Factory* GetFactory() { return m_pFactory; }
	IDWriteFactory* GetDWriteFactory() { return m_pDWriteFactory; }
	
	// 브러시 접근 인터페이스 (기존 호환성 유지)
	ID2D1SolidColorBrush* GetBlackBrush() { return CBrushManager::GetInst()->GetBlackBrush(); }
	ID2D1SolidColorBrush* GetRedBrush() { return CBrushManager::GetInst()->GetRedBrush(); }
	ID2D1SolidColorBrush* GetGreenBrush() { return CBrushManager::GetInst()->GetGreenBrush(); }
	ID2D1SolidColorBrush* GetBlueBrush() { return CBrushManager::GetInst()->GetBlueBrush(); }
	
	// 새로운 브러시 접근 인터페이스
	ID2D1SolidColorBrush* GetBrush(BrushType type) { return CBrushManager::GetInst()->GetBrush(type); }
	ID2D1SolidColorBrush* GetCustomBrush(float r, float g, float b, float a = 1.0f)
	{
		return CBrushManager::GetInst()->GetCustomBrush(r, g, b, a);
	}

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

