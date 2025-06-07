#include "pch.h"
#include "CCore.h"
#include "CKeyMgr.h"
#include "GameObject.h"
#include "CTimeMgr.h"
#include "SPlayer.h"
#include "CSceneMgr.h"
#include "CPathMgr.h"
#include "SelectGDI.h"
#include "CCollisionMgr.h"
#include "CEventMgr.h"
#include "CCamera.h"
#include "CUIMgr.h"
#include "CResMgr.h"
#include "CTexture.h"
#include "SelectGDI.h"
#include "resource.h"
#include "CScene.h"
#include "CBackGround.h"
#include "CSoundMgr.h"
#include "CSound.h"
CCore::CCore()
	:m_hWnd(0)
	, m_ptResolution{}
	, m_hDC(0)
	, m_pD2DFactory(nullptr)
	, m_pDCRenderTarget(nullptr)
	, m_hD2DMemoryDC(nullptr)
	, m_hD2DBitmap(nullptr)
	, m_hOldBitmap(nullptr)
	, m_pBlackBrush(nullptr)
	, m_pRedBrush(nullptr)
	, m_pGreenBrush(nullptr)
	, m_pBlueBrush(nullptr)
	, m_arrPen{}
	, m_arrBrush{}
{
}

CCore::~CCore()
{
	// Direct2D 리소스 해제
	ReleaseD2DResources();

	ReleaseDC(m_hWnd,m_hDC); //GetDC로 만든 Dc는 릴리즈로 해제

	for (int i = 0; i < static_cast<UINT>(PEN_TYPE::END); i++)
	{
		DeleteObject(m_arrPen[i]);
	}

	for (int i = 0; i < static_cast<UINT>(BRUSH_TYPE::END); i++)
	{
		DeleteObject(m_arrBrush[i]);
	}


	DestroyMenu(m_hMenu);
}





int CCore::init(HWND _hWnd, POINT _ptResolution)
{
	


	m_hWnd = _hWnd;
	m_ptResolution = _ptResolution;

	//해상도에 맞게 윈도우 크기 조절
	ChangeWindowSize(m_ptResolution, false);
	
	//메뉴바 생성
	m_hMenu = LoadMenu(nullptr, MAKEINTRESOURCEW(IDC_WINAPIPROJ));

	m_hDC = GetDC(m_hWnd);

	//더블버퍼링 용도의 텍스쳐 한장을 만든다.
	m_pMemTex = CResMgr::GetInst()->CreateTexture(L"BackBuffer",static_cast<UINT>(m_ptResolution.x), static_cast<UINT>(m_ptResolution.y));

	
	CreateBrushPen();
	
	// Direct2D 리소스 초기화
	CreateD2DResources();

	//Manager 초기화
	CTimeMgr::GetInst()->init();
	CKeyMgr::GetInst()->init();
	CPathMgr::GetInst()->init();
	CCamera::GetInst()->init();
	CSoundMgr::GetInst()->init();
	CSceneMgr::GetInst()->init();


	//Sound 로드 테스트
	CResMgr::GetInst()->LoadSound(L"BGM_01", L"sound\\BGM_Title.wav");
	CSound* pNewSound = CResMgr::GetInst()->FindSound(L"BGM_01");

	pNewSound->PlayToBGM(true);
	//pNewSound->Play();

	//pNewSound->SetPosition(50.f); //백분율, 소리 위치 설정
	pNewSound->SetVolume(20.f);


	return S_OK;
}



void CCore::Progress()
{
	//Manager 업데이트
	CTimeMgr::GetInst()->Update();
	CKeyMgr::GetInst()->Update();
	CCamera::GetInst()->Update();

	//씬 업데이트, 충돌처리
	CSceneMgr::GetInst()->Update();
	CCollisionMgr::GetInst()->Update();

	//UI 이벤트 체크
	CUIMgr::GetInst()->Update();

	// 🚀 혁신적 렌더링 파이프라인 시작!
	// 1단계: 기존 GDI 렌더링 완료 (안정성 보장)
	Clear(m_pMemTex->GetDC());
	CSceneMgr::GetInst()->Render(m_pMemTex->GetDC());
	CCamera::GetInst()->Render(m_pMemTex->GetDC());
	
	// 2단계: 별도 메모리 DC에 Direct2D 애니메이션 렌더링 (핵심 혁신!)
	if (m_pDCRenderTarget && m_hD2DMemoryDC)
	{
		OutputDebugStringA("🎨 DC 렌더 타겟 애니메이션 렌더링 시작\n");
		
		// DC 바인딩 갱신 (매 프레임마다 안전하게)
		RECT rc = { 0, 0, m_ptResolution.x, m_ptResolution.y };
		HRESULT hr = m_pDCRenderTarget->BindDC(m_hD2DMemoryDC, &rc);
		if (SUCCEEDED(hr))
		{
			m_pDCRenderTarget->BeginDraw();
			
			// ⭐ 핵심: Clear() 호출 안함! 투명 배경 유지
			// GDI와 충돌하지 않는 애니메이션만 렌더링
			CSceneMgr::GetInst()->RenderD2D(m_pDCRenderTarget);
			
			hr = m_pDCRenderTarget->EndDraw();
			if (FAILED(hr))
			{
				OutputDebugStringA("❌ DC 렌더 타겟 EndDraw 실패\n");
				if (hr == D2DERR_RECREATE_TARGET)
				{
					OutputDebugStringA("🔄 DC 렌더 타겟 재생성 시도\n");
					ReleaseD2DResources();
					CreateD2DResources();
				}
			}
			else
			{
				OutputDebugStringA("✅ DC 렌더 타겟 애니메이션 렌더링 완료\n");
			}
		}
		else
		{
			OutputDebugStringA("❌ DC 바인딩 실패 - GDI로 폴백\n");
		}
	}
	
	// 3단계: 최종 합성 - GDI 백버퍼를 먼저 메인 윈도우에 복사
	BitBlt(m_hDC, 0, 0, m_ptResolution.x, m_ptResolution.y,
		m_pMemTex->GetDC(), 0, 0, SRCCOPY);
	
	// 4단계: Direct2D 메모리 DC를 메인 윈도우에 알파 블렌딩으로 합성
	if (m_hD2DMemoryDC)
	{
		BLENDFUNCTION blend = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
		AlphaBlend(m_hDC, 0, 0, m_ptResolution.x, m_ptResolution.y,
			m_hD2DMemoryDC, 0, 0, m_ptResolution.x, m_ptResolution.y, blend);
		OutputDebugStringA("🎭 Direct2D 오버레이 합성 완료\n");
	}

	CTimeMgr::GetInst()->Render();

	//이벤트 지연처리
	CEventMgr::GetInst()->Update();
}

void CCore::CreateBrushPen()
{
    m_arrBrush[static_cast<UINT>(BRUSH_TYPE::HOLLOW)] = static_cast<HBRUSH>(GetStockObject(HOLLOW_BRUSH));
    m_arrBrush[static_cast<UINT>(BRUSH_TYPE::BLACK)] = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
    m_arrBrush[static_cast<UINT>(BRUSH_TYPE::RED)] = CreateSolidBrush(RGB(255, 0, 0));
    m_arrBrush[static_cast<UINT>(BRUSH_TYPE::MAGENTA)] = CreateSolidBrush(RGB(255, 0, 255));

    m_arrPen[static_cast<UINT>(PEN_TYPE::RED)] = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
    m_arrPen[static_cast<UINT>(PEN_TYPE::GREEN)] = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
    m_arrPen[static_cast<UINT>(PEN_TYPE::BLUE)] = CreatePen(PS_SOLID, 1, RGB(80, 183, 220));
    
    // 추가 펜들 초기화 (ClearD2D에서 이동)
    m_arrPen[static_cast<UINT>(PEN_TYPE::PURPLE)] = CreatePen(PS_SOLID, 1, RGB(102, 0, 153));
    m_arrPen[static_cast<UINT>(PEN_TYPE::ORANGE)] = CreatePen(PS_SOLID, 1, RGB(255, 165, 0));
    m_arrPen[static_cast<UINT>(PEN_TYPE::BIGGREEN)] = CreatePen(PS_SOLID, 5, RGB(0, 255, 0));
    m_arrPen[static_cast<UINT>(PEN_TYPE::HOLLOW)] = CreatePen(PS_NULL, 1, 0);
}



void CCore::CreateD2DResources()
{
    OutputDebugStringA("🚀 혁신적 DC 렌더 타겟 기반 Direct2D 리소스 생성 시작\n");
    
    // 기존 리소스 정리 (안전을 위해)
    ReleaseD2DResources();
    
    // Direct2D Factory 생성
    HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);
    if (FAILED(hr))
    {
        OutputDebugStringA("❌ Direct2D Factory 생성 실패\n");
        return;
    }
    OutputDebugStringA("✅ Direct2D Factory 생성 성공\n");

    // 1단계: Direct2D 전용 메모리 DC 생성
    m_hD2DMemoryDC = CreateCompatibleDC(m_hDC);
    if (!m_hD2DMemoryDC)
    {
        OutputDebugStringA("❌ Direct2D 메모리 DC 생성 실패\n");
        ReleaseD2DResources();
        return;
    }
    OutputDebugStringA("✅ Direct2D 메모리 DC 생성 성공\n");

    // 2단계: Direct2D 전용 비트맵 생성 (32비트 ARGB)
    BITMAPINFO bmi = {};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = m_ptResolution.x;
    bmi.bmiHeader.biHeight = -m_ptResolution.y; // 상하 반전 방지
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    void* pBits = nullptr;
    m_hD2DBitmap = CreateDIBSection(m_hD2DMemoryDC, &bmi, DIB_RGB_COLORS, &pBits, nullptr, 0);
    if (!m_hD2DBitmap)
    {
        OutputDebugStringA("❌ Direct2D 비트맵 생성 실패\n");
        ReleaseD2DResources();
        return;
    }
    OutputDebugStringA("✅ Direct2D 비트맵 생성 성공\n");

    // 3단계: 메모리 DC에 비트맵 선택
    m_hOldBitmap = (HBITMAP)SelectObject(m_hD2DMemoryDC, m_hD2DBitmap);

    // 4단계: DC 렌더 타겟 생성 (핵심 혁신!)
    D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
        D2D1_RENDER_TARGET_TYPE_DEFAULT,
        D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
        0.0f, 0.0f,
        D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE,
        D2D1_FEATURE_LEVEL_DEFAULT
    );

    hr = m_pD2DFactory->CreateDCRenderTarget(&props, &m_pDCRenderTarget);
    if (FAILED(hr))
    {
        OutputDebugStringA("❌ DC 렌더 타겟 생성 실패\n");
        ReleaseD2DResources();
        return;
    }
    OutputDebugStringA("✅ DC 렌더 타겟 생성 성공\n");

    // 5단계: DC 렌더 타겟과 메모리 DC 바인딩
    RECT rc = { 0, 0, m_ptResolution.x, m_ptResolution.y };
    hr = m_pDCRenderTarget->BindDC(m_hD2DMemoryDC, &rc);
    if (FAILED(hr))
    {
        OutputDebugStringA("❌ DC 바인딩 실패\n");
        ReleaseD2DResources();
        return;
    }
    OutputDebugStringA("✅ DC 바인딩 성공\n");

    // 6단계: 브러시들 생성
    if (m_pDCRenderTarget)
    {
        hr = m_pDCRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_pBlackBrush);
        if (FAILED(hr)) { OutputDebugStringA("❌ Black Brush 생성 실패\n"); }
        
        hr = m_pDCRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &m_pRedBrush);
        if (FAILED(hr)) { OutputDebugStringA("❌ Red Brush 생성 실패\n"); }
        
        hr = m_pDCRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), &m_pGreenBrush);
        if (FAILED(hr)) { OutputDebugStringA("❌ Green Brush 생성 실패\n"); }
        
        hr = m_pDCRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue), &m_pBlueBrush);
        if (FAILED(hr)) { OutputDebugStringA("❌ Blue Brush 생성 실패\n"); }
        
        OutputDebugStringA("🎉 DC 렌더 타겟 기반 Direct2D 리소스 생성 완료!\n");
        OutputDebugStringA("💡 GDI와 완전히 분리된 안전한 렌더링 파이프라인 구축\n");
    }
}

void CCore::ReleaseD2DResources()
{
    OutputDebugStringA("🧹 DC 렌더 타겟 리소스 정리 시작\n");
    
    // 브러시들 해제
    if (m_pBlueBrush) { m_pBlueBrush->Release(); m_pBlueBrush = nullptr; }
    if (m_pGreenBrush) { m_pGreenBrush->Release(); m_pGreenBrush = nullptr; }
    if (m_pRedBrush) { m_pRedBrush->Release(); m_pRedBrush = nullptr; }
    if (m_pBlackBrush) { m_pBlackBrush->Release(); m_pBlackBrush = nullptr; }
    
    // DC 렌더 타겟 해제
    if (m_pDCRenderTarget) { m_pDCRenderTarget->Release(); m_pDCRenderTarget = nullptr; }
    
    // GDI 리소스 정리 (순서 중요!)
    if (m_hD2DMemoryDC && m_hOldBitmap) {
        SelectObject(m_hD2DMemoryDC, m_hOldBitmap);
        m_hOldBitmap = nullptr;
    }
    if (m_hD2DBitmap) { DeleteObject(m_hD2DBitmap); m_hD2DBitmap = nullptr; }
    if (m_hD2DMemoryDC) { DeleteDC(m_hD2DMemoryDC); m_hD2DMemoryDC = nullptr; }
    
    // Direct2D Factory 해제
    if (m_pD2DFactory) { m_pD2DFactory->Release(); m_pD2DFactory = nullptr; }
    
    OutputDebugStringA("✅ DC 렌더 타겟 리소스 정리 완료\n");
}

void CCore::Clear(HDC _dc)
{
	//SelectGDI gdi(m_pMemTex->GetDC(), BRUSH_TYPE::BLACK);
	//Rectangle(m_pMemTex->GetDC(), -1, -1, m_ptResolution.x + 1, m_ptResolution.y + 1);
	CScene* curScene = CSceneMgr::GetInst()->GetCurScene();
	CBackGround* backGround = curScene->GetBackGround();
	if (backGround == nullptr)
		return;


	Vec2 vRenderPos = backGround->GetWorldPos();

	Vec2 vScale = backGround->GetScale();


	UINT iWidth = backGround->GetTexture()->Width();
	UINT iHeight = backGround->GetTexture()->Height();


	TransparentBlt(_dc
		, static_cast<int>(vRenderPos.x)
		, static_cast<int>(vRenderPos.y)
		, static_cast<int>(vScale.x), static_cast<int>(vScale.y)
		, backGround->GetTexture()->GetDC()
		, 0, 0,
		iWidth, iHeight, RGB(255, 0, 255));



}

void CCore::ClearD2D()
{
    // Direct2D Clear 제거 - GDI 백그라운드와 충돌 방지
    // 투명한 배경을 유지하여 GDI 더블 버퍼링과 협력
    // 펜 생성은 CreateBrushPen()으로 이동됨
}

void CCore::DockMenu()
{
	//메뉴바 장착
	SetMenu(m_hWnd, m_hMenu);
	ChangeWindowSize(GetResolution(), true);

}

void CCore::DivideMenu()
{
	SetMenu(m_hWnd, nullptr);
	ChangeWindowSize(GetResolution(), false);
}

void CCore::ChangeWindowSize(Vec2 _vResolution, bool _bMenu)
{
	RECT rect = { 0,0, static_cast<long>(_vResolution.x), static_cast<long>(_vResolution.y)};
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(m_hWnd, nullptr, 0, 0, rect.right - rect.left, rect.bottom - rect.top, 0);


}

HDC CCore::GetmemDC()
{
	return m_pMemTex->GetDC();
}




