#pragma once

// Direct2D 헤더
#include <d2d1.h>
#pragma comment(lib, "d2d1.lib")

class CTexture;

//객체를 데이터영역에 넣어두는 방식
class CCore
{
	SINGLE(CCore)

private:
	HWND	m_hWnd;
	POINT	m_ptResolution;
	HDC		m_hDC;

	// Direct2D 리소스 - 혁신적 DC 렌더 타겟 기반 접근법
	ID2D1Factory*				m_pD2DFactory;
	ID2D1DCRenderTarget*		m_pDCRenderTarget;  // 메모리 DC 기반 렌더 타겟
	HDC							m_hD2DMemoryDC;     // Direct2D 전용 메모리 DC
	HBITMAP						m_hD2DBitmap;       // Direct2D 전용 비트맵
	HBITMAP						m_hOldBitmap;       // 이전 비트맵 백업
	ID2D1SolidColorBrush*		m_pBlackBrush;
	ID2D1SolidColorBrush*		m_pRedBrush;
	ID2D1SolidColorBrush*		m_pGreenBrush;
	ID2D1SolidColorBrush*		m_pBlueBrush;

	CTexture* m_pMemTex; //백버퍼 텍스쳐


	//GDI Object
	HBRUSH m_arrBrush[static_cast<UINT>(BRUSH_TYPE::END)];
	HPEN m_arrPen[static_cast<UINT>(PEN_TYPE::END)];

	//메뉴
	HMENU m_hMenu; //Tool씬에서 사용

public:
	int init(HWND _hWnd, POINT _ptResolution);
	void Progress();

	void CreateBrushPen();
	void CreateD2DResources();
	void ReleaseD2DResources();
	void Clear(HDC _dc);
	void ClearD2D();

	void DockMenu();
	void DivideMenu();
	void ChangeWindowSize(Vec2 _vResolution, bool _bMenu);


	HDC GetmemDC();
	HDC GetMainDC() { return m_hDC; }
	HWND GetMainHwnd() { return m_hWnd; }
	POINT GetResolution() { return m_ptResolution; }
	HBRUSH GetBrush(BRUSH_TYPE _eType) { return m_arrBrush[static_cast<UINT>(_eType)]; }
	HPEN GetPen(PEN_TYPE _eType) { return m_arrPen[static_cast<UINT>(_eType)]; }
	
	// Direct2D 접근자 - DC 렌더 타겟 기반
	ID2D1DCRenderTarget* GetD2DRenderTarget() { return m_pDCRenderTarget; }
	ID2D1Factory* GetD2DFactory() { return m_pD2DFactory; }
	HDC GetD2DMemoryDC() { return m_hD2DMemoryDC; }

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

