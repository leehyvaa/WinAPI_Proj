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
#include "CAnimation.h"
#include "CAnimator.h"

CCore::CCore()
	:m_hWnd(0)
	, m_ptResolution{}
	, m_hDC(0)
	, m_pD2DFactory(nullptr)
	, m_pDCRenderTarget(nullptr)
	, m_hD2DMemoryDC(nullptr)
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
	ReleaseD2DResources();
	ReleaseDC(m_hWnd,m_hDC); // GetDC로 만든 Dc는 릴리즈로 해제

	for (int i = 0; i < static_cast<UINT>(PEN_TYPE::END); i++)
		DeleteObject(m_arrPen[i]);

	for (int i = 0; i < static_cast<UINT>(BRUSH_TYPE::END); i++)
		DeleteObject(m_arrBrush[i]);

	DestroyMenu(m_hMenu);
}





int CCore::init(HWND _hWnd, POINT _ptResolution)
{
	m_hWnd = _hWnd;
	m_ptResolution = _ptResolution;

	// 해상도에 맞게 윈도우 크기 조절
	ChangeWindowSize(m_ptResolution, false);
	
	// 메뉴바 생성
	m_hMenu = LoadMenu(nullptr, MAKEINTRESOURCEW(IDC_WINAPIPROJ));

	m_hDC = GetDC(m_hWnd);

	// 더블버퍼링 용도의 텍스쳐 한장을 만든다.
	m_pMemTex = CResMgr::GetInst()->CreateTexture(L"BackBuffer",static_cast<UINT>(m_ptResolution.x), static_cast<UINT>(m_ptResolution.y));

	
	CreateBrushPen();
	CreateD2DResources();

	// Manager 초기화
	CTimeMgr::GetInst()->init();
	CKeyMgr::GetInst()->init();
	CPathMgr::GetInst()->init();
	CCamera::GetInst()->init();
	CSoundMgr::GetInst()->init();
	CSceneMgr::GetInst()->init();


	// Sound 로드 테스트
	CResMgr::GetInst()->LoadSound(L"BGM_01", L"sound\\BGM_Title.wav");
	CSound* pNewSound = CResMgr::GetInst()->FindSound(L"BGM_01");

	pNewSound->PlayToBGM(true);
	// pNewSound->Play();

	// pNewSound->SetPosition(50.f); //백분율, 소리 위치 설정
	pNewSound->SetVolume(20.f);


	return S_OK;
}



void CCore::Progress()
{
    // Manager 업데이트
    CTimeMgr::GetInst()->Update();
    CKeyMgr::GetInst()->Update();
    CCamera::GetInst()->Update();

    // 씬 업데이트, 충돌처리
    CSceneMgr::GetInst()->Update();
    CCollisionMgr::GetInst()->Update();

    // UI 이벤트 체크
    CUIMgr::GetInst()->Update();

    // 렌더링 시작
    HDC hBackBufferDC = m_pMemTex->GetDC(); // GDI 백버퍼 DC 가져오기
    RECT rc = { 0, 0, m_ptResolution.x, m_ptResolution.y };

    // GDI 렌더링 배경,타일,UI
    Clear(hBackBufferDC);
    CSceneMgr::GetInst()->Render(hBackBufferDC);
    CCamera::GetInst()->Render(hBackBufferDC);
	
    // 애니메이션 렌더링
    // 렌더 타겟을 GDI DC에 바인딩
    m_pDCRenderTarget->BindDC(hBackBufferDC, &rc);
    m_pDCRenderTarget->BeginDraw();
    CSceneMgr::GetInst()->RenderD2D(m_pDCRenderTarget);

    HRESULT hr = m_pDCRenderTarget->EndDraw();
    if (FAILED(hr) && hr == D2DERR_RECREATE_TARGET)
    {
        ReleaseD2DResources();
        CreateD2DResources();
    }

	
    // 최종 출력
    BitBlt(m_hDC, 0, 0, m_ptResolution.x, m_ptResolution.y,
        hBackBufferDC, 0, 0, SRCCOPY);

    CTimeMgr::GetInst()->Render();

    // 이벤트 지연처리
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
    
    m_arrPen[static_cast<UINT>(PEN_TYPE::PURPLE)] = CreatePen(PS_SOLID, 1, RGB(102, 0, 153));
    m_arrPen[static_cast<UINT>(PEN_TYPE::ORANGE)] = CreatePen(PS_SOLID, 1, RGB(255, 165, 0));
    m_arrPen[static_cast<UINT>(PEN_TYPE::BIGGREEN)] = CreatePen(PS_SOLID, 5, RGB(0, 255, 0));
    m_arrPen[static_cast<UINT>(PEN_TYPE::HOLLOW)] = CreatePen(PS_NULL, 1, 0);
}



void CCore::CreateD2DResources()
{
    ReleaseD2DResources();

    // 팩토리 생성
    HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);
    if (FAILED(hr)) return;


    // 속성 지정, GDI랑 32비트 비트맵 호환 
    D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
        D2D1_RENDER_TARGET_TYPE_DEFAULT,
        D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
        0.0f, 0.0f,
        D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE
    );

    // 타겟 생성
    hr = m_pD2DFactory->CreateDCRenderTarget(&props, &m_pDCRenderTarget);
    if (FAILED(hr))
    {
        if (m_pD2DFactory) { m_pD2DFactory->Release(); m_pD2DFactory = nullptr; }
        return;
    }
    
    if (m_pDCRenderTarget)
    {
        m_pDCRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_pBlackBrush);
        m_pDCRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &m_pRedBrush);
        m_pDCRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), &m_pGreenBrush);
        m_pDCRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue), &m_pBlueBrush);
    }
}


void CCore::ReleaseD2DResources()
{
    if (m_pBlackBrush) { m_pBlackBrush->Release(); m_pBlackBrush = nullptr; }
    if (m_pRedBrush)   { m_pRedBrush->Release();   m_pRedBrush = nullptr; }
    if (m_pGreenBrush) { m_pGreenBrush->Release(); m_pGreenBrush = nullptr; }
    if (m_pBlueBrush)  { m_pBlueBrush->Release();  m_pBlueBrush = nullptr; }

    // 렌더 타겟 해제
    if (m_pDCRenderTarget) { m_pDCRenderTarget->Release(); m_pDCRenderTarget = nullptr; }

    // 팩토리 해제
    if (m_pD2DFactory) { m_pD2DFactory->Release(); m_pD2DFactory = nullptr; }
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


void CCore::DockMenu()
{
	// 메뉴바 장착
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




